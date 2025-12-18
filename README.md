===== Pendeteksi Kecepatan Kendaraan =====

Projek ini dibuat untuk mendeteksi kecepatan sebuah kendaraan yang melintas di antara 2 IR sensor, dan mendisplay kecepatan kendaraan tersebut serta memberikan warning berupa LED 3 warna dan sebuah buzzer

=== Daftar Komponen ===
* Arduino Nano
* 2 IR Sensor
* LCD 16x2 I2C
* Mini Traffic Light
* Buzzer
* Breadboard

=== Skema ===
<img width="955" height="1007" alt="Skema Pendeteksi Kecepatan" src="https://github.com/user-attachments/assets/fdb9b4b0-3333-49c1-887f-fdc7f93c832d" />

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

=== Logic Rumus ===

<img width="591" height="591" alt="V =" src="https://github.com/user-attachments/assets/2114e6c2-c8b5-474a-8f22-0edbe28bddef" />
