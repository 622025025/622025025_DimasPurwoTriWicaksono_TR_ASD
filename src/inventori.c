#include <stdio.h>
#include <string.h>
#include "../include/inventori.h"

static const KatalogItem KATALOG_ITEM[] = {
    /* Item umum (Potion) */
    {"Semua",    "Minor_Potion",      "Potion", 25,  0, 15},
    {"Semua",    "Greater_Potion",    "Potion", 75,  0, 35},

    /* Assassin */
    {"Assassin", "Shadow_Dagger",     "Weapon", 140, 8,  0},
    {"Assassin", "Night_Cloak",       "Gear",   210, 6,  20},

    /* Marksman */
    {"Marksman", "Swift_Crossbow",    "Weapon", 150, 9,  0},
    {"Marksman", "Wind_Quiver",       "Relic",  220, 7,  15},

    /* Mage */
    {"Mage",     "Starlight_Staff",   "Weapon", 155, 10, 0},
    {"Mage",     "Mana_Grimoire",     "Relic",  225, 7,  20},

    /* Fighter */
    {"Fighter",  "Runed_Greatsword",  "Weapon", 160, 7,  25},
    {"Fighter",  "Titan_Gauntlets",   "Gear",   230, 5,  40},

    /* Tank */
    {"Tank",     "Aegis_TowerShield", "Gear",   165, 2,  55},
    {"Tank",     "Colossus_Plate",    "Armor",  240, 3,  80}
};

static const int JUMLAH_KATALOG = (int)(sizeof(KATALOG_ITEM) / sizeof(KATALOG_ITEM[0]));

static void bersihkanBufferInv(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
}

int muatInventori(ItemInventori daftar[], int maks) {
    FILE *file = fopen(INVENTORI_FILE, "r");
    if (file == NULL) {
        return 0;
    }

    int count = 0;
    while (count < maks &&
           fscanf(file, "%29s %29s %29s %19s %d %d",
                  daftar[count].pemilik,
                  daftar[count].namaKarakter,
                  daftar[count].namaItem,
                  daftar[count].tipe,
                  &daftar[count].jumlah,
                  &daftar[count].nilai) == 6) {
        count++;
    }

    fclose(file);
    return count;
}

int simpanInventori(const ItemInventori daftar[], int jumlah) {
    FILE *file = fopen(INVENTORI_FILE, "w");
    if (file == NULL) {
        printf("Gagal membuka file %s untuk menyimpan.\n", INVENTORI_FILE);
        return 0;
    }

    int i;
    for (i = 0; i < jumlah; i++) {
        fprintf(file, "%s %s %s %s %d %d\n",
                daftar[i].pemilik,
                daftar[i].namaKarakter,
                daftar[i].namaItem,
                daftar[i].tipe,
                daftar[i].jumlah,
                daftar[i].nilai);
    }

    fclose(file);
    return 1;
}

int beliItemKarakter(ItemInventori daftarInv[], int *jumlahInv,
                     Karakter daftarKar[], int jumlahKar,
                     const char *pemilik, const char *namaKarakter) {
    int idxKar = cariIndeksKarakterAktif(daftarKar, jumlahKar, pemilik, namaKarakter);
    if (idxKar == -1) {
        printf("Pilih atau buat karakter aktif terlebih dahulu sebelum membeli item!\n");
        return 0;
    }

    const char *roleKar = daftarKar[idxKar].role;
    int indeksTersedia[30];
    int jumlahTersedia = 0;
    int k;

    for (k = 0; k < JUMLAH_KATALOG && jumlahTersedia < 30; k++) {
        if (strcmp(KATALOG_ITEM[k].roleTarget, "Semua") == 0 ||
            strcmp(KATALOG_ITEM[k].roleTarget, roleKar) == 0) {
            indeksTersedia[jumlahTersedia++] = k;
        }
    }

    printf("\n--- KATALOG ITEM ROLE %s (Karakter: %s | Gold: %d) ---\n",
           roleKar, daftarKar[idxKar].nama, daftarKar[idxKar].gold);
    for (k = 0; k < jumlahTersedia; k++) {
        const KatalogItem *it = &KATALOG_ITEM[indeksTersedia[k]];
        printf("%d. %s [%s] - %d Gold (+%d ATK, +%d HP)\n",
               k + 1,
               it->nama,
               it->tipe,
               it->harga,
               it->bonusAtk,
               it->bonusHp);
    }

    int pilihan = 0;
    int qtyInput = 0;

    printf("Pilih nomor item (1-%d): ", jumlahTersedia);
    if (scanf("%d", &pilihan) != 1 || pilihan < 1 || pilihan > jumlahTersedia) {
        printf("Pilihan item tidak valid!\n");
        bersihkanBufferInv();
        return 0;
    }
    bersihkanBufferInv();

    printf("Jumlah item: ");
    if (scanf("%d", &qtyInput) != 1 || qtyInput <= 0) {
        printf("Jumlah item harus lebih dari 0!\n");
        bersihkanBufferInv();
        return 0;
    }
    bersihkanBufferInv();

    const KatalogItem dipilih = KATALOG_ITEM[indeksTersedia[pilihan - 1]];
    int totalBiaya = dipilih.harga * qtyInput;

    if (daftarKar[idxKar].gold < totalBiaya) {
        printf("Pembelian ditolak! Gold %s tidak cukup (Butuh: %d Gold, Tersedia: %d Gold).\n",
               daftarKar[idxKar].nama, totalBiaya, daftarKar[idxKar].gold);
        return 0;
    }

    int adaItem = 0;
    int i;
    for (i = 0; i < *jumlahInv; i++) {
        if (strcmp(daftarInv[i].pemilik, pemilik) == 0 &&
            strcmp(daftarInv[i].namaKarakter, daftarKar[idxKar].nama) == 0 &&
            strcmp(daftarInv[i].namaItem, dipilih.nama) == 0) {
            adaItem = 1;
            break;
        }
    }

    if (!adaItem && *jumlahInv >= MAX_ITEM) {
        printf("Inventori sudah penuh!\n");
        return 0;
    }

    daftarKar[idxKar].gold -= totalBiaya;
    daftarKar[idxKar].atk += dipilih.bonusAtk * qtyInput;
    daftarKar[idxKar].hp += dipilih.bonusHp * qtyInput;

    if (!simpanKarakter(daftarKar, jumlahKar)) {
        daftarKar[idxKar].gold += totalBiaya;
        daftarKar[idxKar].atk -= dipilih.bonusAtk * qtyInput;
        daftarKar[idxKar].hp -= dipilih.bonusHp * qtyInput;
        return 0;
    }

    if (adaItem) {
        daftarInv[i].jumlah += qtyInput;
        daftarInv[i].nilai = dipilih.harga;
        printf("Pembelian berhasil! Jumlah '%s' milik %s menjadi %d (Sisa Gold: %d | ATK: %d | HP: %d).\n",
               dipilih.nama, daftarKar[idxKar].nama, daftarInv[i].jumlah,
               daftarKar[idxKar].gold, daftarKar[idxKar].atk, daftarKar[idxKar].hp);
    } else {
        ItemInventori baru;
        strncpy(baru.pemilik, pemilik, MAX_USER - 1);
        baru.pemilik[MAX_USER - 1] = '\0';
        strncpy(baru.namaKarakter, daftarKar[idxKar].nama, MAX_NAMA_KAR - 1);
        baru.namaKarakter[MAX_NAMA_KAR - 1] = '\0';
        strncpy(baru.namaItem, dipilih.nama, MAX_NAMA_ITEM - 1);
        baru.namaItem[MAX_NAMA_ITEM - 1] = '\0';
        strncpy(baru.tipe, dipilih.tipe, MAX_TIPE_ITEM - 1);
        baru.tipe[MAX_TIPE_ITEM - 1] = '\0';
        baru.jumlah = qtyInput;
        baru.nilai = dipilih.harga;

        daftarInv[*jumlahInv] = baru;
        (*jumlahInv)++;
        printf("Pembelian berhasil! '%s' (x%d) masuk ke inventori %s (Sisa Gold: %d | ATK: %d | HP: %d).\n",
               baru.namaItem, baru.jumlah, daftarKar[idxKar].nama,
               daftarKar[idxKar].gold, daftarKar[idxKar].atk, daftarKar[idxKar].hp);
    }

    simpanInventori(daftarInv, *jumlahInv);
    return 1;
}

void tampilkanInventoriKarakter(const ItemInventori daftar[], int jumlah,
                                  const char *pemilik, const char *namaKarakter) {
    if (namaKarakter == NULL || namaKarakter[0] == '\0') {
        printf("Belum ada karakter aktif yang dipilih.\n");
        return;
    }

    int i;
    int ditemukan = 0;

    printf("\n--- INVENTORI KARAKTER '%s' (Akun: %s) ---\n", namaKarakter, pemilik);
    for (i = 0; i < jumlah; i++) {
        if (strcmp(daftar[i].pemilik, pemilik) == 0 &&
            strcmp(daftar[i].namaKarakter, namaKarakter) == 0) {
            ditemukan++;
            printf("%d. Item: %s | Tipe: %s | Jumlah: %d | Harga Satuan: %d Gold\n",
                   ditemukan,
                   daftar[i].namaItem,
                   daftar[i].tipe,
                   daftar[i].jumlah,
                   daftar[i].nilai);
        }
    }

    if (ditemukan == 0) {
        printf("Inventori karakter '%s' masih kosong.\n", namaKarakter);
    }
}

void urutkanInventoriKarakter(const ItemInventori daftar[], int jumlah,
                                const char *pemilik, const char *namaKarakter, int mode) {
    if (namaKarakter == NULL || namaKarakter[0] == '\0') {
        printf("Belum ada karakter aktif yang dipilih.\n");
        return;
    }

    ItemInventori salinan[MAX_ITEM];
    int n = 0;
    int i, j;

    for (i = 0; i < jumlah; i++) {
        if (strcmp(daftar[i].pemilik, pemilik) == 0 &&
            strcmp(daftar[i].namaKarakter, namaKarakter) == 0 && n < MAX_ITEM) {
            salinan[n++] = daftar[i];
        }
    }

    if (n == 0) {
        printf("Inventori '%s' masih kosong, tidak ada yang diurutkan.\n", namaKarakter);
        return;
    }

    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            int tukar = 0;
            if (mode == 1) {
                if (salinan[j].nilai < salinan[j + 1].nilai) {
                    tukar = 1;
                }
            } else if (mode == 2) {
                if (salinan[j].jumlah < salinan[j + 1].jumlah) {
                    tukar = 1;
                }
            } else {
                if (strcmp(salinan[j].namaItem, salinan[j + 1].namaItem) > 0) {
                    tukar = 1;
                }
            }

            if (tukar) {
                ItemInventori temp = salinan[j];
                salinan[j] = salinan[j + 1];
                salinan[j + 1] = temp;
            }
        }
    }

    printf("\n--- INVENTORI TERURUT ('%s') ---\n", namaKarakter);
    for (i = 0; i < n; i++) {
        printf("%d. Item: %s | Tipe: %s | Jumlah: %d | Harga Satuan: %d Gold\n",
               i + 1,
               salinan[i].namaItem,
               salinan[i].tipe,
               salinan[i].jumlah,
               salinan[i].nilai);
    }
}

void cariInventoriKarakter(const ItemInventori daftar[], int jumlah,
                             const char *pemilik, const char *namaKarakter) {
    if (namaKarakter == NULL || namaKarakter[0] == '\0') {
        printf("Belum ada karakter aktif yang dipilih.\n");
        return;
    }

    char kataKunci[MAX_NAMA_ITEM];
    int i;
    int ketemu = 0;

    printf("Masukkan nama item atau tipe yang dicari (tanpa spasi): ");
    scanf("%29s", kataKunci);
    bersihkanBufferInv();

    printf("\n--- HASIL PENCARIAN INVENTORI '%s' ('%s') ---\n", namaKarakter, kataKunci);
    for (i = 0; i < jumlah; i++) {
        if (strcmp(daftar[i].pemilik, pemilik) == 0 &&
            strcmp(daftar[i].namaKarakter, namaKarakter) == 0 &&
            (strcmp(daftar[i].namaItem, kataKunci) == 0 ||
             strcmp(daftar[i].tipe, kataKunci) == 0)) {
            ketemu++;
            printf("%d. Item: %s | Tipe: %s | Jumlah: %d | Harga Satuan: %d Gold\n",
                   ketemu,
                   daftar[i].namaItem,
                   daftar[i].tipe,
                   daftar[i].jumlah,
                   daftar[i].nilai);
        }
    }

    if (ketemu == 0) {
        printf("Tidak ada item yang cocok dengan '%s' pada karakter '%s'.\n", kataKunci, namaKarakter);
    }
}