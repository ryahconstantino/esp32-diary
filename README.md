# ESP32 Diary

ESP32 Diary is an off-grid journaling application hosted entirely within the flash memory of an ESP32 microcontroller.
[
]()It functions as a standalone web server, creating its own Wi-Fi Access Point and serving a modern Single Page 
Application (SPA). This allows users to write, read, and manage notes without an internet connection, SD card, or
cloud dependency.

## Features

* **Fully Offline:** Operates without internet access. Connect to "Diary" Wi-Fi.
* **Captive Portal:** Automatically redirects users to the diary interface upon connecting to the Wi-Fi network.
* **Persistent Storage:** Entries are saved as JSON files in the flash memory using LittleFS.
* **Live Reload (WebSockets):** Real-time updates across connected devices.
* **Security:** Client-side PIN protection (Default: 0000) with a 24-hour persistent session per device.
* **Storage Management:** Real-time display of available flash memory (in MB).
* **Modern UI:** Dark mode, responsive design, automatic pagination, and confirmation modals for deletion.

## Technology Stack

* **Hardware:** ESP32 (DevKit V1 or compatible).
* **Framework:** Arduino (via PlatformIO).
* **Backend:** C++, ESPAsyncWebServer, ArduinoJson.
* **Frontend:** HTML5, CSS3, Vanilla JavaScript.
* **File System:** LittleFS.

## Prerequisites

To compile and upload this project, you need:

2.  PlatformIO IDE or CLI

## Installation and Compilation

Follow these steps to deploy the diary to your ESP32:

### 1. Open the Project
Open the project folder. PlatformIO should automatically detect the `platformio.ini` file and download the required libraries.

### 2. Upload File System and Firmware
It uploads the `data/` and `src/` folders.

Run the following command in the terminal:

```bash
chmod +x install.sh
./install.sh