# Data Sending Program for InfluxDB in the Cloud

This is a data sending program designed to run on an ESP32 microcontroller. It reads data from a load cell using the HX711 library and sends the measurements to an InfluxDB database in the cloud. The program also includes a web server that allows configuration of the Wi-Fi credentials and the tare value.

## Features

- Reads weight measurements from a load cell using HX711 library
- Sends the measurements to an InfluxDB database in the cloud
- Provides a web server for configuring Wi-Fi credentials and tare value
- Supports deep sleep mode during specific hours

## Requirements

- ESP32 microcontroller board
- Load cell with HX711 amplifier
- Wi-Fi connection
- InfluxDB database server

## Dependencies

- WiFi library
- WebServer library
- HTTPClient library
- HX711 library
- esp_sleep library
- NTPClient library
- WiFiUdp library

## Installation and Setup

1. Connect the load cell to the ESP32 board following the wiring instructions for the HX711 library.
2. Install the required libraries mentioned above using the Arduino Library Manager.
3. Modify the program code to provide the necessary configuration details:
   - Set your Wi-Fi credentials (`ssid` and `password`) in the code.
   - Set the InfluxDB server address (`influxDB_server`) and token (`influxDB_token`).
   - Customize other settings as needed.
4. Compile and upload the program to your ESP32 board.
5. Open the Serial Monitor to view the program output and check for any errors.
6. Access the ESP32 web server at the assigned IP address to configure Wi-Fi and tare value.
7. The ESP32 will start sending weight measurements to the InfluxDB server at the specified interval.

## Deep Sleep Mode

The program includes a feature to enter deep sleep mode during specific hours (from 21:00 to 09:00). This helps conserve power when the load cell measurements are not needed. During this period, the ESP32 will go into deep sleep and wake up again at 09:00. To enable this feature, make sure to set the correct time zone in the NTPClient configuration (`timeClient`). Additionally, ensure that the ESP32 has a reliable time source to synchronize with (e.g., through an internet connection).

## License

This program is licensed under the GPL v3. You can find the license details in the program source code.

## Author

- Name: Luiz Quirino
- Email: lugumar@gmail.com





