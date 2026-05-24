# Smart Rabies Vaccination Monitoring System Using RFID and Fingerprint Authentication

## 📌 Overview

The **Smart Rabies Vaccination Monitoring System** is an IoT-based healthcare project developed using **ESP32**, **RFID RC522**, **Fingerprint Sensor**, **Firebase Realtime Database**, and **I2C LCD**.
This system helps hospitals and clinics manage rabies vaccination schedules digitally by securely identifying patients using RFID cards and fingerprint authentication.

The project automatically:

* Registers new patients
* Stores vaccination records in Firebase
* Tracks rabies vaccine schedules
* Detects missed doses
* Displays vaccination status on LCD

---

# 🚀 Features

* RFID-based patient identification
* Fingerprint authentication for secure registration
* Firebase cloud database integration
* Automatic rabies dose schedule tracking
* Missed dose warning system
* LCD status display
* IoT-based real-time patient monitoring
* ESP32 WiFi connectivity

---

# 🛠 Components Used

| Component                       | Quantity  |
| ------------------------------- | --------- |
| ESP32 Dev Board                 | 1         |
| RFID RC522 Module               | 1         |
| RFID Tags/Cards                 | 1+        |
| Fingerprint Sensor (R307/AS608) | 1         |
| 16x2 LCD with I2C Module        | 1         |
| Push Buttons                    | 2         |
| Jumper Wires                    | As needed |
| Breadboard                      | 1         |
| USB Cable                       | 1         |

---

# 🔌 Pin Connections

## 📟 LCD I2C Connection

| LCD | ESP32  |
| --- | ------ |
| SDA | GPIO21 |
| SCL | GPIO22 |
| VCC | 5V     |
| GND | GND    |

---

## 📡 RFID RC522 Connection

| RC522 | ESP32  |
| ----- | ------ |
| SDA   | GPIO5  |
| SCK   | GPIO18 |
| MOSI  | GPIO23 |
| MISO  | GPIO19 |
| RST   | GPIO4  |
| 3.3V  | 3.3V   |
| GND   | GND    |

---

## 👆 Fingerprint Sensor Connection

| Fingerprint Sensor | ESP32  |
| ------------------ | ------ |
| TX                 | GPIO16 |
| RX                 | GPIO17 |
| VCC                | 5V     |
| GND                | GND    |

---

## 🔘 Push Button Connection

| Button          | ESP32  |
| --------------- | ------ |
| Present Button  | GPIO32 |
| Register Button | GPIO33 |

---

# ☁ Firebase Setup

## Step 1: Create Firebase Project

* Open Firebase Console
* Create a new project

## Step 2: Enable Realtime Database

* Build → Realtime Database
* Create Database
* Start in test mode

## Step 3: Database Rules

```json
{
  "rules": {
    ".read": true,
    ".write": true
  }
}
```

---

## Step 4: Enable Authentication

* Authentication → Get Started
* Enable **Anonymous Authentication**

---

## Step 5: Copy Firebase Credentials

```cpp
#define API_KEY "YOUR_API_KEY"
#define DATABASE_URL "YOUR_DATABASE_URL"
```

---

# 📂 Firebase Database Structure

```json
Patients
   UID1234
      fingerID: 1
      dose: 0
      day0: 1712300000
```

---

# 💉 Rabies Vaccination Schedule

| Dose   | Day    |
| ------ | ------ |
| Dose 1 | Day 0  |
| Dose 2 | Day 3  |
| Dose 3 | Day 7  |
| Dose 4 | Day 14 |
| Dose 5 | Day 28 |

---

# ⚙ Working Process

## 🆕 New Patient

1. Scan RFID card
2. Press Register button
3. Scan fingerprint
4. Store patient data in Firebase
5. Assign Day 0 vaccination

---

## 👤 Existing Patient

1. Scan RFID card
2. Fetch patient record from Firebase
3. Check vaccination schedule
4. Display:

   * Give Vaccine
   * Not Due Yet
   * Missed Dose

---

# 📟 Serial Monitor Output

```text
System Starting
RFID Ready
Fingerprint sensor detected
WiFi Connected
Firebase Ready
Waiting for RFID card...
```

---

# 📦 Required Arduino Libraries

Install these libraries from Arduino IDE Library Manager:

* MFRC522
* Adafruit Fingerprint Sensor Library
* LiquidCrystal_I2C
* Firebase ESP Client by Mobizt
* WiFi
* SPI
* Wire

---

# 🧠 Technologies Used

* Embedded Systems
* IoT
* Cloud Database
* RFID Authentication
* Biometric Security
* ESP32 Programming

---

# 🎯 Applications

* Government hospitals
* Veterinary clinics
* Rabies vaccination centers
* Rural healthcare monitoring
* Vaccine tracking systems

---

# 🔮 Future Improvements

* ESP32-CAM patient photo storage
* SMS vaccine reminders
* Mobile application integration
* QR code support
* Online dashboard analytics
* GPS-based vaccination tracking

---

# 📷 Project Output

* RFID card detection
* Fingerprint registration
* Firebase cloud updates
* Vaccine schedule monitoring
* Missed dose alert system

---

# 👨‍💻 Developed Using

* Arduino IDE
* ESP32
* Firebase Realtime Database

---
## 📄 License

This project is licensed under the [MIT License](LICENSE).

---

If you found this project useful, give this repository a ⭐ on GitHub.
