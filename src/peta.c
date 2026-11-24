#include <stdio.h>
#include <string.h>
#include "../include/peta.h"

#define INF_JARAK 999999

static const char *NAMA_WILAYAH[JUMLAH_WILAYAH] = {
    "Gerbang_Utara",
    "Hutan_Kabut",
    "Rawa_Slime",
    "Puncak_Kafilah",
    "Reruntuhan_Kuno",
    "Sarang_Naga"
};

/* Matriks ketetanggaan (0 berarti tidak terhubung langsung) */
static const int GRAF_PETA[JUMLAH_WILAYAH][JUMLAH_WILAYAH] = {
    /* 0: Gerbang_Utara */ {0,  4,  6,  0,  0,  0},
    /* 1: Hutan_Kabut   */ {4,  0,  3,  7,  0,  0},
    /* 2: Rawa_Slime    */ {6,  3,  0,  0,  8,  0},
    /* 3: Puncak_Kafilah*/ {0,  7,  0,  0,  5,  9},
    /* 4: Reruntuhan    */ {0,  0,  8,  5,  0,  6},
    /* 5: Sarang_Naga   */ {0,  0,  0,  9,  6,  0}
};

static void bersihkanBufferPeta(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
}

int muatLokasiKarakter(CatatanLokasi daftar[], int maks) {
    FILE *file = fopen(LOKASI_FILE, "r");
    if (file == NULL) {
        return 0;
    }

    int count = 0;
    while (count < maks &&
           fscanf(file, "%29s %29s %d",
                  daftar[count].pemilik,
                  daftar[count].namaKarakter,
                  &daftar[count].idWilayah) == 3) {
        if (daftar[count].idWilayah < 0 || daftar[count].idWilayah >= JUMLAH_WILAYAH) {
            daftar[count].idWilayah = 0;
        }
        count++;
    }

    fclose(file);
    return count;
}

int simpanLokasiKarakter(const CatatanLokasi daftar[], int jumlah) {
    FILE *file = fopen(LOKASI_FILE, "w");
    if (file == NULL) {
        printf("Gagal membuka file %s untuk menyimpan.\n", LOKASI_FILE);
        return 0;
    }

    int i;
    for (i = 0; i < jumlah; i++) {
        fprintf(file, "%s %s %d\n",
                daftar[i].pemilik,
                daftar[i].namaKarakter,
                daftar[i].idWilayah);
    }

    fclose(file);
    return 1;
}

static int cariIndeksLokasi(const CatatanLokasi daftar[], int jumlah,
                            const char *pemilik, const char *namaKarakter) {
    int i;
    for (i = 0; i < jumlah; i++) {
        if (strcmp(daftar[i].pemilik, pemilik) == 0 &&
            strcmp(daftar[i].namaKarakter, namaKarakter) == 0) {
            return i;
        }
    }
    return -1;
}

int ambilWilayahKarakter(CatatanLokasi daftarLok[], int *jumlahLok,
                         const char *pemilik, const char *namaKarakter) {
    int idx = cariIndeksLokasi(daftarLok, *jumlahLok, pemilik, namaKarakter);
    if (idx != -1) {
        return daftarLok[idx].idWilayah;
    }

    if (*jumlahLok < 1000) {
        CatatanLokasi baru;
        strncpy(baru.pemilik, pemilik, MAX_USER - 1);
        baru.pemilik[MAX_USER - 1] = '\0';
        strncpy(baru.namaKarakter, namaKarakter, MAX_NAMA_KAR - 1);
        baru.namaKarakter[MAX_NAMA_KAR - 1] = '\0';
        baru.idWilayah = 0; /* Mulai dari Gerbang_Utara */

        daftarLok[*jumlahLok] = baru;
        (*jumlahLok)++;
        simpanLokasiKarakter(daftarLok, *jumlahLok);
    }

    return 0;
}

static void setWilayahKarakter(CatatanLokasi daftarLok[], int *jumlahLok,
                               const char *pemilik, const char *namaKarakter, int idBaru) {
    int idx = cariIndeksLokasi(daftarLok, *jumlahLok, pemilik, namaKarakter);
    if (idx != -1) {
        daftarLok[idx].idWilayah = idBaru;
    } else if (*jumlahLok < 1000) {
        CatatanLokasi baru;
        strncpy(baru.pemilik, pemilik, MAX_USER - 1);
        baru.pemilik[MAX_USER - 1] = '\0';
        strncpy(baru.namaKarakter, namaKarakter, MAX_NAMA_KAR - 1);
        baru.namaKarakter[MAX_NAMA_KAR - 1] = '\0';
        baru.idWilayah = idBaru;
        daftarLok[*jumlahLok] = baru;
        (*jumlahLok)++;
    }
    simpanLokasiKarakter(daftarLok, *jumlahLok);
}

void tampilkanPetaDanTetangga(CatatanLokasi daftarLok[], int *jumlahLok,
                              const Karakter daftarKar[], int jumlahKar,
                              const char *pemilik, const char *namaKarakter) {
    int idxKar = cariIndeksKarakterAktif(daftarKar, jumlahKar, pemilik, namaKarakter);
    if (idxKar == -1) {
        printf("Pilih atau buat karakter aktif terlebih dahulu!\n");
        return;
    }

    int posisi = ambilWilayahKarakter(daftarLok, jumlahLok, pemilik, daftarKar[idxKar].nama);
    int i, j;

    printf("\n--- PETA DUNIA FANTASI (Karakter: %s) ---\n", daftarKar[idxKar].nama);
    printf("Lokasi saat ini: %s (ID %d)\n\n", NAMA_WILAYAH[posisi], posisi + 1);

    printf("Daftar Wilayah & Jarak Terhubung:\n");
    for (i = 0; i < JUMLAH_WILAYAH; i++) {
        printf("%d. %s%s -> Terhubung ke: ",
               i + 1,
               NAMA_WILAYAH[i],
               i == posisi ? " [POSISI ANDA]" : "");
        int adaTetangga = 0;
        for (j = 0; j < JUMLAH_WILAYAH; j++) {
            if (GRAF_PETA[i][j] > 0) {
                if (adaTetangga) {
                    printf(", ");
                }
                printf("%s (jarak %d)", NAMA_WILAYAH[j], GRAF_PETA[i][j]);
                adaTetangga = 1;
            }
        }
        if (!adaTetangga) {
            printf("-");
        }
        printf("\n");
    }
}

int pindahWilayahKarakter(CatatanLokasi daftarLok[], int *jumlahLok,
                          const Karakter daftarKar[], int jumlahKar,
                          const char *pemilik, const char *namaKarakter) {
    int idxKar = cariIndeksKarakterAktif(daftarKar, jumlahKar, pemilik, namaKarakter);
    if (idxKar == -1) {
        printf("Pilih atau buat karakter aktif terlebih dahulu!\n");
        return 0;
    }

    int posisi = ambilWilayahKarakter(daftarLok, jumlahLok, pemilik, daftarKar[idxKar].nama);
    int tujuan = 0;

    printf("\n--- PERPINDAHAN WILAYAH (Dari: %s) ---\n", NAMA_WILAYAH[posisi]);
    int i;
    for (i = 0; i < JUMLAH_WILAYAH; i++) {
        if (GRAF_PETA[posisi][i] > 0) {
            printf("ID %d. %s (Jarak: %d)\n", i + 1, NAMA_WILAYAH[i], GRAF_PETA[posisi][i]);
        }
    }

    printf("Masukkan ID wilayah tujuan (1-%d): ", JUMLAH_WILAYAH);
    if (scanf("%d", &tujuan) != 1 || tujuan < 1 || tujuan > JUMLAH_WILAYAH) {
        printf("ID wilayah tidak valid!\n");
        bersihkanBufferPeta();
        return 0;
    }
    bersihkanBufferPeta();

    int idxTujuan = tujuan - 1;
    if (idxTujuan == posisi) {
        printf("Karakter sudah berada di %s.\n", NAMA_WILAYAH[posisi]);
        return 0;
    }

    if (GRAF_PETA[posisi][idxTujuan] == 0) {
        printf("Tidak ada jalur langsung dari %s ke %s! Gunakan menu Jalur Terpendek.\n",
               NAMA_WILAYAH[posisi], NAMA_WILAYAH[idxTujuan]);
        return 0;
    }

    setWilayahKarakter(daftarLok, jumlahLok, pemilik, daftarKar[idxKar].nama, idxTujuan);
    printf("%s berhasil berpindah dari %s ke %s (Jarak tempuh: %d).\n",
           daftarKar[idxKar].nama,
           NAMA_WILAYAH[posisi],
           NAMA_WILAYAH[idxTujuan],
           GRAF_PETA[posisi][idxTujuan]);
    return 1;
}

void cariJalurTerpendekPeta(CatatanLokasi daftarLok[], int *jumlahLok,
                             const Karakter daftarKar[], int jumlahKar,
                             const char *pemilik, const char *namaKarakter) {
    int idxKar = cariIndeksKarakterAktif(daftarKar, jumlahKar, pemilik, namaKarakter);
    if (idxKar == -1) {
        printf("Pilih atau buat karakter aktif terlebih dahulu!\n");
        return;
    }

    int asal = ambilWilayahKarakter(daftarLok, jumlahLok, pemilik, daftarKar[idxKar].nama);
    int tujuanInput = 0;

    printf("\n--- JALUR TERPENDEK / DIJKSTRA (Asal: %s) ---\n", NAMA_WILAYAH[asal]);
    int i;
    for (i = 0; i < JUMLAH_WILAYAH; i++) {
        printf("ID %d. %s\n", i + 1, NAMA_WILAYAH[i]);
    }

    printf("Pilih ID wilayah tujuan (1-%d): ", JUMLAH_WILAYAH);
    if (scanf("%d", &tujuanInput) != 1 || tujuanInput < 1 || tujuanInput > JUMLAH_WILAYAH) {
        printf("ID wilayah tidak valid!\n");
        bersihkanBufferPeta();
        return;
    }
    bersihkanBufferPeta();

    int tujuan = tujuanInput - 1;
    int jarak[JUMLAH_WILAYAH];
    int dikunjungi[JUMLAH_WILAYAH];
    int sebelumnya[JUMLAH_WILAYAH];

    for (i = 0; i < JUMLAH_WILAYAH; i++) {
        jarak[i] = INF_JARAK;
        dikunjungi[i] = 0;
        sebelumnya[i] = -1;
    }
    jarak[asal] = 0;

    int langkah, v;
    for (langkah = 0; langkah < JUMLAH_WILAYAH; langkah++) {
        int u = -1;
        int minJarak = INF_JARAK;
        for (v = 0; v < JUMLAH_WILAYAH; v++) {
            if (!dikunjungi[v] && jarak[v] < minJarak) {
                minJarak = jarak[v];
                u = v;
            }
        }

        if (u == -1 || u == tujuan) {
            break;
        }
        dikunjungi[u] = 1;

        for (v = 0; v < JUMLAH_WILAYAH; v++) {
            if (!dikunjungi[v] && GRAF_PETA[u][v] > 0) {
                if (jarak[u] + GRAF_PETA[u][v] < jarak[v]) {
                    jarak[v] = jarak[u] + GRAF_PETA[u][v];
                    sebelumnya[v] = u;
                }
            }
        }
    }

    if (jarak[tujuan] >= INF_JARAK) {
        printf("Tidak ditemukan jalur dari %s ke %s.\n", NAMA_WILAYAH[asal], NAMA_WILAYAH[tujuan]);
        return;
    }

    int jalur[JUMLAH_WILAYAH];
    int panjangJalur = 0;
    int curr = tujuan;
    while (curr != -1 && panjangJalur < JUMLAH_WILAYAH) {
        jalur[panjangJalur++] = curr;
        curr = sebelumnya[curr];
    }

    printf("Total jarak terpendek ke %s: %d\n", NAMA_WILAYAH[tujuan], jarak[tujuan]);
    printf("Rute: ");
    for (i = panjangJalur - 1; i >= 0; i--) {
        printf("%s", NAMA_WILAYAH[jalur[i]]);
        if (i > 0) {
            printf(" -> ");
        }
    }
    printf("\n");
}