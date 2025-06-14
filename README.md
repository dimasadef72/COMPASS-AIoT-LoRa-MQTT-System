# COMPASS-AIoT: Compost Automation Process and Sensor System

Sistem pemantauan kualitas kompos berbasis AIoT, dikembangkan saat magang di **Telkom Corporate University (Telkom CorpU)**. Sistem ini terdiri dari sensor lingkungan, komunikasi nirkabel berbasis LoRa, backend pengolahan data MQTT-HTTP, dan AI Chatbot berbasis DifyAI.

## 📁 Struktur Proyek

- `arduino-sensor/`: Program pembacaan dan pengiriman data sensor
- `lynx32-lora/`: Program pengiriman data via LoRa ke Gateway Telkom IoT
- `backend-mqtt-http/`: Backend Node.js (MQTT subscriber & API endpoint)

## 🚀 Fitur Utama

- Monitoring suhu, kelembapan, CO2, gas, dan isi tong kompos
- Komunikasi LoRa + MQTT + HTTP
- Penyimpanan data di PostgreSQL
- Dashboard & endpoint API real-time
- Chatbot AI interaktif (DifyAI)

## 📡 Teknologi Digunakan

Arduino Uno R3, LYNX-32, LoRaWAN, MQTT, Node.js, PostgreSQL, DifyAI, HTTP API

## 🔧 Author

Magang di Telkom CorpU — Junior IoT & AI Engineer: Dimas Ade Firmanda
