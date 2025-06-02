# Arduino Sensor Module

## Deskripsi  
Modul ini berisi kode program untuk pemrograman sensor dan tampilan data menggunakan Arduino Uno R3. Sensor yang terintegrasi meliputi:  
- Sensor suhu dan kelembapan DHT22  
- Sensor gas MH-Z19 (kadar CO2)  
- Penggunaan LCD I2C untuk menampilkan data sensor secara real-time  

Kode ini juga memanfaatkan beberapa library populer untuk membaca sensor dan menampilkan data secara efisien.

---

## Library yang Digunakan  
- **DHT.h** — untuk membaca sensor suhu dan kelembapan DHT22  
- **SoftwareSerial.h** — untuk komunikasi serial dengan sensor eksternal  
- **MHZ19.h** — untuk sensor MH-Z19 mengukur kadar CO2  
- **ArduinoJson.h** — untuk format data JSON pada pengiriman data  
- **Wire.h** — komunikasi I2C untuk LCD  
- **LiquidCrystal_I2C.h** — kontrol LCD I2C 20x4  

---

## Fungsi Utama  
- Membaca data sensor DHT22 dan MH-Z19 secara berkala  
- Memformat data sensor menjadi JSON untuk dikirimkan ke modul LoRa (LYNX-32)  
- Menampilkan informasi suhu, kelembapan, dan kadar CO2 pada LCD I2C 20x4 secara real-time  

---

## Cara Penggunaan  
1. Pasang sensor DHT22, MH-Z19, dan LCD I2C sesuai dengan pin yang telah ditentukan pada kode  
2. Upload kode ini ke board Arduino Uno R3 menggunakan Arduino IDE  
3. Pastikan semua library telah terinstall di Arduino IDE  
4. Sensor akan membaca data dan menampilkan langsung ke LCD, sekaligus menyiapkan data untuk dikirim ke sistem LoRa  

---

## Struktur Kode  
- Inisialisasi library dan LCD  
- Loop utama membaca sensor dan menampilkan data  
- Komunikasi serial dengan modul LoRa untuk pengiriman data sensor  

---

## Catatan  
- Pastikan sensor terhubung dengan benar dan berfungsi optimal  
- Sesuaikan alamat I2C LCD jika menggunakan modul berbeda  
- Interval pengiriman data dapat disesuaikan di dalam kode sesuai kebutuhan  
