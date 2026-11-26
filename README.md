## identitas
NAMA : Dimas Purwo Tri Wicaksono
NIM  : 622025025

## Deskripsi Aplikasi
Aplikasi Pengelolaan Karakter, Misi, dan Navigasi Peta Dunia Fantasi adalah aplikasi permainan peran (Role-Playing Game / RPG) berbasis konsol yang dibangun menggunakan bahasa C (standar C99) dengan arsitektur modular (include/, src/, dan data/). Aplikasi ini dirancang untuk membantu pengguna mengelola akun petualang, membuat dan memilih karakter aktif dengan peran (role) serta statistik tertentu, membeli perlengkapan sesuai role melalui validasi gold, menjalankan misi secara terstruktur menggunakan Queue dan Stack, serta menjelajahi wilayah dunia fantasi berbasis Graph dan algoritma jalur terpendek Dijkstra. Seluruh data penting disimpan secara persisten ke dalam berkas teks dan dilengkapi kontrol layar konsol (system("cls"), getch(), serta perulangan sub-menu) agar alur penggunaan tetap rapi dan konsisten.

## Daftar Fitur 
1. Autentikasi & Manajemen Akun (auth)
Registrasi akun baru dengan pengecekan agar tidak ada username ganda.
Login akun dengan validasi username dan password, serta fitur logout.
Penyimpanan data akun secara persisten di data/users.txt.
2. Manajemen Karakter & Role Baku (karakter)
Pembuatan karakter baru dengan lima pilihan role baku berbasis angka (1–5): Assassin, Marksman, Mage, Fighter, dan Tank.
Setiap karakter menyimpan atribut Level, HP, ATK, dan Gold miliknya sendiri di data/karakter.txt.
Dukungan banyak karakter dalam satu akun serta fitur Ganti Karakter Aktif agar gold, inventori, misi, dan lokasi terikat pada karakter aktif yang sedang dimainkan.
3. Katalog Item per Role, Pembelian & Validasi Gold (inventori)
Katalog item yang disesuaikan dengan role karakter aktif ditambah item umum (Potion), lengkap dengan informasi harga dan bonus statistik (ATK/HP).
Pengecekan kecukupan Gold karakter aktif (harga × jumlah) sebelum transaksi diproses; pembelian otomatis ditolak jika gold kurang.
Pemotongan Gold, penambahan bonus stat, akumulasi jumlah item yang sama, dan penyimpanan persisten ke data/inventori.txt.
4. Sorting & Searching Data (karakter & inventori)
Pengurutan (sorting) karakter berdasarkan Level, Gold, atau Nama (A–Z), serta pencarian (searching) berdasarkan nama atau role.
Pengurutan (sorting) inventori karakter aktif berdasarkan Harga Satuan, Jumlah, atau Nama Item (A–Z), serta pencarian berdasarkan nama item atau tipe.
5. Sistem Misi Berbasis Queue & Stack (misi)
Antrean Misi Aktif (Queue - FIFO): Mengambil misi ke antrean aktif dan memproses misi yang paling awal masuk (dequeue) saat misi diselesaikan.
Riwayat Misi Selesai (Stack - LIFO): Misi yang selesai masuk ke tumpukan riwayat sehingga misi terbaru berada di posisi teratas (TOP), sekaligus memberikan hadiah Gold, bonus ATK/HP, dan kenaikan Level ke karakter aktif.
Penyimpanan status misi secara persisten di data/misi.txt.
6. Navigasi Peta Dunia Fantasi Berbasis Graph (peta)
Representasi peta dunia fantasi menggunakan Adjacency Matrix beserta bobot jarak antarwilayah.
Melihat wilayah saat ini beserta daftar wilayah tetangga dan jaraknya, serta berpindah ke wilayah tetangga yang terhubung langsung.
Pencarian rute jarak terpendek dari posisi karakter saat ini ke wilayah tujuan menggunakan algoritma Dijkstra.
Penyimpanan posisi terakhir tiap karakter di data/lokasi.txt.
7. Antarmuka Konsol & Pengendalian Alur (main)
Validasi input numerik dan pembersihan buffer (getchar()) untuk mencegah kesalahan pembacaan atau infinite loop saat input tidak sesuai.
Pembersihan layar dengan system("cls") dan jeda tekan tombol dengan getch() sebelum kembali menampilkan menu.
Perulangan khusus di dalam sub-menu Sorting & Searching Karakter, Sorting & Searching Inventori, Sistem Misi, dan Navigasi Peta sehingga tetap berada di sub-menu yang sama sampai dipilih kembali (0).
Untuk mengompilasi dan menggabungkan (compile & link) seluruh kode 

## Kompilasi Kode
- sumber C menjadi file executable app.exe :
gcc -std=c99 -Wall -Wextra -Iinclude src\main.c src\auth.c src\karakter.c src\inventori.c src\misi.c src\peta.c -o app.exe

- untuk menjalankan hasil kompilasi tersebut secara langsung.
.\app.exe
