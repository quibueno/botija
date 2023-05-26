# Programa de Envio de Dados para InfluxDB na Nuvem

Este programa foi desenvolvido para um dispositivo ESP32 com a finalidade de enviar dados de medição de uma célula de carga para um servidor InfluxDB na nuvem.

O programa realiza as seguintes funcionalidades:

- Conexão à rede Wi-Fi: O dispositivo se conecta a uma rede Wi-Fi usando as informações de SSID e senha fornecidas.
- Medição de peso: Utilizando uma célula de carga e o módulo HX711, o programa realiza a medição do peso em uma unidade configurada (quilogramas, neste caso).
- Configuração: É possível configurar o SSID e senha da rede Wi-Fi, assim como a tara (valor de calibração) por meio de uma interface web.
- Envio de dados: Os dados de medição são enviados periodicamente para um servidor InfluxDB na nuvem usando a API HTTP.

## Requisitos

- Plataforma: ESP32
- Bibliotecas: WiFi, WebServer, HTTPClient, HX711
- Ambiente de Desenvolvimento: Arduino IDE

## Configuração

Antes de utilizar o programa, é necessário realizar algumas configurações:

1. Defina as informações de conexão para o servidor InfluxDB na nuvem:
   - `INFLUXDB_URL`: URL do servidor InfluxDB na nuvem.
   - `INFLUXDB_TOKEN`: Token de autenticação para acessar o servidor InfluxDB.
   - `INFLUXDB_ORG`: ID da organização no servidor InfluxDB.
   - `INFLUXDB_BUCKET`: Nome do bucket no servidor InfluxDB onde os dados serão armazenados.

2. Defina as informações de conexão para a rede Wi-Fi:
   - `ssid`: Nome da rede Wi-Fi.
   - `password`: Senha da rede Wi-Fi.

3. Defina as informações específicas da célula de carga e calibração:
   - `CELULA_DT`: Pino DT (Data) do módulo HX711 conectado à célula de carga.
   - `CELULA_SCK`: Pino SCK (Clock) do módulo HX711 conectado à célula de carga.
   - `fator_calib`: Fator de calibração para converter o valor lido pelo HX711 em uma unidade de peso (quilogramas, neste caso).
   - `tara`: Valor de tara para ajustar a medição de peso.

## Uso

1. Faça o upload do programa para o dispositivo ESP32 usando a Arduino IDE.

2. Abra o Monitor Serial na Arduino IDE para visualizar as informações de debug e medição de peso.

3. O dispositivo se conectará à rede Wi-Fi configurada e realizará medições de peso periodicamente.

4. Os dados de medição serão enviados para o servidor InfluxDB na nuvem e armazenados no bucket especificado.

5. O LED embutido no dispositivo piscará a cada envio de dados.

## Contribuição

Este projeto é aberto a contribuições. Sinta-se à vontade para enviar pull requests para adicionar recursos, fazer melhorias ou corrigir problemas.

## Autor

Este programa foi desenvolvido por Luiz Quirino

