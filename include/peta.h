#ifndef PETA_H
#define PETA_H

#include "auth.h"
#include "karakter.h"

#define JUMLAH_WILAYAH 6
#define MAX_NAMA_WILAYAH 35
#define LOKASI_FILE "data/lokasi.txt"

typedef struct {
    char pemilik[MAX_USER];
    char namaKarakter[MAX_NAMA_KAR];
    int idWilayah;
} CatatanLokasi;

int muatLokasiKarakter(CatatanLokasi daftar[], int maks);
int simpanLokasiKarakter(const CatatanLokasi daftar[], int jumlah);

int ambilWilayahKarakter(CatatanLokasi daftarLok[], int *jumlahLok,
                         const char *pemilik, const char *namaKarakter);

void tampilkanPetaDanTetangga(CatatanLokasi daftarLok[], int *jumlahLok,
                              const Karakter daftarKar[], int jumlahKar,
                              const char *pemilik, const char *namaKarakter);

int pindahWilayahKarakter(CatatanLokasi daftarLok[], int *jumlahLok,
                          const Karakter daftarKar[], int jumlahKar,
                          const char *pemilik, const char *namaKarakter);

void cariJalurTerpendekPeta(CatatanLokasi daftarLok[], int *jumlahLok,
                             const Karakter daftarKar[], int jumlahKar,
                             const char *pemilik, const char *namaKarakter);

#endif