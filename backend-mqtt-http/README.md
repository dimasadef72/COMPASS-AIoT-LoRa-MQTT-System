# Backend MQTT-HTTP COMPASS

## Deskripsi  
Backend ini merupakan server Node.js yang menggunakan Express untuk menerima data sensor dari broker MQTT dan juga melalui HTTP POST. Data sensor yang diterima akan diproses, divalidasi, dan disimpan ke database PostgreSQL. Server ini juga menyediakan API HTTP untuk mengakses data mentah maupun data terfilter dari database.

Backend ini dikembangkan untuk proyek COMPASS (Compost Automation Process and Sensor System) yang bertujuan mengotomatisasi pemantauan proses pengomposan berbasis IoT.

---

## Fitur Utama  
- Terhubung dan subscribe ke broker MQTT untuk menerima data sensor secara real-time  
- Parsing dan validasi data sensor sebelum penyimpanan  
- Penyimpanan data ke database PostgreSQL dengan pencegahan duplikasi berdasarkan timestamp  
- Cache data mentah (raw) sensor dalam memori untuk akses cepat tanpa query database  
- API HTTP untuk mengakses data:  
  - `GET /raw` — menampilkan data mentah terbaru dari cache (max 100 data)  
  - `GET /raw/filtered` — menampilkan 15 data terfilter terakhir dari database  
  - `POST /mqtt/raw` — menerima data sensor via HTTP POST (opsional)  
  - `GET /` — endpoint untuk pengecekan status server

---

## Teknologi yang Digunakan  
- **Node.js** sebagai runtime environment  
- **Express.js** untuk framework backend HTTP server  
- **PostgreSQL** sebagai database relasional penyimpan data sensor  
- **pg** Node.js client untuk PostgreSQL  
- **mqtt** Node.js client untuk koneksi dan subscribe ke broker MQTT  
- **JSON** sebagai format komunikasi data sensor

---

## Struktur Database  
Tabel `sensor_data` pada PostgreSQL harus memiliki struktur berikut:  

| Kolom       | Tipe Data       | Keterangan                      |
|-------------|-----------------|--------------------------------|
| temperature | FLOAT           | Suhu dalam derajat Celcius      |
| humidity    | FLOAT           | Kelembapan relatif (%)          |
| gm          | FLOAT           | Gas metana (ppm)                |
| co2         | FLOAT           | Kadar CO2 (ppm)                 |
| vol         | FLOAT           | Volume gas atau variabel lain   |
| tm          | FLOAT           | Variabel waktu/parameter lain   |
| timestamp   | TIMESTAMP (PK)  | Waktu pencatatan data, unik     |

---

## Instalasi & Setup

### 1. Clone Repository  
```bash
git clone <URL-REPO-ANDA>
cd backend-mqtt-http
