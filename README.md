===== Pendeteksi Kecepatan Kendaraan =====

Projek ini dibuat untuk mendeteksi kecepatan sebuah kendaraan yang melintas di antara 2 IR sensor, dan mendisplay kecepatan kendaraan tersebut serta memberikan warning berupa LED 3 warna dan sebuah buzzer

=== Daftar Komponen ===
* Arduino Nano
* 2 IR Sensor
* LCD 16x2 I2C
* Mini Traffic Light
* Buzzer
* Breadboard

=== Wiring ===
- VCC to 5V
- GND to GND
- IR 1 (Out) to D2
- IR 2 (Out) to D3
- LCD (SDA to A4 || SCL to A5)
- Buzzer (+ to D7)
- Traffic Light (R to D4 || Y to D5 || G to D6)

=== Cara Kerja ===
- IR 1&2 mendeteksi kendaraan yang melewatinya, dengan jarak antar IR 15cm
- Mikrokontroler mengkonversi waktu kendaraan saat melewati IR 1 dan IR 2 ke kmh dengan V=S/T
- LCD menampilkan kecepatan mobil dalam kmh dan mengkategorikan apakah kecepatannya rendah, normal, atau overspeed
- Traffic Light akan menyala sesuai dengan kategori kecepatan kendaraan
- Buzzer juga akan mengeluarkan beep sesuai dengan kategori kecepatan kendaraan
