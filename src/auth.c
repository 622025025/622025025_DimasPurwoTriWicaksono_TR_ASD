#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/auth.h"

int cekUsernameAda(const char *username) {
    FILE *file = fopen(USER_FILE, "r");
    if (file == NULL) {
        return 0;
    }

    Akun temp;
    while (fscanf(file, "%29s %29s", temp.username, temp.password) == 2) {
        if (strcmp(temp.username, username) == 0) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

int registerAkun(const char *username, const char *password) {
    if (cekUsernameAda(username)) {
        return 0;
    }

    FILE *file = fopen(USER_FILE, "a");
    if (file == NULL) {
        printf("Gagal membuka atau membuat file %s.\n", USER_FILE);
        return -1;
    }

    fprintf(file, "%s %s\n", username, password);
    fclose(file);
    return 1;
}

int loginAkun(const char *username, const char *password) {
    FILE *file = fopen(USER_FILE, "r");
    if (file == NULL) {
        return 0;
    }

    Akun temp;
    while (fscanf(file, "%29s %29s", temp.username, temp.password) == 2) {
        if (strcmp(temp.username, username) == 0 &&
            strcmp(temp.password, password) == 0) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}