#include <stdio.h>
#include <string.h>
#include "../include/karakter.h"

static void bersihkanBufferKar(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
}

static int pilihRole(char *tujuan, size_t ukuran) {
    const char *daftarRole[5] = {
        "Assassin",
        "Marksman",
        "Mage",
        "Fighter",
        "Tank"
    };
    int pilihan = 0;

    while (1) {
        printf("Pilih Role:\n");
        printf("1. Assassin\n");
        printf("2. Marksman\n");
        printf("3. Mage\n");
        printf("4. Fighter\n");
        printf("5. Tank\n");
        printf("Masukkan nomor role (1-5): ");

        if (scanf("%d", &pilihan) != 1) {
            printf("Input harus berupa angka 1 sampai 5!\n");
            bersihkanBufferKar();
            continue;
        }
        bersihkanBufferKar();

        if (pilihan >= 1 && pilihan <= 5) {
            strncpy(tujuan, daftarRole[pilihan - 1], ukuran - 1);
            tujuan[ukuran - 1] = '\0';
            return 1;
        }

        printf("Pilihan tidak valid! Silakan pilih angka 1 sampai 5.\n");
    }
}

int cariIndeksKarakterAktif(const Karakter daftar[], int jumlah, const char *pemilik, const char *namaKar) {
    int i;
    if (namaKar == NULL || namaKar[0] == '\0') {
        return -1;
    }

    for (i = 0; i < jumlah; i++) {
        if (strcmp(daftar[i].pemilik, pemilik) == 0 &&
            strcmp(daftar[i].nama, namaKar) == 0) {
            return i;
        }
    }
    return -1;
}

int muatKarakter(Karakter daftar[], int maks) {
    FILE *file = fopen(KARAKTER_FILE, "r");
    if (file == NULL) {
        return 0;
    }

    int count = 0;
    while (count < maks &&
           fscanf(file, "%29s %29s %19s %d %d %d %d",
                  daftar[count].pemilik,
                  daftar[count].nama,
                  daftar[count].role,
                  &daftar[count].level,
                  &daftar[count].hp,
                  &daftar[count].atk,
                  &daftar[count].gold) == 7) {
        count++;
    }

    fclose(file);
    return count;
}

int simpanKarakter(const Karakter daftar[], int jumlah) {
    FILE *file = fopen(KARAKTER_FILE, "w");
    if (file == NULL) {
        printf("Gagal membuka file %s untuk menyimpan.\n", KARAKTER_FILE);
        return 0;
    }

    int i;
    for (i = 0; i < jumlah; i++) {
        fprintf(file, "%s %s %s %d %d %d %d\n",
                daftar[i].pemilik,
                daftar[i].nama,
                daftar[i].role,
                daftar[i].level,
                daftar[i].hp,
                daftar[i].atk,
                daftar[i].gold);
    }

    fclose(file);
    return 1;
}

int tambahKarakter(Karakter daftar[], int *jumlah, const char *pemilik, char *karakterAktif) {
    if (*jumlah >= MAX_KARAKTER) {
        printf("Kapasitas penyimpanan karakter penuh!\n");
        return 0;
    }

    Karakter baru;
    strncpy(baru.pemilik, pemilik, MAX_USER - 1);
    baru.pemilik[MAX_USER - 1] = '\0';

    printf("Nama karakter (tanpa spasi): ");
    scanf("%29s", baru.nama);
    bersihkanBufferKar();

    if (cariIndeksKarakterAktif(daftar, *jumlah, pemilik, baru.nama) != -1) {
        printf("Karakter bernama '%s' sudah ada di akun ini!\n", baru.nama);
        return 0;
    }

    pilihRole(baru.role, sizeof(baru.role));

    baru.level = 1;
    baru.hp = 120;
    baru.atk = 25;
    baru.gold = 100;

    daftar[*jumlah] = baru;
    (*jumlah)++;

    if (!simpanKarakter(daftar, *jumlah)) {
        return 0;
    }

    if (karakterAktif != NULL) {
        strncpy(karakterAktif, baru.nama, MAX_NAMA_KAR - 1);
        karakterAktif[MAX_NAMA_KAR - 1] = '\0';
    }

    printf("Karakter '%s' (%s) berhasil dibuat dan dijadikan karakter aktif!\n", baru.nama, baru.role);
    return 1;
}

void tampilkanKarakterUser(const Karakter daftar[], int jumlah, const char *pemilik, const char *karakterAktif) {
    int i;
    int ditemukan = 0;

    printf("\n--- DAFTAR KARAKTER MILIK %s ---\n", pemilik);
    for (i = 0; i < jumlah; i++) {
        if (strcmp(daftar[i].pemilik, pemilik) == 0) {
            ditemukan++;
            int aktif = (karakterAktif != NULL && strcmp(daftar[i].nama, karakterAktif) == 0);
            printf("%d. Nama: %s%s | Role: %s | Lv: %d | HP: %d | ATK: %d | Gold: %d\n",
                   ditemukan,
                   daftar[i].nama,
                   aktif ? " [AKTIF]" : "",
                   daftar[i].role,
                   daftar[i].level,
                   daftar[i].hp,
                   daftar[i].atk,
                   daftar[i].gold);
        }
    }

    if (ditemukan == 0) {
        printf("Belum ada karakter yang dibuat untuk akun ini.\n");
    }
}

int pilihKarakterAktif(const Karakter daftar[], int jumlah, const char *pemilik, char *karakterAktif) {
    int indeksMilikUser[MAX_KARAKTER];
    int totalMilik = 0;
    int i;

    for (i = 0; i < jumlah; i++) {
        if (strcmp(daftar[i].pemilik, pemilik) == 0) {
            indeksMilikUser[totalMilik++] = i;
        }
    }

    if (totalMilik == 0) {
        printf("Belum ada karakter untuk dipilih. Buat karakter terlebih dahulu!\n");
        return 0;
    }

    printf("\n--- PILIH KARAKTER AKTIF (%s) ---\n", pemilik);
    for (i = 0; i < totalMilik; i++) {
        int idx = indeksMilikUser[i];
        printf("%d. %s (%s) | Lv: %d | Gold: %d\n",
               i + 1,
               daftar[idx].nama,
               daftar[idx].role,
               daftar[idx].level,
               daftar[idx].gold);
    }

    int pilihan = 0;
    printf("Pilih nomor karakter (1-%d): ", totalMilik);
    if (scanf("%d", &pilihan) != 1 || pilihan < 1 || pilihan > totalMilik) {
        printf("Pilihan tidak valid!\n");
        bersihkanBufferKar();
        return 0;
    }
    bersihkanBufferKar();

    int terpilih = indeksMilikUser[pilihan - 1];
    strncpy(karakterAktif, daftar[terpilih].nama, MAX_NAMA_KAR - 1);
    karakterAktif[MAX_NAMA_KAR - 1] = '\0';

    printf("Karakter aktif sekarang: %s (Gold: %d)\n", daftar[terpilih].nama, daftar[terpilih].gold);
    return 1;
}

void urutkanKarakterUser(const Karakter daftar[], int jumlah, const char *pemilik, int mode) {
    Karakter salinan[MAX_KARAKTER];
    int n = 0;
    int i, j;

    for (i = 0; i < jumlah; i++) {
        if (strcmp(daftar[i].pemilik, pemilik) == 0 && n < MAX_KARAKTER) {
            salinan[n++] = daftar[i];
        }
    }

    if (n == 0) {
        printf("Belum ada karakter untuk diurutkan.\n");
        return;
    }

    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            int tukar = 0;
            if (mode == 1) {
                if (salinan[j].level < salinan[j + 1].level) {
                    tukar = 1;
                }
            } else if (mode == 2) {
                if (salinan[j].gold < salinan[j + 1].gold) {
                    tukar = 1;
                }
            } else {
                if (strcmp(salinan[j].nama, salinan[j + 1].nama) > 0) {
                    tukar = 1;
                }
            }

            if (tukar) {
                Karakter temp = salinan[j];
                salinan[j] = salinan[j + 1];
                salinan[j + 1] = temp;
            }
        }
    }

    printf("\n--- DAFTAR KARAKTER TERURUT (%s) ---\n", pemilik);
    for (i = 0; i < n; i++) {
        printf("%d. Nama: %s | Role: %s | Lv: %d | HP: %d | ATK: %d | Gold: %d\n",
               i + 1,
               salinan[i].nama,
               salinan[i].role,
               salinan[i].level,
               salinan[i].hp,
               salinan[i].atk,
               salinan[i].gold);
    }
}

void cariKarakterUser(const Karakter daftar[], int jumlah, const char *pemilik) {
    char kunci[MAX_NAMA_KAR];
    int i;
    int ketemu = 0;

    printf("Masukkan nama atau role karakter yang dicari (tanpa spasi): ");
    scanf("%29s", kunci);
    bersihkanBufferKar();

    printf("\n--- HASIL PENCARIAN KARAKTER ('%s') ---\n", kunci);
    for (i = 0; i < jumlah; i++) {
        if (strcmp(daftar[i].pemilik, pemilik) == 0 &&
            (strcmp(daftar[i].nama, kunci) == 0 ||
             strcmp(daftar[i].role, kunci) == 0)) {
            ketemu++;
            printf("%d. Nama: %s | Role: %s | Lv: %d | HP: %d | ATK: %d | Gold: %d\n",
                   ketemu,
                   daftar[i].nama,
                   daftar[i].role,
                   daftar[i].level,
                   daftar[i].hp,
                   daftar[i].atk,
                   daftar[i].gold);
        }
    }

    if (ketemu == 0) {
        printf("Tidak ada karakter yang cocok dengan '%s'.\n", kunci);
    }
}
