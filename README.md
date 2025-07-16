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

> LED toggle and console logs confirm working transmission & reception logic.  
> *(You can add a screenshot or photo here if desired)*

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

This project is intended for academic and research use only.  
All code and documentation © Aanjaneya Pandey 2025.

