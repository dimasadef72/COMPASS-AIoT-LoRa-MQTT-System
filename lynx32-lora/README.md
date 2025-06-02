# LoRa Communication Module (LYNX-32)

## Deskripsi  
Modul ini berisi kode program untuk komunikasi data sensor menggunakan perangkat LYNX-32 dengan teknologi LoRaWAN. Kode bertugas untuk menerima data dari Arduino melalui serial, lalu mengirim data tersebut secara berkala ke Gateway LoRa milik Telkom IoT Platform.

---

## Fitur Utama  
- Inisialisasi dan konfigurasi LoRaWAN (ABP mode) dengan DevAddr, NwkSKey, dan AppSKey  
- Membaca data sensor dari Arduino melalui komunikasi serial setiap 2 detik  
- Mengirim data ke Gateway LoRa setiap 10 menit  
- Menerima data dari LoRa dan menampilkan hasilnya di serial monitor  
- Pengaturan channel, data rate, dan TxPower untuk optimasi komunikasi  

---

## Library yang Digunakan  
- **lorawan.h** — untuk komunikasi LoRaWAN  

---

## Konfigurasi Penting  
- `devAddr`, `nwkSKey`, dan `appSKey` harus disesuaikan dengan kredensial dari platform Antares atau Telkom IoT Platform  
- Pin yang digunakan untuk modul LoRa sudah disesuaikan dengan hardware LYNX-32 (NSS, RST, DIO0, DIO1)  
- Baud rate serial untuk komunikasi dengan Arduino adalah 9600  
- Interval pengiriman data LoRa diset setiap 10 menit (600.000 ms)  
- Interval pembacaan data serial Arduino diset setiap 2 detik (2.000 ms)  

---

## Cara Penggunaan  
1. Sesuaikan kredensial LoRaWAN (devAddr, nwkSKey, appSKey) sesuai data dari platform IoT yang digunakan  
2. Hubungkan LYNX-32 dengan Arduino Uno R3 melalui komunikasi serial (pin RX/TX sesuai kode)  
3. Upload kode ke board LYNX-32 menggunakan IDE yang sesuai  
4. Monitor data yang masuk dan terkirim pada serial monitor  
5. Pastikan frekuensi LoRa sesuai dengan regional (kode ini menggunakan 922.4 MHz)  

---

## Struktur Kode  
- Setup LoRa dan serial communication  
- Loop utama membaca data serial Arduino dan mengirimkan data via LoRa secara berkala  
- Monitoring data LoRa yang diterima dan menampilkannya ke serial monitor  

---

## Catatan  
- Pastikan modul LoRa terpasang dan terdeteksi dengan benar  
- Sesuaikan pin hardware jika menggunakan konfigurasi berbeda  
- Waktu interval dapat diubah sesuai kebutuhan aplikasi  
