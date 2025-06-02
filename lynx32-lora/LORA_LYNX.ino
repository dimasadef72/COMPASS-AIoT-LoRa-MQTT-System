#include <lorawan.h>

const char *devAddr = "isi dengan devAddr"; // Device Address dari Antares
const char *nwkSKey = "isi dengan nwkSKey"; // Network Session Key dari Antares
const char *appSKey = "isi dengan appSKey"; // Application Session Key dari Antares
  
const unsigned long sendInterval = 600000;// 10 menit interval untuk mengirim data ke LoRa
const unsigned long readInterval = 2000;   // 2 detik interval untuk membaca data dari Arduino
unsigned long previousMillisSend = 0;  // Waktu terakhir pengiriman LoRa
unsigned long previousMillisRead = 0;  // Waktu terakhir pembacaan data sensor dari Arduino
unsigned int counter = 0;     // Counter pesan

char myStr[150];  // Untuk menyimpan data sensor yang akan dikirim, ukuran lebih besar
char outStr[255];  // Buffer untuk data yang diterima dari LoRa
byte recvStatus = 0;    
int channel;

const sRFM_pins RFM_pins = {
  .CS = 5,    // Pin untuk NSS
  .RST = 0,   // Pin untuk RST
  .DIO0 = 27, // Pin untuk DIO0
  .DIO1 = 2, // Pin untuk DIO1
};

HardwareSerial mySerial(1);  // Serial 1, dapat menggunakan Serial1 atau Serial2

void setup() {
  Serial.begin(9600);  
  Serial.println("Menunggu data dari Arduino...");

  // Inisialisasi mySerial dengan baud rate 9600 dan pin RX=16, TX=17
  mySerial.begin(9600, SERIAL_8N1, 26, 5);

  delay(2000);
  if (!lora.init()) {
    Serial.println("LoRa not detected");
    delay(5000);
    return;
  }
  Serial.println("LoRa Initialized");

  // Set LoRaWAN Class
  lora.setDeviceClass(CLASS_A);

  // Set Data Rate
  lora.setDataRate(SF10BW125);

  // Set channel ke random
  lora.setChannel(MULTI);

  // Set TxPower ke 15 dBi (max)
  lora.setTxPower1(15);

  // Masukkan ABP Key dan DevAddress
  lora.setNwkSKey(nwkSKey);
  lora.setAppSKey(appSKey);
  lora.setDevAddr(devAddr);
}

void loop() {
  unsigned long currentMillis = millis();  // Mendapatkan waktu saat ini
  
  // Membaca data dari Arduino setiap 2 detik
  if (millis() - previousMillisRead >= readInterval) {
    previousMillisRead = millis();  // Update waktu pembacaan

    if (mySerial.available()>0) {
      String data = mySerial.readStringUntil('\n');  // Membaca seluruh data yang diterima
      Serial.println("Data diterima dari Arduino:");
      Serial.println(data);  // Menampilkan data yang diterima
    
      // Mengonversi String data menjadi char array untuk pengiriman LoRa
      data.toCharArray(myStr, sizeof(myStr) - 1);  // Membatasi panjang string agar tidak overflow
      myStr[sizeof(myStr) - 1] = '\0';  // Pastikan null-terminated
    }
    else {
      Serial.println("No data received");
    }
  }

  // Mengirim data ke LoRa setiap 5 menit
  if (millis() - previousMillisSend >= sendInterval) {
    previousMillisSend = millis();  // Update waktu pengiriman

    // Kirim data melalui LoRaWAN hanya jika data tersedia
    if (strlen(myStr) > 0) {
      Serial.println("Sending via LoRa: ");
      Serial.println(myStr);
      lora.sendUplink(myStr, strlen(myStr), 0,5);  // Mengirim data melalui LoRa
      counter++;
    }
  }

  // Check Lora RX (menerima data dari LoRa)
  lora.update();

  recvStatus = lora.readData(outStr);
  if (recvStatus) {
    int counter = 0;

    for (int i = 0; i < recvStatus; i++) {
      if (((outStr[i] >= 32) && (outStr[i] <= 126)) || (outStr[i] == 10) || (outStr[i] == 13))
        counter++;
    }

    if (counter == recvStatus) {
      Serial.print(F("Received String : "));
      for (int i = 0; i < recvStatus; i++) {
        Serial.print(char(outStr[i]));
      }
    } else {
      Serial.print(F("Received Hex : "));
      for (int i = 0; i < recvStatus; i++) {
        Serial.print(outStr[i], HEX); Serial.print(" ");
      }
    }
    Serial.println();
  }
}
