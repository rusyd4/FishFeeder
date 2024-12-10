# Smart Fish Feeder

## Introduction to the Problem and the Solution

Memelihara ikan dalam akuarium memerlukan perhatian rutin, khususnya dalam pemberian makan dan menjaga kondisi lingkungan. Ketidakteraturan dalam perawatan dapat berdampak buruk pada kesehatan ikan. Selain itu, tidak semua pemilik akuarium memiliki alat pemantau untuk memastikan kondisi lingkungan tetap ideal.

**Smart Fish Feeder** hadir sebagai solusi inovatif dengan menggunakan teknologi IoT untuk pemberian pakan otomatis yang terjadwal dan pemantauan kondisi lingkungan akuarium secara real-time. Sistem ini dirancang untuk memberikan kemudahan bagi pemilik akuarium, terutama mereka yang memiliki keterbatasan waktu.

## Hardware Design and Implementation Details

### Komponen Utama
1. **ESP32**: Sebagai mikrokontroler utama dengan kemampuan Wi-Fi untuk kontrol perangkat dan komunikasi dua arah.
2. **DHT11 Sensor**: Untuk membaca suhu dan kelembapan akuarium secara real-time.
3. **Servo Motor**: Menggerakkan dispenser pakan sesuai interval waktu yang telah diatur.
4. **Buzzer**: Memberikan notifikasi suara selama proses pemberian pakan.
5. **Tombol Interrupt**: Memungkinkan pengguna menghentikan proses secara manual.
6. **LED Indikator**: Memberikan notifikasi visual terkait status sistem.

### Desain Hardware
- Komponen dirangkai menggunakan breadboard untuk pengujian awal.
- Koneksi antar-komponen dibuat dengan kabel jumper.
- Sistem mendapatkan daya dari adaptor atau koneksi USB yang terhubung ke ESP32.

## Software Implementation Details

### Perangkat Lunak
Perangkat lunak dikembangkan menggunakan **Arduino IDE**, dengan ESP32 sebagai pusat kendali sistem. Fitur utama perangkat lunak meliputi:
1. **Pengaturan Koneksi Wi-Fi**: Menyediakan komunikasi antara ESP32 dan aplikasi Blynk.
2. **Pemantauan Suhu dan Kelembapan**: Data dari sensor DHT11 dikirim ke Blynk setiap 2 detik.
3. **Pengaturan Interval Pemberian Pakan**: Slider pada aplikasi Blynk memungkinkan pengguna mengatur waktu pemberian pakan.
4. **Kontrol Manual**: Tombol interrupt memungkinkan penghentian proses pemberian pakan secara manual.

### Integrasi
Integrasi hardware dan software dilakukan melalui jaringan Wi-Fi menggunakan **platform Blynk**. Sistem ini memungkinkan:
- Pemantauan kondisi akuarium secara real-time.
- Kendali perangkat keras seperti servo motor dan indikator LED melalui aplikasi.

### Flowchart

![flowchart](https://i.imgur.com/J6L3W28.png)

## Test Results and Performance Evaluation

Pengujian dilakukan untuk memastikan semua fitur berfungsi dengan baik:
- **Pemantauan Lingkungan**: Sensor DHT11 berhasil membaca dan mengirim data suhu serta kelembapan secara akurat ke aplikasi Blynk.
- **Pemberian Pakan**: Servo motor berhasil bekerja sesuai pengaturan waktu pengguna.
- **Interrupt**: Tombol interrupt terbukti efektif menghentikan proses pemberian pakan kapan saja.
- **Komunikasi**: Koneksi antara ESP32 dan Blynk stabil selama pengujian.

Hasil evaluasi menunjukkan bahwa sistem memenuhi semua kriteria penerimaan dan memberikan performa yang andal.

## Final Product

![final](https://cdn.forback.me/u/QSZoOm.jpg)

## Blynk Visual
**Blynk Application Display:**

![blynk](https://cdn.forback.me/u/ExJB3d.jpg)

## Conclusion and Future Work

**Smart Fish Feeder** berhasil memberikan solusi praktis untuk perawatan akuarium, khususnya dalam pemberian pakan otomatis dan pemantauan kondisi lingkungan. Penggunaan teknologi IoT memastikan kemudahan dan efisiensi bagi pengguna.

### Rencana Pengembangan
1. Menambah fitur deteksi kualitas air untuk pemantauan yang lebih lengkap.
2. Mengintegrasikan sistem dengan kamera untuk pemantauan visual akuarium.
3. Mengoptimalkan konsumsi daya agar lebih hemat energi.
4. Memanfaatkan teknologi AI untuk analisis kesehatan ikan berdasarkan data lingkungan.

---

### Authors
- Naufal Rusyda Santosa
- Yoel Dwi Miryano
- Sihombing Giovano Geraldo
- Emir Fateen Haqqi

**Universitas Indonesia**  
Department of Electrical Engineering  
December 8, 2024
