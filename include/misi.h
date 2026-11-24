#ifndef MISI_H
#define MISI_H

#include "auth.h"
#include "karakter.h"

#define MAX_MISI 100
#define MAX_JUDUL_MISI 40
#define MISI_FILE "data/misi.txt"

typedef struct {
    int id;
    char judul[MAX_JUDUL_MISI];
    int rewardGold;
    int bonusAtk;
    int bonusHp;
} KatalogMisi;

typedef struct {
    char pemilik[MAX_USER];
    char namaKarakter[MAX_NAMA_KAR];
    int idMisi;
    char judul[MAX_JUDUL_MISI];
    int rewardGold;
    int bonusAtk;
    int bonusHp;
    int status; /* 1 = Queue (Aktif), 2 = Stack (Selesai) */
} CatatanMisi;

int muatMisi(CatatanMisi daftar[], int maks);
int simpanMisi(const CatatanMisi daftar[], int jumlah);

int ambilMisiQueue(CatatanMisi daftarMisi[], int *jumlahMisi,
                   const Karakter daftarKar[], int jumlahKar,
                   const char *pemilik, const char *namaKarakter);

int selesaikanMisiKeStack(CatatanMisi daftarMisi[], int jumlahMisi,
                           Karakter daftarKar[], int jumlahKar,
                           const char *pemilik, const char *namaKarakter);

void tampilkanQueueMisi(const CatatanMisi daftarMisi[], int jumlahMisi,
                        const char *pemilik, const char *namaKarakter);

void tampilkanStackRiwayat(const CatatanMisi daftarMisi[], int jumlahMisi,
                           const char *pemilik, const char *namaKarakter);

#endif