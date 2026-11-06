# DryBox - Smart Filament Dryer & Humidity Control System

**Autor:** Alberto Magno (F3an0r)

DryBox é uma caixa de secagem e controle ambiental baseada em ESP8266, desenvolvida para manter filamentos de impressão 3D protegidos contra umidade durante armazenamento e utilização.

O sistema monitora temperatura, umidade relativa e umidade absoluta em tempo real, controlando automaticamente um elemento de aquecimento através de um relé para manter as condições configuradas pelo usuário.

A interface é totalmente web, acessível por computador, smartphone ou tablet através da rede Wi‑Fi local, eliminando a necessidade de displays físicos.

---

## Principais Recursos

- ESP8266 (NodeMCU / ESP-12E)
- Sensor DHT22
- Controle automático de aquecimento
- Interface Web responsiva
- Acesso via navegador
- Atualização em tempo real via AJAX
- Controle por histerese
- Armazenamento de configuração em EEPROM
- Monitoramento de:
  - Temperatura (°C)
  - Umidade Relativa (%)
  - Umidade Absoluta (g/m³)
- Histórico das últimas 12 horas
- Hostname local via mDNS
- Indicador de sinal Wi‑Fi (RSSI)
- Uptime do sistema

---

## Hardware Utilizado

| Componente | Descrição |
|------------|------------|
| ESP8266 | NodeMCU ESP-12E / ESP-12F |
| DHT22 | Sensor de temperatura e umidade |
| Relé 1 Canal | Controle do aquecimento |
| Fonte 5V | Alimentação do sistema |
| Resistência ou lâmpada | Elemento de aquecimento |

---

## Ligações

### DHT22

| DHT22 | ESP8266 |
|---------|---------|
| VCC | 3V3 |
| DATA | D4 |
| GND | GND |

**Importante:**

Utilizar resistor de pull-up de 10 kΩ entre:

- DATA
- 3V3

### Relé

| Relé | ESP8266 |
|---------|---------|
| VCC | 5V |
| GND | GND |
| IN | D1 |

---

## Funcionamento

O DryBox utiliza controle por histerese para evitar acionamentos excessivos do relé.

Exemplo:

- Temperatura alvo: 45°C
- Histerese: 1°C

Comportamento:

- ≥ 45°C → Aquecimento desligado
- ≤ 44°C → Aquecimento ligado

---

## Umidade Absoluta

Além da umidade relativa, o sistema calcula a umidade absoluta utilizando temperatura e umidade medidas pelo DHT22.

A umidade absoluta representa a quantidade real de vapor de água presente no ar e normalmente é um indicador mais útil para secagem de filamentos.

---

## Interface Web

A interface exibe:

- Temperatura atual
- Umidade relativa
- Umidade absoluta
- Estado do aquecimento
- Temperatura alvo
- RSSI do Wi‑Fi
- Uptime do sistema

A atualização é realizada automaticamente através de chamadas AJAX sem recarregamento da página.

---

## Histórico

O sistema mantém:

- 72 amostras
- 1 amostra a cada 10 minutos

Totalizando aproximadamente:

- 12 horas de histórico

---

## Hostname

Quando compatível com o sistema operacional e roteador:

```text
http://drybox.local
```

Também pode ser acessado diretamente pelo IP fornecido pelo roteador:

```text
http://192.168.x.x
```

---

## Compilação

### Bibliotecas Necessárias

Instalar através do Gerenciador de Bibliotecas da Arduino IDE:

```text
DHT sensor library by Adafruit
```

```text
Adafruit Unified Sensor
```

### Pacote ESP8266

Instalar:

```text
ESP8266 by ESP8266 Community
```

---

## Configuração da IDE

Placa:

```text
NodeMCU 1.0 (ESP-12E Module)
```

Velocidade Serial:

```text
115200 baud
```

---

## Estrutura Atual

Versão atual:

```text
DryBox v3.0 Stable
```

Recursos implementados:

- Interface Web
- AJAX
- EEPROM
- DHT22
- Controle de aquecimento
- Histórico 12h
- mDNS
- Tema escuro
- Indicadores visuais

---

## Roadmap Futuro

### v3.1

- Melhorias visuais
- Exibição da qualidade Wi‑Fi em %
- Cartão de status geral
- Melhor apresentação do histórico

### v4.0

- Perfis pré-configurados:
  - PLA
  - PETG
  - ABS
  - ASA
  - TPU
  - Nylon

- Temporizador de secagem
- Desligamento automático
- Estatísticas de secagem

### v5.0

- OTA (atualização via navegador)
- Portal de configuração Wi‑Fi
- Backup e restauração de configurações

---

## Licença

Projeto open-source para uso pessoal, educacional e maker.

Contribuições, melhorias e sugestões são bem-vindas.

---

**DryBox**
  
Desenvolvido por **Alberto Magno (F3an0r)**.
