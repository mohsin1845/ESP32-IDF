# ESP32 LED Control via Web using Espress IDF

The ESP32 LED Control via Web project allows users to control an LED remotely through a web browser. This system hosts a webpage on the ESP32 microcontroller that can be accessed over Wi-Fi. With simple ON and OFF buttons, users can toggle an LED from any device connected to the same network. It’s a lightweight and efficient solution to demonstrate the capabilities of embedded web servers using ESP32.

## Features
- **Wi-Fi Connectivity**: Connects to an existing Wi-Fi network to enable remote control.
- **Web Interface**: Hosts a simple and user-friendly HTML page with ON and OFF buttons.
- **LED Control**: Toggles an LED connected to a GPIO pin of the ESP32 using HTTP requests.
- **Standalone Operation**: Does not require any external server or cloud platform.
- **Responsive Design**: Styled HTML page suitable for both mobile and desktop browsers.
  
## Working Principle
The ESP32 initializes as a Wi-Fi station and connects to the local network. It then starts an HTTP server that serves a webpage. This page contains buttons to control the LED.

The following sequence occurs during operation:

**1.** The user accesses the ESP32’s IP address via a browser.  
**2.** The ESP32 serves a styled HTML page with **Turn ON** and **Turn OFF** buttons.  
**3.** When a button is clicked, a GET request is sent to the ESP32.  
**4.** The ESP32 processes this request and updates the GPIO pin accordingly:
   - `/on`: Turns the LED ON.  
   - `/off`: Turns the LED OFF.

The page dynamically updates to reflect the current state of the LED.

---

The ESP32 can function in three modes:

- **Station Mode (STA)**
- **Access Point Mode (AP)**
- **Station Mode (STA) + Access Point Mode (AP)**
## 1. Station Mode (STA)
In Station Mode, the ESP32 connects to an existing Wi-Fi network.

## 🔧 How it works:

- ESP32 joins your home or office Wi-Fi using SSID and password.
- Once connected, it obtains an IP address from the router.
- The web server is hosted on that IP address.
- Users can visit the IP address in a browser and control the LED using the webpage.
- This mode is ideal for integration into existing network environments.

## 2. Access Point Mode (AP)
In Access Point Mode, the ESP32 creates its own Wi-Fi network, allowing direct connection without any router.

## 🔧 How it works:

- ESP32 creates a hotspot (e.g., ESP32-LED).
-  Users connect their phones or laptops to this Wi-Fi.
- The server is accessible at a static IP (typically 192.168.4.1).
- Users can access the same LED control interface.
- This mode is useful in locations where no Wi-Fi infrastructure exists.
  
## 3. Station + Access Point Mode (AP + STA)
In AP + STA Mode, the ESP32 does both: it connects to an existing Wi-Fi network and simultaneously creates its own access point.

## 🔧 How it works:

- ESP32 connects to your router (as STA) using your SSID and password.
- Simultaneously, it broadcasts its own Wi-Fi network (as AP) — e.g., ESP32-Access-Point.
- You can control the ESP32 in two ways:
    - From your existing Wi-Fi network (via the router-assigned IP address).
    - By connecting directly to the ESP32’s hotspot and accessing it at 192.168.4.1.
- This mode offers maximum flexibility:
- Use STA when in range of a router.
- Fall back to AP when offline or in the field.
  
## ESP32 LED Control System
Below is the circuit Connections of the ESP32 LED Web Control :

![Car Black Box Diagram](https://github.com/mohsin1845/ESP32-IDF/blob/91cf8c6ae00e77a3f3647f689f92440ecdc2b0b8/esp32-led-control-via-web/Img/Screenshot%202025-04-08%20163651.png)

## ESP32 LED CONTROL WEB IMPLEMENTATION
Below is the Implementation of the ESP32 LED Web Control :

![Car Black Box Diagram](https://github.com/mohsin1845/ESP32-IDF/blob/e72b8c790b1feb90b08f019512c9258a5a2b4150/esp32-led-control-via-web/Img/img1.png)
