# Breath Analysis System using ESP32 and INMP441

## Overview
This project implements a breath analysis system using an ESP32 and an INMP441 MEMS microphone. It records breath sounds, processes the signal to estimate the breath rate, diagnoses possible respiratory conditions, and sends the data to a mobile device via Wi-Fi.

## Features
- Real-time breath sound recording using the INMP441 MEMS microphone.
- Breath rate estimation based on detected peaks in the audio signal.
- Diagnosis of possible respiratory conditions (Bradypnea, Tachypnea, or Normal breathing).
- Wireless data transmission to a mobile device via HTTP.

## Components Required
- **ESP32 Development Board**
- **INMP441 MEMS Microphone**
- **Jumper Wires**
- **Power Supply (5V/3.3V)**
- 
### INMP441 to ESP32 Connections:
| INMP441 Pin | ESP32 Pin |
|------------|----------|
| VDD        | 3.3V     |
| GND        | GND      |
| SCK (BCLK) | GPIO 14  |
| WS (LRCL)  | GPIO 15  |
| SD (DOUT)  | GPIO 32  |

## Installation & Setup
1. **Install Arduino IDE** (if not already installed).
2. **Install Required Libraries**:
   - Install the ESP32 board package in Arduino IDE.
   - Install `WiFi.h`, `HTTPClient.h`, and `driver/i2s.h`.
3. **Update Wi-Fi Credentials**:
   - Replace `YOUR_WIFI_SSID` and `YOUR_WIFI_PASSWORD` in the code with your network details.
4. **Upload Code to ESP32**:
   - Compile and upload the code to the ESP32 using Arduino IDE.
5. **Set Up a Server to Receive Data**:
   - Use a local or cloud server to receive and process the breath rate and diagnosis messages.

## How It Works
1. The ESP32 captures breath sound data using the INMP441 microphone.
2. The breath rate is estimated by detecting peaks in the audio signal.
3. The system determines the diagnosis based on breath rate thresholds.
4. The data (breath rate and diagnosis) is sent to a mobile device via HTTP.
5. The results are displayed on the mobile device.

## Diagnosis Criteria
- **Bradypnea**: Breath rate < 12 breaths per minute.
- **Normal Breathing**: Breath rate between 12-20 breaths per minute.
- **Tachypnea**: Breath rate > 20 breaths per minute.

## Future Improvements
- Integration with an Android/iOS app for real-time monitoring.
- Enhanced signal processing for better accuracy.
- Cloud-based data storage and analytics.

## License
This project is open-source and can be modified for research and development purposes.

---
For any queries or contributions, feel free to reach out!

