#ifndef AUTH_H
#define AUTH_H

#define MAX_USER 30
#define MAX_PASS 30
#define USER_FILE "data/users.txt"

typedef struct {
    char username[MAX_USER];
    char password[MAX_PASS];
} Akun;

int cekUsernameAda(const char *username);
int registerAkun(const char *username, const char *password);
int loginAkun(const char *username, const char *password);

#endif