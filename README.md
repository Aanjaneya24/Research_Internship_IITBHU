# 📡 Research Internship – IIT BHU (July 2025)

**Project Title:**  
**Development of LoRa and LoRaWAN Communication using nRF54L15 & SX1261**

**Intern:** Aanjaneya Pandey (NIT Delhi)  
**Mentor:** Dr. Hari Prabhat Gupta  
**Institute:** Department of Computer Science and Engineering, IIT (BHU) Varanasi

---

## 📝 Description

This repository contains the complete work carried out during my research internship at **IIT BHU**. The objective was to establish **Point-to-Point (P2P)** and **LoRaWAN** communication using the **nRF54L15 Development Kit** and **SX1261MB2BAS LoRa shield**.

✅ Verified working code  
✅ Devicetree overlays  
✅ Final PDF report  
✅ Practical results — all are included

---

## 🔬 Objectives

- ✔️ Interface **SX1261 LoRa** module with **nRF54L15 DK**
- ✔️ Build a **LoRa P2P transmitter–receiver system**
- ✔️ Enable **LoRaWAN** end-node functionality with ChirpStack
- ✔️ Provide a working demo of real-time wireless data exchange
- ✔️ Document results with report and code

---

## 📂 Repository Structure

```
Research_Internship_IITBHU/
├── objective/                   # Code for LoRa TX, RX, and LoRaWAN node
│   ├── transmitter/            # P2P TX application
│   ├── receiver/               # P2P RX application
│   ├── lorawan/                # LoRaWAN OTAA node
│   └── overlays/               # Board & pin mapping overlays
├── IITBHU_REPORT_FILE.pdf      # Final internship report
├── README.md
```

---


## 📷 Preview

This section showcases the successful implementation and testing of LoRa and LoRaWAN communication using the nRF54L15 DK and SX1261 module. The images below highlight:

- Real hardware setup and connections
- Console logs confirming data transmission and reception
- Live demonstration of the working system


**Hardware Setup:**
[View nRF54L15 DK with SX1261 LoRa Module (Image)](IMG_3500BA64F448-1%202.JPEG)

**Output – Successful Transmission:**
[View Blinking of LEDs showing LoRa data exchange (Image)](Screenshot%202025-09-17%20at%201.35.58%E2%80%AFAM.png)

---

## 🧰 Environment & Tools

| Tool                | Version/Details               |
|---------------------|-------------------------------|
| **SoC**             | nRF54L15 DK (BLE + LoRa)      |
| **LoRa Module**     | Semtech SX1261MB2BAS          |
| **RTOS**            | Zephyr v2.9.1                 |
| **Toolchain**       | Nordic SDK with nRF Connect   |
| **Debugger**        | SEGGER J-Link                 |
| **Code Editor**     | Visual Studio Code            |
| **Gateway**         | ChirpStack + Docker (for LoRaWAN) |

---

## ⚙️ Setup Instructions

### 1. Clone the Repository

```bash
git clone https://github.com/Aanjaneya24/Research_Internship_IITBHU.git
cd Research_Internship_IITBHU
```

### 2. Build & Flash (for P2P)

```bash
west build -b nrf54l15dk_nrf54l15 objective/transmitter
west flash
```

*Use `receiver/` path similarly for the second board.*

### 3. LoRaWAN Setup (OTAA)

- Register the device in **ChirpStack**
- Configure `dev_eui`, `app_eui`, and `app_key` in `main.c`
- Connect to gateway and view messages on the dashboard

---

## 🛠️ API Key and Configuration

❗ This project does **not** require cloud API keys.  
All communication is done locally via LoRa and LoRaWAN. ChirpStack configuration credentials are embedded in the firmware itself.

---

## 📄 Report

📘 **[Download Final Report – IITBHU_REPORT_FILE.pdf](./IITBHU_REPORT_FILE.pdf)**

Includes:

- System architecture & schematics
- Firmware logic and interfaces
- Experimental results with logs
- Summary and key learnings

---

## 📑 Certificate

📄 **[View Internship Certificate on LinkedIn](https://www.linkedin.com/in/aanjaneya-pandey-9715b2335/overlay/1754292001300/single-media-viewer/?type=DOCUMENT&profileId=ACoAAFRFmeUBzzjYbsTC-O3yws6EvKYSc_1Jqzw)**


## 🧠 Key Learnings

- Embedded RTOS development (Zephyr)
- SPI, GPIO, UART peripheral configuration
- SX1261 LoRa protocol integration
- End-to-end LoRaWAN stack deployment
- Practical debugging and signal analysis

---

## 📬 Contact

**👤 Aanjaneya Pandey**  
GitHub: [@Aanjaneya24](https://github.com/Aanjaneya24)  
---


## 📜 License

This project is licensed under the MIT License.

```
MIT License

Copyright (c) 2025 Aanjaneya Pandey

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

