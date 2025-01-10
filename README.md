# Autonomous RC Car using Gemini

This project demonstrates a WiFi-controlled motor system that uses the Gemini 1.5 API to process user queries and control motor actions. The system is equipped with an LCD display for status updates and supports directional commands (forward, backward, left, right, and stop).

## Table of Contents
- [Overview](#overview)
- [Features](#features)
- [Hardware Requirements](#hardware-requirements)
- [Software Requirements](#software-requirements)
- [Wiring Diagram](#wiring-diagram)
- [Installation](#installation)
- [Usage](#usage)
- [Code Explanation](#code-explanation)

## Overview
This project combines Arduino with the Gemini API to process text-based user queries. It receives commands through a serial monitor, processes them via the API, and executes appropriate motor actions (e.g., moving forward or backward). The LCD display shows system status and command responses.

## Features
- WiFi connectivity for API communication.
- Integration with Gemini 1.5 API for natural language processing.
- Motor control for forward, backward, left, right, and stop commands.
- LCD display for real-time status updates.

## Hardware Requirements
- ESP32 or Arduino with WiFi capabilities
- Motor driver (e.g., L298N)
- 2 DC motors
- LCD display (16x2)
- Jumper wires
- Power supply for motors

## Software Requirements
- Arduino IDE
- [ArduinoJson](https://arduinojson.org/) library
- WiFi library (comes pre-installed for ESP32)

## Wiring Diagram
| Component       | Pin Connection |
|-----------------|----------------|
| LCD RS          | GPIO 19        |
| LCD Enable (E)  | GPIO 33        |
| LCD D4          | GPIO 18        |
| LCD D5          | GPIO 22        |
| LCD D6          | GPIO 25        |
| LCD D7          | GPIO 23        |
| Motor A IN1     | GPIO 14        |
| Motor A IN2     | GPIO 12        |
| Motor B IN3     | GPIO 13        |
| Motor B IN4     | GPIO 4         |
| Motor A ENA     | GPIO 5         |
| Motor B ENB     | GPIO 22        |

## Installation
1. **Clone the Repository**
   ```bash
   git clone https://github.com/your-username/your-repo-name.git
   ```
2. **Open the Project in Arduino IDE**
   - Navigate to the downloaded folder and open the `.ino` file.

3. **Install Required Libraries**
   - Install the [ArduinoJson](https://arduinojson.org/) library via Arduino Library Manager.

4. **Update WiFi and API Credentials**
   Replace placeholders in the code with your WiFi and Gemini API credentials:
   ```cpp
   const char* ssid = "WIFI_SSID";
   const char* password = "WIFI_PASSWORD";
   const char* Gemini_Token = "GEMINI_API";
   ```

5. **Upload the Code**
   Connect your ESP32/Arduino to your computer and upload the code.

## Usage
1. Open the Serial Monitor in Arduino IDE.
2. Enter a text-based command (e.g., "Move forward").
3. The command is sent to the Gemini API, which processes it and returns a response.
4. The system parses the response and executes the motor action.
5. The LCD displays the system status and action taken.

## Code Explanation
### Key Functionalities
- **WiFi Setup**:
  Connects to the specified WiFi network.
  ```cpp
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  ```

- **Gemini API Integration**:
  Sends user input to the API and retrieves responses.
  ```cpp
  HTTPClient https;
  if (https.begin("https://generativelanguage.googleapis.com/v1beta/models/gemini-1.5-flash:generateContent?key=" + (String)Gemini_Token)) {
      ...
  }
  ```

- **Motor Control**:
  Uses GPIO pins to control motor directions.
  ```cpp
  void moveForward() {
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      analogWrite(ENA, 255);
  }
  ```

- **LCD Display**:
  Displays status updates and command responses.
  ```cpp
  lcd.print("Processing...");
  ```

---
Feel free to contribute or report issues! Happy tinkering!

