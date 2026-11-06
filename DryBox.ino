#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>
#include <DHT.h>

#define DHTPIN D4
#define DHTTYPE DHT22
#define RELAY_PIN D1
#define HIST_SIZE 72
#define SAMPLE_INTERVAL 600000UL

const char* ssid="SSID";
const char* password="SUA_SENHA";

ESP8266WebServer server(80);
DHT dht(DHTPIN,DHTTYPE);

struct Registro{float t;float h;float ua;};
Registro hist[HIST_SIZE];
int histIndex=0,histCount=0;

float tempAlvo=45.0;
float histerese=1.0;
unsigned long lastSample=0;

float calcUA(float t,float h){return ((6.112*exp((17.67*t)/(t+243.5))*h*2.1674)/(273.15+t));}
void salvarConfig(){EEPROM.put(0,tempAlvo);EEPROM.commit();}
void carregarConfig(){EEPROM.get(0,tempAlvo);if(isnan(tempAlvo)||tempAlvo<20||tempAlvo>80)tempAlvo=45.0;}

String uptime(){unsigned long s=millis()/1000;unsigned d=s/86400;s%=86400;unsigned h=s/3600;s%=3600;unsigned m=s/60;return String(d)+"d "+String(h)+"h "+String(m)+"m";}

void controle(){float t=dht.readTemperature();if(isnan(t))return;if(t>=tempAlvo)digitalWrite(RELAY_PIN,HIGH);if(t<=(tempAlvo-histerese))digitalWrite(RELAY_PIN,LOW);}

void amostrar(){float t=dht.readTemperature();float h=dht.readHumidity();if(isnan(t)||isnan(h))return;hist[histIndex]={t,h,calcUA(t,h)};histIndex=(histIndex+1)%HIST_SIZE;if(histCount<HIST_SIZE)histCount++;}

void handleStatus(){float t=dht.readTemperature();float h=dht.readHumidity();float ua=(isnan(t)||isnan(h))?0:calcUA(t,h);String j="{";j+="\"temp\":"+String(t,1)+",";j+="\"hum\":"+String(h,1)+",";j+="\"ua\":"+String(ua,1)+",";j+="\"target\":"+String(tempAlvo,1)+",";j+="\"rssi\":"+String(WiFi.RSSI())+",";j+="\"relay\":"+String(digitalRead(RELAY_PIN)?0:1)+",";j+="\"uptime\":\""+uptime()+"\"}";server.send(200,"application/json",j);}

void handleConfig(){if(server.hasArg("temp")){float n=server.arg("temp").toFloat();if(n>=20&&n<=80){tempAlvo=n;salvarConfig();}}server.sendHeader("Location","/");server.send(302,"text/plain","");}

void handleHistory(){String h="<html><body style='background:#121212;color:white'><h2>Historico 12h</h2><table border='1'><tr><th>Tempo</th><th>T</th><th>UR</th><th>UA</th></tr>";for(int i=0;i<histCount;i++){int idx=(histIndex-1-i+HIST_SIZE)%HIST_SIZE;h+="<tr><td>-"+String(i*10)+" min</td><td>"+String(hist[idx].t,1)+"</td><td>"+String(hist[idx].h,1)+"</td><td>"+String(hist[idx].ua,1)+"</td></tr>";}h+="</table></body></html>";server.send(200,"text/html",h);}

const char PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head><meta charset='utf-8'><meta name='viewport' content='width=device-width,initial-scale=1'>
<style>
body{background:#121212;color:#fff;font-family:Arial;margin:10px}.card{background:#1e1e1e;padding:12px;border-radius:12px;margin:8px 0}.bar{height:18px;background:#333;border-radius:10px}.fill{height:18px;border-radius:10px}input,button{padding:8px;background:#333;color:#fff;border:1px solid #666}
</style></head><body>
<h2>DryBox v3.0</h2>
<div class='card'>Host: drybox.local<br>IP:<span id='ip'>IPADDR</span><br>RSSI: <span id='rssi'>--</span>dBm<br>Uptime: <span id='up'>--</span></div>
<div class='card'>Temperatura: <span id='temp'>--</span> C<div class='bar'><div id='tb' class='fill' style='background:#ff9800;width:0%'></div></div></div>
<div class='card'>Umidade Relativa: <span id='hum'>--</span>%<div class='bar'><div id='hb' class='fill' style='background:#2196f3;width:0%'></div></div></div>
<div class='card'>Umidade Absoluta: <span id='ua'>--</span> g/m3<div class='bar'><div id='ub' class='fill' style='background:#9c27b0;width:0%'></div></div></div>
<div class='card'><div id='relay'>Carregando...</div></div>
<div class='card'>Temperatura Alvo: <span id='target'>--</span> C<form action='/config'><input name='temp'><button>Salvar</button></form></div>
<div class='card'><a href='/history'>Historico 12h</a></div>
<script>
function upd(){fetch('/status').then(r=>r.json()).then(d=>{temp.textContent=d.temp;hum.textContent=d.hum;ua.textContent=d.ua;target.textContent=d.target;rssi.textContent=d.rssi;up.textContent=d.uptime;relay.innerHTML=d.relay?'<span style="color:#4caf50">● Aquecimento Ligado</span>':'<span style="color:#9e9e9e">● Aquecimento Desligado</span>';tb.style.width=Math.min(d.temp/60*100,100)+'%';hb.style.width=d.hum+'%';ub.style.width=Math.min(d.ua/50*100,100)+'%';});}
setInterval(upd,1000);upd();
</script></body></html>
)rawliteral";

void handleRoot(){String p=FPSTR(PAGE);p.replace("IPADDR",WiFi.localIP().toString());server.send(200,"text/html",p);} 

void setup(){EEPROM.begin(64);carregarConfig();pinMode(RELAY_PIN,OUTPUT);digitalWrite(RELAY_PIN,HIGH);Serial.begin(115200);dht.begin();WiFi.begin(ssid,password);while(WiFi.status()!=WL_CONNECTED){delay(500);}MDNS.begin("drybox");server.on("/",handleRoot);server.on("/status",handleStatus);server.on("/config",handleConfig);server.on("/history",handleHistory);server.begin();amostrar();}

void loop(){controle();server.handleClient();MDNS.update();if(millis()-lastSample>SAMPLE_INTERVAL){lastSample=millis();amostrar();}}
