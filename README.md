# Bank Vault Security System

## Project Overview
This project implements a bank vault system using Arduino and Python, featuring motion detection, PIN authentication, and automated security measures. The system utilizes various sensors and actuators to create a comprehensive security solution for protecting valuable assets in a bank vault scenario.

## Features
- **Motion Detection**: PIR sensor detects movement near the vault and automatically activates security measures
- **PIN Authentication**: Secure digital keypad interface for authorized access using a 4-digit PIN
- **Proximity Detection**: Ultrasonic sensor detects presence and distance of individuals approaching the vault
- **Automated Lock Mechanism**: Servo motor controls the physical locking mechanism
- **Security Camera Integration**: Automatic photo capture when access is granted or denied
- **Alarm System**: Buzzer sounds when security is compromised
- **Auto-Adjusting Lighting**: Ambient light sensor controls interior vault lighting based on environmental conditions
- **Security Protocols**: 
  - Lockdown mode activates after three failed PIN attempts
  - Visual indicators (LED lights) show system status (red for locked/danger, green for unlocked)
- **Data Logging**: All sensor readings are stored in CSV format for security analysis

## Hardware Components
- Arduino microcontroller
- PIR motion sensor
- Ultrasonic distance sensor
- Light intensity sensor
- Servo motor for lock mechanism
- Red, green and white LEDs for status indication
- Buzzer for alarm
- Webcam for security photos
- Keypad interface (implemented in software)

## Software Components
- **Arduino (C/C++)**: Controls all sensors, actuators and communicates with the Python application
- **Python**: 
  - Provides GUI interface for PIN entry
  - Handles security camera functionality
  - Processes and logs sensor data
  - Communicates with Arduino via serial connection

## File Structure
- `main/Bank_Vault.ino`: Arduino code for sensor reading and actuator control
- `main/Bank_Vault.py`: Python application for GUI and high-level security functions
- `Sensor_Arduino.csv`: Log file for sensor readings
- `yolov8n.pt`: Machine learning model (potentially for object/person detection)
- `captures/`: Directory containing security photos taken during access attempts
- `test/`: Directory containing testing code for individual components

## How It Works
1. The system continuously monitors the environment using PIR and ultrasonic sensors
2. When motion is detected within proximity of the vault, the system activates
3. User must enter the correct PIN through the GUI interface
4. If correct PIN is entered, the vault unlocks (servo moves), green LED lights, and a photo is taken
5. If incorrect PIN is entered 3 times, alarm sounds, red LED lights, and a security photo is taken
6. The vault can be manually locked using the "Close" button
7. Lighting inside the vault automatically adjusts based on ambient light conditions

## Setup and Usage
1. Connect all hardware components according to the pin definitions in the Arduino code
2. Upload `Bank_Vault.ino` to the Arduino board
3. Adjust the serial port in `Bank_Vault.py` if needed (currently set to COM10)
4. Run the Python script to start the system
5. The default PIN is set to "1234" (can be changed in the code)

## Future Improvements
- Integration with broader security systems
- Multiple user PIN support with different access levels
- Real-time notifications for unauthorized access attempts
- Biometric authentication options
- Remote monitoring and control capabilities

## Project Context
This project was developed as a final project for the Microcontroller and Sensors course at Calvin Institute, Semester 4.

## Author
Paltiraja Raphael 
