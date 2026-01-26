# Step by step demonstration of the BMP180 + MongoDB project

This project demonstrates how to read **temperature, pressure, and altitude** data from a **BMP180 sensor** using an **ESP8266 D1 Mini**, display the data on an **OLED screen**, and send it to a **Flask server** that stores the data in **MongoDB**.

---

##  Project Overview

**System flow:**

BMP180 Sensor → ESP8266 D1 Mini → OLED Display → Flask API → MongoDB

* The ESP8266 reads sensor data every 5 seconds
* Data is displayed locally on an OLED
* Data is sent as JSON to a Flask REST API
* Flask stores the data with timestamps in MongoDB

---

## Hardware Requirements

* ESP8266 D1 Mini
* BMP180 Pressure & Temperature Sensor
* 0.96" OLED Display (SSD1306, I2C)
* Jumper wires
* USB cable

###   I2C Connections (D1 Mini)


With Triple Adapter:


Without Triple Adapter:

| Device | SDA | SCL |
| ------ | --- | --- |
| BMP180 | D2  | D1  |
| OLED   | D2  | D1  |

---

## Software Requirements

### 1️⃣ Install VS Code

Download and install **Visual Studio Code**:
[https://code.visualstudio.com/](https://code.visualstudio.com/)

---

### 2️⃣ Install PlatformIO

1. Open VS Code
2. Go to **Extensions** (`Ctrl + Shift + X`)
3. Search for **PlatformIO IDE**
4. Click **Install**
5. Restart VS Code

---

## 🚀 ESP8266 Setup (C++ / PlatformIO)

### 3️⃣ Create a New PlatformIO Project

1. Open VS Code
2. Click **PlatformIO** icon → **New Project**
3. Fill in:

   * **Project Name:** `BMP180_D1Mini`
   * **Board:** `LOLIN D1 Mini`
   * **Framework:** `Arduino`
4. Click **Finish**

PlatformIO will create the project structure automatically.

---

### 4️⃣ Install Required Libraries

Option1: Open `platformio.ini` and add:

```ini
lib_deps =
  adafruit/Adafruit BMP085 Library
  adafruit/Adafruit SSD1306
  adafruit/Adafruit GFX Library
```

PlatformIO will download the libraries automatically.

Option 2: 
Click Libraries > search for the needed Library ie. Adafruit BMP085, Adafruit SSD1396,Adafruit GFX Library > Click Enter> Add  to project> Select the project you want to add the library to (BMP180_D1Mini)
PlatformIO will then download the libraries to your project

---

### 5️⃣ Add the ESP8266 Code

1. Open `src/main.cpp`
2. Paste the ESP8266 code
3. Update the following values:

```cpp
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";
const char* serverURL = "http://<SERVER_IP>:5000/api/bmp180";
```

⚠️ Replace `<SERVER_IP>` with the IP address of the computer running Flask.

---

### 6️⃣ Upload the Code

1. Connect the D1 Mini via USB
2. Click **Upload** (➡️ icon) in PlatformIO
3. Open **Serial Monitor** to verify output

---

## 🐍 Flask Server Setup (Python)

### 7️⃣ Install Python

Download Python 3:
[https://www.python.org/downloads/](https://www.python.org/downloads/)

Make sure **"Add Python to PATH"** is checked during installation.

---

### 8️⃣ Install Required Python Packages

hOpen a terminal and run:

```bash
pip install flask pymongo
```

---

### 9️⃣ Install MongoDB

Download and install MongoDB Community Server:
[https://www.mongodb.com/try/download/community](https://www.mongodb.com/try/download/community)

Check the following tutorial on how to install MongoDB and its dependencies (Compass,etc) : [https://youtu.be/KYIOJrE3zjk?si=G3zVJcHGrcFiOEm_](https://youtu.be/KYIOJrE3zjk?si=G3zVJcHGrcFiOEm_)

Ensure MongoDB is running on:

```
localhost:27017
```

---

### 🔟 Run the Flask Server

1. N/avigate to the server file
2. Run:

```bash
python bmpserver.py
```

The server will start on:

```
http://0.0.0.0:5000
```

---

## 📡 API Endpoints

| Endpoint      | Method | Description          |
| ------------- | ------ | -------------------- |
| `/api/bmp180` | POST   | Store BMP180 data    |
| /api/sensor   | GET    | Retrieve stored data |
|               |        |                      |

---

##  Example JSON Payload

```json
{
  "temperature": 24.5,
  "pressure": 101325,
  "altitude": 1780
}
```

---

##   Forking & Cloning This Project

### 🔁 Fork the Repository

1. Go to the GitHub repository
2. Click **Fork** (top-right)
3. This creates your own copy

---

### 📥 Clone the Project (Open in VS Code)

Option 1: Open a new file in VS code (Ctrl+ Shift+ N) > Click Clone git repository
PlatformIO will download all libraries automatically
Option 2: Open

```bash
git clone https://github.com/YOUR_USERNAME/BMP180_D1Mini.git
```

Then:

```bash
cd BMP180_D1Mini
code .
```

This opens the project **directly in VS Code**.

---

##  Git Workflow (Quick Reference)

```bash
git add .
git commit -m "Describe your change"
git push
```

---

## 📌 Notes & Tips

* Ensure ESP8266 and Flask server are on the **same network**
* MongoDB must be running before starting Flask
* Use Serial Monitor for debugging
* Auto-save in VS Code is recommended

---

##
