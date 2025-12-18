//The program is supported by Gemini<3

#include <LiquidCrystal_I2C.h> 
#include <Wire.h> 

// === KONFIGURASI LCD ===
// Alamat default 0x27, jika tidak muncul teks ganti ke 0x3F
LiquidCrystal_I2C lcd(0x27, 16, 2); 

// === DEFINISI PIN ===
const int PIN_MERAH   = 4;
const int PIN_KUNING  = 5;
const int PIN_HIJAU   = 6;
const int PIN_BUZZER  = 7; 

// Pin Sensor (Gunakan pin Digital yang mendukung Interrupt)
const int PIN_IR1 = 2; // Sensor Awal
const int PIN_IR2 = 3; // Sensor Akhir

// === KONSTANTA PENGUKURAN ===
const float JARAK_SENSOR_M = 0.15; // Jarak fisik antar sensor (15 cm)
const int WAKTU_TAMPIL_HASIL = 5000; // Berapa lama hasil tampil di LCD (ms)

// === VARIABEL KONTROL (Volatile untuk Interrupt) ===
volatile unsigned long waktu_t1 = 0; 
volatile unsigned long waktu_t2 = 0; 
volatile bool siap_ukur = true; 
volatile bool pengukuran_selesai = false; 

float kecepatan_kmh = 0.0; 
unsigned long waktu_reset_lcd = 0; 

// --- FUNGSI BUZZER ---
void bunyiBuzzer(int jumlah_beep) {
  for (int i = 0; i < jumlah_beep; i++) {
    digitalWrite(PIN_BUZZER, HIGH);
    delay(100);
    digitalWrite(PIN_BUZZER, LOW);
    if (i < jumlah_beep - 1) delay(100);
  }
}

// --- INTERRUPT SERVICE ROUTINES (ISR) ---
void hitungT1() {
  if (siap_ukur) {
    waktu_t1 = micros();
    siap_ukur = false;
  }
}

void hitungT2() {
  if (!siap_ukur && !pengukuran_selesai) {
    waktu_t2 = micros();
    pengukuran_selesai = true; 
    detachInterrupt(digitalPinToInterrupt(PIN_IR1));
    detachInterrupt(digitalPinToInterrupt(PIN_IR2));
  }
}

// --- FUNGSI RESET SISTEM ---
void resetPengukuran() {
  siap_ukur = true;
  pengukuran_selesai = false;
  
  // Standby: Lampu Hijau menyala
  digitalWrite(PIN_HIJAU, HIGH);
  digitalWrite(PIN_MERAH, LOW);
  digitalWrite(PIN_KUNING, LOW);
  
  attachInterrupt(digitalPinToInterrupt(PIN_IR1), hitungT1, RISING);
  attachInterrupt(digitalPinToInterrupt(PIN_IR2), hitungT2, RISING);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Siap Mengukur...");
  lcd.setCursor(0, 1);
  lcd.print("Max Lim: 5 KMH");
  
  waktu_reset_lcd = 0xFFFFFFFF; // Set ke nilai maksimal agar tidak ter-reset otomatis
}

// --- LOGIKA UTAMA TAMPILAN & INDIKATOR ---
void aturLampuDanTampilan(float kmh) {
  digitalWrite(PIN_HIJAU, LOW);
  digitalWrite(PIN_KUNING, LOW);
  digitalWrite(PIN_MERAH, LOW);

  String status_kecepatan = "";
  int jumlah_beep = 0;

  // LOGIKA: Batas maksimal 5 KMH
  if (kmh >= 5.0) {
    status_kecepatan = "OVERSPEED";
    digitalWrite(PIN_MERAH, HIGH);
    jumlah_beep = 3; // 3x Beep untuk bahaya
  } 
  else if (kmh >= 2.0 && kmh < 5.0) {
    status_kecepatan = "NORMAL   ";
    digitalWrite(PIN_KUNING, HIGH);
    jumlah_beep = 2; // 2x Beep untuk peringatan
  } 
  else {
    status_kecepatan = "RENDAH   ";
    digitalWrite(PIN_HIJAU, HIGH);
    jumlah_beep = 1; // 1x Beep
  }

  // Eksekusi Buzzer
  bunyiBuzzer(jumlah_beep);

  // Tampil LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SPD: ");
  lcd.print(kmh, 2);
  lcd.print(" KMH");

  lcd.setCursor(0, 1);
  lcd.print("STS: ");
  lcd.print(status_kecepatan);
  
  waktu_reset_lcd = millis() + WAKTU_TAMPIL_HASIL; 
}

void setup() {
  Serial.begin(9600);
  
  lcd.init();
  lcd.backlight();
  
  pinMode(PIN_MERAH, OUTPUT);
  pinMode(PIN_KUNING, OUTPUT);
  pinMode(PIN_HIJAU, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  
  pinMode(PIN_IR1, INPUT); 
  pinMode(PIN_IR2, INPUT); 
  
  resetPengukuran();
}

void loop() {
  // Jika pengukuran terdeteksi sensor akhir
  if (pengukuran_selesai) {
    unsigned long selisih_micros = waktu_t2 - waktu_t1;

    // Filter jika ada gangguan sinyal (kurang dari 1ms)
    if (selisih_micros < 1000) {
      resetPengukuran();
      return;
    }
    
    float selisih_detik = (float)selisih_micros / 1000000.0; 
    float mps = JARAK_SENSOR_M / selisih_detik;
    kecepatan_kmh = mps * 3.6; 
    
    aturLampuDanTampilan(kecepatan_kmh);
    pengukuran_selesai = false;
  }
  
  // Timeout: Jika sensor 1 kena tapi sensor 2 tidak kena dalam 5 detik
  if (!siap_ukur && !pengukuran_selesai && (micros() - waktu_t1 > 5000000)) {
    resetPengukuran();
  }
  
  // Kembali ke mode siap ukur setelah hasil tampil selesai
  if (millis() > waktu_reset_lcd) {
    resetPengukuran();
  }
}
