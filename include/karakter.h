#ifndef KARAKTER_H
#define KARAKTER_H

#include "auth.h"

#define MAX_KARAKTER 50
#define MAX_NAMA_KAR 30
#define MAX_ROLE_KAR 20
#define KARAKTER_FILE "data/karakter.txt"

typedef struct {
    char pemilik[MAX_USER];
    char nama[MAX_NAMA_KAR];
    char role[MAX_ROLE_KAR];
    int level;
    int hp;
    int atk;
    int gold;
} Karakter;

int muatKarakter(Karakter daftar[], int maks);
int simpanKarakter(const Karakter daftar[], int jumlah);
int tambahKarakter(Karakter daftar[], int *jumlah, const char *pemilik, char *karakterAktif);
void tampilkanKarakterUser(const Karakter daftar[], int jumlah, const char *pemilik, const char *karakterAktif);
int pilihKarakterAktif(const Karakter daftar[], int jumlah, const char *pemilik, char *karakterAktif);
int cariIndeksKarakterAktif(const Karakter daftar[], int jumlah, const char *pemilik, const char *namaKar);

void urutkanKarakterUser(const Karakter daftar[], int jumlah, const char *pemilik, int mode);
void cariKarakterUser(const Karakter daftar[], int jumlah, const char *pemilik);

#endif
