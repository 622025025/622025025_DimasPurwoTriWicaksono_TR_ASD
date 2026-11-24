#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "../include/auth.h"
#include "../include/karakter.h"
#include "../include/inventori.h"
#include "../include/misi.h"
#include "../include/peta.h"

#define MAX_LOKASI 1000

static void bersihkanBuffer(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
}

static void jedaLayar(void) {
    printf("\nTekan tombol apa saja untuk melanjutkan...");
    getch();
    system("cls");
}

static void sinkronkanKarakterAktifAwal(const Karakter daftar[], int jumlah,
                                        const char *pemilik, char *karakterAktif) {
    int i;
    karakterAktif[0] = '\0';
    for (i = 0; i < jumlah; i++) {
        if (strcmp(daftar[i].pemilik, pemilik) == 0) {
            strncpy(karakterAktif, daftar[i].nama, MAX_NAMA_KAR - 1);
            karakterAktif[MAX_NAMA_KAR - 1] = '\0';
            return;
        }
    }
}

int main(void) {
    int pilihan = 0;
    char username[MAX_USER];
    char password[MAX_PASS];
    int sudahLogin = 0;
    char userAktif[MAX_USER];
    char karakterAktif[MAX_NAMA_KAR];
    karakterAktif[0] = '\0';

    Karakter daftarKarakter[MAX_KARAKTER];
    int jumlahKarakter = muatKarakter(daftarKarakter, MAX_KARAKTER);

    ItemInventori daftarInventori[MAX_ITEM];
    int jumlahInventori = muatInventori(daftarInventori, MAX_ITEM);

    CatatanMisi daftarMisi[MAX_MISI];
    int jumlahMisi = muatMisi(daftarMisi, MAX_MISI);

    CatatanLokasi daftarLokasi[MAX_LOKASI];
    int jumlahLokasi = muatLokasiKarakter(daftarLokasi, MAX_LOKASI);

    system("cls");

    while (1) {
        printf("\n======================================================\n");
        printf(" APLIKASI PENGELOLAAN KARAKTER, MISI, & NAVIGASI PETA\n");
        printf("======================================================\n");
        if (!sudahLogin) {
            printf("1. Register Akun\n");
            printf("2. Login Akun\n");
            printf("0. Keluar\n");
            printf("Pilih menu: ");

            if (scanf("%d", &pilihan) != 1) {
                printf("Input harus berupa angka!\n");
                bersihkanBuffer();
                jedaLayar();
                continue;
            }
            bersihkanBuffer();

            if (pilihan == 1) {
                printf("\n--- REGISTER AKUN ---\n");
                printf("Masukkan username (tanpa spasi): ");
                scanf("%29s", username);
                printf("Masukkan password (tanpa spasi): ");
                scanf("%29s", password);
                bersihkanBuffer();

                int status = registerAkun(username, password);
                if (status == 1) {
                    printf("Registrasi berhasil! Silakan login.\n");
                } else if (status == 0) {
                    printf("Username '%s' sudah terdaftar!\n", username);
                }
                jedaLayar();
            } else if (pilihan == 2) {
                printf("\n--- LOGIN AKUN ---\n");
                printf("Username: ");
                scanf("%29s", username);
                printf("Password: ");
                scanf("%29s", password);
                bersihkanBuffer();

                if (loginAkun(username, password)) {
                    sudahLogin = 1;
                    strncpy(userAktif, username, MAX_USER - 1);
                    userAktif[MAX_USER - 1] = '\0';
                    sinkronkanKarakterAktifAwal(daftarKarakter, jumlahKarakter, userAktif, karakterAktif);
                    printf("Login berhasil! Selamat datang, %s.\n", userAktif);
                } else {
                    printf("Username atau password salah!\n");
                }
                jedaLayar();
            } else if (pilihan == 0) {
                system("cls");
                printf("Keluar dari program. Sampai jumpa!\n");
                break;
            } else {
                printf("Pilihan tidak valid.\n");
                jedaLayar();
            }
        } else {
            int idxAktif = cariIndeksKarakterAktif(daftarKarakter, jumlahKarakter, userAktif, karakterAktif);
            if (idxAktif != -1) {
                printf("Akun: %s | Karakter Aktif: %s (%s, Lv.%d, Gold: %d)\n",
                       userAktif,
                       daftarKarakter[idxAktif].nama,
                       daftarKarakter[idxAktif].role,
                       daftarKarakter[idxAktif].level,
                       daftarKarakter[idxAktif].gold);
            } else {
                printf("Akun: %s | Karakter Aktif: (Belum ada)\n", userAktif);
            }

            printf("1. Buat Karakter Baru\n");
            printf("2. Lihat Karakter Saya\n");
            printf("3. Ganti Karakter Aktif\n");
            printf("4. Beli Item untuk Karakter Aktif\n");
            printf("5. Lihat Inventori Karakter Aktif\n");
            printf("6. Urutkan & Cari Karakter\n");
            printf("7. Urutkan & Cari Inventori Karakter Aktif\n");
            printf("8. Sistem Misi (Queue & Stack)\n");
            printf("9. Navigasi Peta Dunia (Graph)\n");
            printf("88. Logout\n");
            printf("0. Keluar\n");
            printf("Pilih menu: ");

            if (scanf("%d", &pilihan) != 1) {
                printf("Input harus berupa angka!\n");
                bersihkanBuffer();
                jedaLayar();
                continue;
            }
            bersihkanBuffer();

            switch (pilihan) {
                case 1:
                    tambahKarakter(daftarKarakter, &jumlahKarakter, userAktif, karakterAktif);
                    jedaLayar();
                    break;
                case 2:
                    tampilkanKarakterUser(daftarKarakter, jumlahKarakter, userAktif, karakterAktif);
                    jedaLayar();
                    break;
                case 3:
                    pilihKarakterAktif(daftarKarakter, jumlahKarakter, userAktif, karakterAktif);
                    jedaLayar();
                    break;
                case 4:
                    beliItemKarakter(daftarInventori, &jumlahInventori,
                                     daftarKarakter, jumlahKarakter,
                                     userAktif, karakterAktif);
                    jedaLayar();
                    break;
                case 5:
                    tampilkanInventoriKarakter(daftarInventori, jumlahInventori, userAktif, karakterAktif);
                    jedaLayar();
                    break;
                case 6: {
                    int sub = 0;
                    system("cls");
                    while (1) {
                        printf("\n--- KARAKTER: SORTING & SEARCHING ---\n");
                        printf("1. Urutkan berdasarkan Level (Tertinggi)\n");
                        printf("2. Urutkan berdasarkan Gold (Tertinggi)\n");
                        printf("3. Urutkan berdasarkan Nama (A-Z)\n");
                        printf("4. Cari Karakter (Nama / Role)\n");
                        printf("0. Kembali ke Menu Utama\n");
                        printf("Pilih: ");
                        if (scanf("%d", &sub) != 1) {
                            printf("Input harus angka!\n");
                            bersihkanBuffer();
                            jedaLayar();
                            continue;
                        }
                        bersihkanBuffer();
                        if (sub == 0) {
                            system("cls");
                            break;
                        } else if (sub == 1 || sub == 2 || sub == 3) {
                            urutkanKarakterUser(daftarKarakter, jumlahKarakter, userAktif, sub);
                            jedaLayar();
                        } else if (sub == 4) {
                            cariKarakterUser(daftarKarakter, jumlahKarakter, userAktif);
                            jedaLayar();
                        } else {
                            printf("Pilihan tidak valid.\n");
                            jedaLayar();
                        }
                    }
                    break;
                }
                case 7: {
                    int sub = 0;
                    system("cls");
                    while (1) {
                        printf("\n--- INVENTORI KARAKTER AKTIF: SORTING & SEARCHING ---\n");
                        printf("1. Urutkan berdasarkan Harga Satuan (Tertinggi)\n");
                        printf("2. Urutkan berdasarkan Jumlah (Tertinggi)\n");
                        printf("3. Urutkan berdasarkan Nama Item (A-Z)\n");
                        printf("4. Cari Item (Nama / Tipe)\n");
                        printf("0. Kembali ke Menu Utama\n");
                        printf("Pilih: ");
                        if (scanf("%d", &sub) != 1) {
                            printf("Input harus angka!\n");
                            bersihkanBuffer();
                            jedaLayar();
                            continue;
                        }
                        bersihkanBuffer();
                        if (sub == 0) {
                            system("cls");
                            break;
                        } else if (sub == 1 || sub == 2 || sub == 3) {
                            urutkanInventoriKarakter(daftarInventori, jumlahInventori, userAktif, karakterAktif, sub);
                            jedaLayar();
                        } else if (sub == 4) {
                            cariInventoriKarakter(daftarInventori, jumlahInventori, userAktif, karakterAktif);
                            jedaLayar();
                        } else {
                            printf("Pilihan tidak valid.\n");
                            jedaLayar();
                        }
                    }
                    break;
                }
                case 8: {
                    int subMisi = 0;
                    system("cls");
                    while (1) {
                        printf("\n--- SISTEM MISI (QUEUE & STACK) ---\n");
                        printf("1. Ambil Misi ke Antrean Aktif (Enqueue)\n");
                        printf("2. Selesaikan Misi Terdepan (Dequeue -> Push Riwayat)\n");
                        printf("3. Lihat Antrean Misi Aktif (Queue)\n");
                        printf("4. Lihat Riwayat Misi Selesai (Stack LIFO)\n");
                        printf("0. Kembali ke Menu Utama\n");
                        printf("Pilih: ");
                        if (scanf("%d", &subMisi) != 1) {
                            printf("Input harus angka!\n");
                            bersihkanBuffer();
                            jedaLayar();
                            continue;
                        }
                        bersihkanBuffer();
                        if (subMisi == 0) {
                            system("cls");
                            break;
                        } else if (subMisi == 1) {
                            ambilMisiQueue(daftarMisi, &jumlahMisi, daftarKarakter, jumlahKarakter, userAktif, karakterAktif);
                            jedaLayar();
                        } else if (subMisi == 2) {
                            selesaikanMisiKeStack(daftarMisi, jumlahMisi, daftarKarakter, jumlahKarakter, userAktif, karakterAktif);
                            jedaLayar();
                        } else if (subMisi == 3) {
                            tampilkanQueueMisi(daftarMisi, jumlahMisi, userAktif, karakterAktif);
                            jedaLayar();
                        } else if (subMisi == 4) {
                            tampilkanStackRiwayat(daftarMisi, jumlahMisi, userAktif, karakterAktif);
                            jedaLayar();
                        } else {
                            printf("Pilihan tidak valid.\n");
                            jedaLayar();
                        }
                    }
                    break;
                }
                case 9: {
                    int subPeta = 0;
                    system("cls");
                    while (1) {
                        printf("\n--- NAVIGASI PETA DUNIA (GRAPH) ---\n");
                        printf("1. Lihat Peta & Wilayah Terhubung\n");
                        printf("2. Pindah ke Wilayah Tetangga\n");
                        printf("3. Cari Jalur Terpendek (Dijkstra)\n");
                        printf("0. Kembali ke Menu Utama\n");
                        printf("Pilih: ");
                        if (scanf("%d", &subPeta) != 1) {
                            printf("Input harus angka!\n");
                            bersihkanBuffer();
                            jedaLayar();
                            continue;
                        }
                        bersihkanBuffer();
                        if (subPeta == 0) {
                            system("cls");
                            break;
                        } else if (subPeta == 1) {
                            tampilkanPetaDanTetangga(daftarLokasi, &jumlahLokasi, daftarKarakter, jumlahKarakter, userAktif, karakterAktif);
                            jedaLayar();
                        } else if (subPeta == 2) {
                            pindahWilayahKarakter(daftarLokasi, &jumlahLokasi, daftarKarakter, jumlahKarakter, userAktif, karakterAktif);
                            jedaLayar();
                        } else if (subPeta == 3) {
                            cariJalurTerpendekPeta(daftarLokasi, &jumlahLokasi, daftarKarakter, jumlahKarakter, userAktif, karakterAktif);
                            jedaLayar();
                        } else {
                            printf("Pilihan tidak valid.\n");
                            jedaLayar();
                        }
                    }
                    break;
                }
                case 88:
                    sudahLogin = 0;
                    karakterAktif[0] = '\0';
                    printf("Anda telah logout.\n");
                    jedaLayar();
                    break;
                case 0:
                    system("cls");
                    printf("Keluar dari program.\n");
                    return 0;
                default:
                    printf("Pilihan tidak valid.\n");
                    jedaLayar();
                    break;
            }
        }
    }

    return 0;
}