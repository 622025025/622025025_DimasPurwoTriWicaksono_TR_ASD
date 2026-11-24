#include <stdio.h>
#include <string.h>
#include "../include/misi.h"

static const KatalogMisi KATALOG_MISI[] = {
    {1, "Patroli_Gerbang_Utara", 60, 2, 10},
    {2, "Basmi_Slime_Rawa", 90, 3, 15},
    {3, "Kawal_Kafilah_Pegunungan", 130, 5, 20},
    {4, "Selidiki_Reruntuhan_Kuno", 180, 7, 30},
    {5, "Tumbangkan_Penjaga_Naga", 260, 10, 45}
};

static const int JUMLAH_KATALOG_MISI = (int)(sizeof(KATALOG_MISI) / sizeof(KATALOG_MISI[0]));

static void bersihkanBufferMisi(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
}

int muatMisi(CatatanMisi daftar[], int maks) {
    FILE *file = fopen(MISI_FILE, "r");
    if (file == NULL) {
        return 0;
    }

    int count = 0;
    while (count < maks &&
           fscanf(file, "%29s %29s %d %39s %d %d %d %d",
                  daftar[count].pemilik,
                  daftar[count].namaKarakter,
                  &daftar[count].idMisi,
                  daftar[count].judul,
                  &daftar[count].rewardGold,
                  &daftar[count].bonusAtk,
                  &daftar[count].bonusHp,
                  &daftar[count].status) == 8) {
        count++;
    }

    fclose(file);
    return count;
}

int simpanMisi(const CatatanMisi daftar[], int jumlah) {
    FILE *file = fopen(MISI_FILE, "w");
    if (file == NULL) {
        printf("Gagal membuka file %s untuk menyimpan.\n", MISI_FILE);
        return 0;
    }

    int i;
    for (i = 0; i < jumlah; i++) {
        fprintf(file, "%s %s %d %s %d %d %d %d\n",
                daftar[i].pemilik,
                daftar[i].namaKarakter,
                daftar[i].idMisi,
                daftar[i].judul,
                daftar[i].rewardGold,
                daftar[i].bonusAtk,
                daftar[i].bonusHp,
                daftar[i].status);
    }

    fclose(file);
    return 1;
}

int ambilMisiQueue(CatatanMisi daftarMisi[], int *jumlahMisi,
                   const Karakter daftarKar[], int jumlahKar,
                   const char *pemilik, const char *namaKarakter) {
    int idxKar = cariIndeksKarakterAktif(daftarKar, jumlahKar, pemilik, namaKarakter);
    if (idxKar == -1) {
        printf("Pilih atau buat karakter aktif terlebih dahulu!\n");
        return 0;
    }

    if (*jumlahMisi >= MAX_MISI) {
        printf("Kapasitas penyimpanan misi penuh!\n");
        return 0;
    }

    int pilihan = 0;
    int i;
    printf("\n--- DAFTAR MISI TERSEDIA (Karakter: %s) ---\n", daftarKar[idxKar].nama);
    for (i = 0; i < JUMLAH_KATALOG_MISI; i++) {
        printf("%d. %s | Reward: +%d Gold, +%d ATK, +%d HP\n",
               i + 1,
               KATALOG_MISI[i].judul,
               KATALOG_MISI[i].rewardGold,
               KATALOG_MISI[i].bonusAtk,
               KATALOG_MISI[i].bonusHp);
    }

    printf("Pilih nomor misi (1-%d): ", JUMLAH_KATALOG_MISI);
    if (scanf("%d", &pilihan) != 1 || pilihan < 1 || pilihan > JUMLAH_KATALOG_MISI) {
        printf("Pilihan misi tidak valid!\n");
        bersihkanBufferMisi();
        return 0;
    }
    bersihkanBufferMisi();

    const KatalogMisi dipilih = KATALOG_MISI[pilihan - 1];

    /* Cegah misi yang sama masuk duplikat ke antrean aktif karakter yang sama */
    for (i = 0; i < *jumlahMisi; i++) {
        if (strcmp(daftarMisi[i].pemilik, pemilik) == 0 &&
            strcmp(daftarMisi[i].namaKarakter, daftarKar[idxKar].nama) == 0 &&
            daftarMisi[i].idMisi == dipilih.id &&
            daftarMisi[i].status == 1) {
            printf("Misi '%s' sudah ada di antrean aktif %s!\n", dipilih.judul, daftarKar[idxKar].nama);
            return 0;
        }
    }

    CatatanMisi baru;
    strncpy(baru.pemilik, pemilik, MAX_USER - 1);
    baru.pemilik[MAX_USER - 1] = '\0';
    strncpy(baru.namaKarakter, daftarKar[idxKar].nama, MAX_NAMA_KAR - 1);
    baru.namaKarakter[MAX_NAMA_KAR - 1] = '\0';
    baru.idMisi = dipilih.id;
    strncpy(baru.judul, dipilih.judul, MAX_JUDUL_MISI - 1);
    baru.judul[MAX_JUDUL_MISI - 1] = '\0';
    baru.rewardGold = dipilih.rewardGold;
    baru.bonusAtk = dipilih.bonusAtk;
    baru.bonusHp = dipilih.bonusHp;
    baru.status = 1; /* Enqueue ke antrean aktif */

    daftarMisi[*jumlahMisi] = baru;
    (*jumlahMisi)++;

    if (!simpanMisi(daftarMisi, *jumlahMisi)) {
        return 0;
    }

    printf("Misi '%s' masuk ke antrean (Queue) misi aktif %s!\n", baru.judul, daftarKar[idxKar].nama);
    return 1;
}

int selesaikanMisiKeStack(CatatanMisi daftarMisi[], int jumlahMisi,
                           Karakter daftarKar[], int jumlahKar,
                           const char *pemilik, const char *namaKarakter) {
    int idxKar = cariIndeksKarakterAktif(daftarKar, jumlahKar, pemilik, namaKarakter);
    if (idxKar == -1) {
        printf("Pilih atau buat karakter aktif terlebih dahulu!\n");
        return 0;
    }

    /* Dequeue: cari misi aktif (status == 1) paling awal milik karakter aktif */
    int idxAntreanPertama = -1;
    int i;
    for (i = 0; i < jumlahMisi; i++) {
        if (strcmp(daftarMisi[i].pemilik, pemilik) == 0 &&
            strcmp(daftarMisi[i].namaKarakter, daftarKar[idxKar].nama) == 0 &&
            daftarMisi[i].status == 1) {
            idxAntreanPertama = i;
            break;
        }
    }

    if (idxAntreanPertama == -1) {
        printf("Tidak ada misi aktif di antrean (Queue) untuk %s.\n", daftarKar[idxKar].nama);
        return 0;
    }

    /* Pindahkan misi dari Queue (status 1) ke Stack Riwayat (status 2) */
    daftarMisi[idxAntreanPertama].status = 2;

    daftarKar[idxKar].gold += daftarMisi[idxAntreanPertama].rewardGold;
    daftarKar[idxKar].atk += daftarMisi[idxAntreanPertama].bonusAtk;
    daftarKar[idxKar].hp += daftarMisi[idxAntreanPertama].bonusHp;
    daftarKar[idxKar].level += 1;

    simpanMisi(daftarMisi, jumlahMisi);
    simpanKarakter(daftarKar, jumlahKar);

    printf("\nMisi '%s' selesai (Dequeue -> Push ke Stack Riwayat)!\n", daftarMisi[idxAntreanPertama].judul);
    printf("Reward diterima %s: +%d Gold, +%d ATK, +%d HP, Level menjadi %d (Total Gold: %d).\n",
           daftarKar[idxKar].nama,
           daftarMisi[idxAntreanPertama].rewardGold,
           daftarMisi[idxAntreanPertama].bonusAtk,
           daftarMisi[idxAntreanPertama].bonusHp,
           daftarKar[idxKar].level,
           daftarKar[idxKar].gold);

    return 1;
}

void tampilkanQueueMisi(const CatatanMisi daftarMisi[], int jumlahMisi,
                        const char *pemilik, const char *namaKarakter) {
    if (namaKarakter == NULL || namaKarakter[0] == '\0') {
        printf("Belum ada karakter aktif yang dipilih.\n");
        return;
    }

    int i;
    int urutan = 0;
    printf("\n--- ANTREAN MISI AKTIF / QUEUE (Karakter: %s) ---\n", namaKarakter);
    for (i = 0; i < jumlahMisi; i++) {
        if (strcmp(daftarMisi[i].pemilik, pemilik) == 0 &&
            strcmp(daftarMisi[i].namaKarakter, namaKarakter) == 0 &&
            daftarMisi[i].status == 1) {
            urutan++;
            printf("%d. [DEPAN=%s] %s | Reward: +%d Gold, +%d ATK, +%d HP\n",
                   urutan,
                   urutan == 1 ? "YA" : "TIDAK",
                   daftarMisi[i].judul,
                   daftarMisi[i].rewardGold,
                   daftarMisi[i].bonusAtk,
                   daftarMisi[i].bonusHp);
        }
    }

    if (urutan == 0) {
        printf("Antrean misi kosong.\n");
    }
}

void tampilkanStackRiwayat(const CatatanMisi daftarMisi[], int jumlahMisi,
                           const char *pemilik, const char *namaKarakter) {
    if (namaKarakter == NULL || namaKarakter[0] == '\0') {
        printf("Belum ada karakter aktif yang dipilih.\n");
        return;
    }

    /* Tampilkan dari elemen terakhir yang masuk ke Stack (LIFO: TOP -> bawah) */
    int i;
    int urutan = 0;
    printf("\n--- RIWAYAT MISI SELESAI / STACK LIFO (Karakter: %s) ---\n", namaKarakter);
    for (i = jumlahMisi - 1; i >= 0; i--) {
        if (strcmp(daftarMisi[i].pemilik, pemilik) == 0 &&
            strcmp(daftarMisi[i].namaKarakter, namaKarakter) == 0 &&
            daftarMisi[i].status == 2) {
            urutan++;
            printf("%d. [TOP=%s] %s | Reward: +%d Gold, +%d ATK, +%d HP\n",
                   urutan,
                   urutan == 1 ? "YA" : "TIDAK",
                   daftarMisi[i].judul,
                   daftarMisi[i].rewardGold,
                   daftarMisi[i].bonusAtk,
                   daftarMisi[i].bonusHp);
        }
    }

    if (urutan == 0) {
        printf("Belum ada misi yang diselesaikan.\n");
    }
}