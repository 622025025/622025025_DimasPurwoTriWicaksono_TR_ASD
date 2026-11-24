#ifndef INVENTORI_H
#define INVENTORI_H

#include "auth.h"
#include "karakter.h"

#define MAX_ITEM 100
#define MAX_NAMA_ITEM 30
#define MAX_TIPE_ITEM 20
#define INVENTORI_FILE "data/inventori.txt"

typedef struct {
    char roleTarget[MAX_ROLE_KAR]; /* "Semua" atau nama role spesifik */
    char nama[MAX_NAMA_ITEM];
    char tipe[MAX_TIPE_ITEM];
    int harga;
    int bonusAtk;
    int bonusHp;
} KatalogItem;

typedef struct {
    char pemilik[MAX_USER];
    char namaKarakter[MAX_NAMA_KAR];
    char namaItem[MAX_NAMA_ITEM];
    char tipe[MAX_TIPE_ITEM];
    int jumlah;
    int nilai;
} ItemInventori;

int muatInventori(ItemInventori daftar[], int maks);
int simpanInventori(const ItemInventori daftar[], int jumlah);
int beliItemKarakter(ItemInventori daftarInv[], int *jumlahInv,
                     Karakter daftarKar[], int jumlahKar,
                     const char *pemilik, const char *namaKarakter);
void tampilkanInventoriKarakter(const ItemInventori daftar[], int jumlah,
                                  const char *pemilik, const char *namaKarakter);

void urutkanInventoriKarakter(const ItemInventori daftar[], int jumlah,
                                const char *pemilik, const char *namaKarakter, int mode);
void cariInventoriKarakter(const ItemInventori daftar[], int jumlah,
                             const char *pemilik, const char *namaKarakter);

#endif