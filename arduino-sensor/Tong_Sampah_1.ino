#include <DHT.h>
#include <SoftwareSerial.h>
#include <MHZ19.h>
#include <ArduinoJson.h>
#include <Wire.h>              // Memanggil library wire.h
#include <LiquidCrystal_I2C.h> // Memanggil library LCD dengan I2C
LiquidCrystal_I2C lcd(0x27, 20, 4); // Alamat I2C(0x27) LCD dan Jenis LCD (20x4)

#define DHTPIN 2          // Pin untuk sensor DHT22 terhubung ke D2
#define DHTTYPE DHT22     
#define LED_POWER_PIN 13 
#define LED_CO2_PIN 12
#define LED_GAS_PIN 11
#define LED_ULTRASONIK_PIN 10
#define LED_DAY_PIN 9
#define GAS_SENSOR_PIN A0
#define BUZZER_PIN 5
#define TRIG_PIN 3        // Pin untuk trigger sensor ultrasonik
#define ECHO_PIN 4       // Pin untuk echo sensor ultrasonik
#define CO2_TX 6        // TX untuk sensor MH-Z19B
#define CO2_RX 7        // RX untuk sensor MH-Z19B
#define RESET_PIN 8

DHT dht(DHTPIN, DHTTYPE);  // Membuat objek DHT
SoftwareSerial co2Serial(CO2_TX, CO2_RX); // Serial Software
MHZ19 mhz19;

// Variabel untuk waktu 7 hari (7 hari * 24 jam * 60 menit * 60 detik * 1000 milidetik)
unsigned long previousSensorMillis = 0;   // Waktu terakhir mengambil data sensor
unsigned long sensorInterval = 2000; 
unsigned long blinkInterval = 500; // Interval kedipan (500ms)
unsigned long previousBlinkMillis = 0;  // Waktu terakhir kedipan LED
unsigned long startTime = 0;  // Waktu mulai
unsigned long currentTime = 0;
bool ledState = false;
bool lastButtonState = HIGH;  // Status tombol sebelumnya

// Konstanta fisik tong tabung
const float radiusCm = 15.0;    // Jari-jari tabung dalam cm
const float tinggiTotalCm = 40.0;  // Tinggi total tabung dalam cm

void setup() {
  Serial.begin(9600);   // Memulai komunikasi serial untuk melihat data di Serial Monitor
  dht.begin();          // Memulai sensor DHT22

  co2Serial.begin(9600); // Inisialisasi serial untuk sensor CO2
  mhz19.begin(co2Serial); // Inisialisasi MH-Z19B dengan SoftwareSerial

  pinMode(LED_POWER_PIN, OUTPUT);  // Menyetel pin LED power sebagai output
  pinMode(LED_CO2_PIN, OUTPUT);  // Menyetel pin LED suhu sebagai output
  pinMode(LED_GAS_PIN, OUTPUT);  // Menyetel pin LED gas sebagai output
  pinMode(LED_ULTRASONIK_PIN, OUTPUT);  // Menyetel pin LED ultrasonik sebagai output
  pinMode(LED_DAY_PIN, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);  // Set pin trigger sebagai output
  pinMode(ECHO_PIN, INPUT);   // Set pin echo sebagai input
  pinMode(BUZZER_PIN, OUTPUT);  // Set pin buzzer sebagai output
  pinMode(RESET_PIN, INPUT_PULLUP);

  Serial.println("...");
  Serial.println("COMPASS (Compost Process Automation and Sensor System)");
  Serial.println("Unit RIMS");

  startTime = millis(); // Menyimpan waktu mulai saat setup
  
  lcd.init();            // instruksi untuk memulai LCD
  lcd.begin(20,4);      // instruksi untuk menggunakan lcd 20x4
  lcd.backlight();       // instruksi untuk mengaktifkan lampu latar LCD
  //Setup Awal Ketika Running
  lcd.setCursor(7,0);
  lcd.print("COMPASS");
  lcd.setCursor(3,1);
  lcd.print("Compost Sensor");
  lcd.setCursor(8,2);
  lcd.print("....");
  lcd.setCursor(2,3);
  lcd.print("Unit RIM x PENS");
  delay(5000);
  lcd.clear();
}

void loop() {
  // Menyalakan LED power jika data terbaca dengan bena
  digitalWrite(LED_POWER_PIN, HIGH);
  unsigned long currentMillis = millis();  // Mendapatkan waktu saat ini

  bool currentButtonState = digitalRead(RESET_PIN);  // Baca status tombol

  if (lastButtonState == HIGH && currentButtonState == LOW) {
    Serial.println("Tombol ditekan! Reset waktu...");
    startTime = millis();
  }

  currentTime = (millis() - startTime) / 60000;
  lastButtonState = currentButtonState;
  
  delay(50);  // Debouncing sederhana untuk mencegah noise

  if (currentTime > 10080) {
    if (currentMillis - previousBlinkMillis >= blinkInterval) {
      previousBlinkMillis = currentMillis;
      ledState = !ledState;
      digitalWrite(LED_DAY_PIN, ledState ? HIGH : LOW);
    }
  } else {
    digitalWrite(LED_DAY_PIN, LOW); // Jika kurang dari 1 menit, LED tetap mati
  }

  if (currentMillis - previousSensorMillis >= sensorInterval) {
    previousSensorMillis = currentMillis;  // Reset waktu untuk pengambilan sensor berikutnya

    // Membaca suhu dalam Celcius
    float suhu = dht.readTemperature();
    // Membaca kelembapan udara
    float kelembapan = dht.readHumidity();

    // sensor gas
    int gasValue = analogRead(GAS_SENSOR_PIN); // Membaca nilai dari sensor gas (pin analog A0)

    // Membaca data sensor MH-Z19B (CO2)
    int co2Value = mhz19.getCO2();

    // sensor ultrasonik
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);   // Memberi pulsa trigger selama 10 mikrodetik
    digitalWrite(TRIG_PIN, LOW);

    // Membaca durasi pulsa yang diterima oleh pin Echo
    int duration = pulseIn(ECHO_PIN, HIGH);  // Menghitung durasi pulsa yang diterima di echo pin
    int distance = (duration / 2) * 0.0344;  // Menghitung jarak berdasarkan kecepatan suara (0.0344 cm/µs)
    
    // Hitung tinggi isi sampah (cm)
    float tinggiIsi = tinggiTotalCm - distance;
    if(tinggiIsi < 0) tinggiIsi = 0;              // Jika terukur lebih dari tinggi tong, batasi ke 0
    if(tinggiIsi > tinggiTotalCm) tinggiIsi = tinggiTotalCm;  // Batasi max tinggi

    // Hitung volume tabung dalam cm3: V = π r² h
    float volumeCm3 = 3.14 * radiusCm * radiusCm * tinggiIsi;
    float volumeLiter = volumeCm3 / 1000.0;

    float volumeRounded = round(volumeLiter * 100) / 100.0;
    
    // Membuat objek JSON untuk mengonversi data sensor
    StaticJsonDocument<500> doc; // Ukuran buffer untuk JSON
    
    // Menambahkan data sensor ke dalam JSON
    doc["T"] = suhu;        // Menambahkan suhu ke JSON
    doc["RH"] = kelembapan;
    doc["GM"] = gasValue;      // Menambahkan gas ke JSON
    doc["CO2"] = co2Value;      // Menambahkan gas ke JSON
    doc["Vol"] = volumeRounded;
    doc["Tm"]=currentTime;

    // Menyusun JSON menjadi string dalam satu baris
    String output;
    serializeJson(doc, output);  // Tidak ada indentasi atau newline, sehingga JSON dihasilkan dalam satu baris
    Serial.println(output); // Mengirimkan data JSON melalui serial (sebaris)

    lcd.setCursor(1, 0);  // Posisi baris pertama
    lcd.print("Sensor Condition :");

    lcd.setCursor(0, 1);  // Posisi baris pertama
    lcd.print("T :");
    lcd.print(suhu);
    lcd.print("C");

    lcd.setCursor(0, 2);  // Posisi baris pertama
    lcd.print("RH:");
    lcd.print(kelembapan);
    lcd.print("%");

    lcd.setCursor(0, 3);  // Posisi baris pertama
    lcd.print("GM:");
    lcd.print(gasValue);

    lcd.setCursor(10, 1);  // Posisi baris pertama
    lcd.print("CO2:");
    lcd.print(co2Value);

    lcd.setCursor(10, 2);  // Posisi baris pertama
    lcd.print("Vol:");
    lcd.print(volumeRounded);
    lcd.print("L");
    
    // Menghitung hari, jam, dan menit berdasarkan currentTime dalam menit
    int days = currentTime / 1440;           // 1 hari = 1440 menit
    int hours = (currentTime % 1440) / 60;   // Sisa setelah hari dibagi dengan 60 untuk jam
    int minutes = (currentTime % 60);        // Sisa setelah jam dibagi untuk menit

  // Menampilkan hasil konversi pada LCD
    lcd.setCursor(9, 3);   // Menentukan posisi di LCD
    lcd.print("Tm:");      // Menampilkan label Tm
    lcd.print(days);       // Menampilkan jumlah hari
    lcd.print("d");       // Menambahkan "d" untuk hari
    lcd.print(hours);      // Menampilkan jumlah jam
    lcd.print("h");       // Menambahkan "h" untuk jam
    lcd.print(minutes);    // Menampilkan jumlah menit
    lcd.print("m");        // Menambahkan "m" untuk menit

    bool buzzerActive = false;
    
  // Pengecekan kondisi untuk sensor suhu
    if (co2Value > 2000) {  // Misalnya suhu lebih dari 30 derajat C
      tone(BUZZER_PIN, 500);              // Hidupkan buzzer
      digitalWrite(LED_CO2_PIN, HIGH);
      buzzerActive = true; 
    } else {
      digitalWrite(LED_CO2_PIN, LOW);    // LED Suhu mati
    }

    // Pengecekan kondisi untuk sensor gas
    if (gasValue > 1500) {
      tone(BUZZER_PIN, 500);              // Hidupkan buzzer
      digitalWrite(LED_GAS_PIN, HIGH);    // LED Gas menyala
      buzzerActive = true; 
    } else {
      digitalWrite(LED_GAS_PIN, LOW);     // LED Gas mati
    }

    // Pengecekan kondisi untuk sensor ultrasonik
    if (distance < 5) {
      tone(BUZZER_PIN, 500); 
      digitalWrite(LED_ULTRASONIK_PIN, HIGH); // LED Ultrasonik menyala
      buzzerActive = true; 
    } else {
      digitalWrite(LED_ULTRASONIK_PIN, LOW);  // LED Ultrasonik mati
    }

    // Jika tidak ada sensor yang memenuhi kondisi, matikan buzzer
    if (!buzzerActive) {
      noTone(BUZZER_PIN);  // Matikan buzzer jika tidak ada sensor yang aktif
    }
  }
  
  // Cek apakah tombol reset ditekan
}
