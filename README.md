# 🔥 Smart Fire Detection and Alert System

An Arduino-based intelligent fire detection system that monitors temperature and smoke levels, providing real-time alerts and automated response mechanisms for fire emergencies.

## ✨ Features

- **Real-time temperature and smoke level monitoring**
- **LCD display for system status**
- **Automatic SMS alerts via GSM module**
- **LED status indicators**
- **Buzzer alarm system**
- **Automated water pump control**
- **Dual motor control for emergency response**

## 🛠️ Components Used

- **Arduino Uno R3**
- **DHT11 Temperature and Humidity Sensor**
- **MQ2 Gas/Smoke Sensor**
- **SIM800L GSM Module**
- **LCD Display with I2C Module** (16x2)
- **Red and Green LED**
- **Buzzer**
- **Dual-channel Relay**
- **12V DC Motor**
- **12V Submersible Water Pump**
- **Resistors (150Ω)**
- **Power Sources** (3.7V batteries for SIM module and 12V battery for water pump)

## Circuit Diagram

![Circuit Diagram](images/circuit_diagram.png)

## 💡 How It Works

1. **Temperature and Smoke Detection**: The system uses the DHT11 sensor to measure temperature and the MQ2 sensor to detect smoke levels.
2. **Alert Mechanism**:
   - If smoke levels or temperature exceed specified thresholds, the red LED and buzzer activate.
   - The water pump is powered on intermittently to spray water and reduce fire risk.
   - An SMS alert is sent to a specified number via the SIM800L module.
3. **LCD Display**: The LCD shows the current system status and displays temperature and smoke levels.
4. **Relay Control**: The relays are used to control the water pump and additional motor for potential fire extinguishing actions.
