#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DATA_LENGTH 1024

char *encrypt(char *data, int level, char *key) {
    char *encrypted_data = malloc(sizeof(char) * MAX_DATA_LENGTH);
    int i, j, k, l;
    int data_len = strlen(data);
    int key_len = strlen(key);

    for (i = 0, j = 0; i < data_len; i++, j++) {
        if (j == key_len) {
            j = 0;
        }

        if (level >= 1) {
            encrypted_data[i] = data[i] + key[j];
        } else {
            encrypted_data[i] = data[i];
        }
    }

    if (level >= 2) {
        for (k = 0, l = data_len - 1; k < l; k++, l--) {
            encrypted_data[k] ^= encrypted_data[l];
            encrypted_data[l] ^= encrypted_data[k];
            encrypted_data[k] ^= encrypted_data[l];
        }
    }

    if (level >= 3) {
        for (i = 0; i < data_len; i++) {
            encrypted_data[i] = ~encrypted_data[i];
        }
    }

    return encrypted_data;
}

char *decrypt(char *encrypted_data, int level, char *key) {
    char *data = malloc(sizeof(char) * MAX_DATA_LENGTH);
    int i, j, k, l;
    int data_len = strlen(encrypted_data);
    int key_len = strlen(key);

    for (i = 0, j = 0; i < data_len; i++, j++) {
        if (j == key_len) {
            j = 0;
        }

        if (level >= 1) {
            data[i] = encrypted_data[i] - key[j];
        } else {
            data[i] = encrypted_data[i];
        }
    }

    if (level >= 2) {
        for (k = 0, l = data_len - 1; k < l; k++, l--) {
            data[k] ^= data[l];
            data[l] ^= data[k];
            data[k] ^= data[l];
        }
    }

    if (level >= 3) {
        for (i = 0; i < data_len; i++) {
            data[i] = ~data[i];
        }
    }

    return data;
}

int main() {
    char data[MAX_DATA_LENGTH], key[MAX_DATA_LENGTH];
    int level;
    char *encrypted_data, *decrypted_data;

    printf("Enter data to encrypt: ");
    fgets(data, MAX_DATA_LENGTH, stdin);

    printf("Enter level of encryption (1-3): ");
    scanf("%d", &level);

    printf("Enter encryption key: ");
    scanf("%s", key);

    encrypted_data = encrypt(data, level, key);
    printf("Encrypted data: %s\n", encrypted_data);
    printf("Key: %s\n", key);

    decrypted_data = decrypt(encrypted_data, level, key);
    printf("Decrypted data: %s\n", decrypted_data);

    free(encrypted_data);
    free(decrypted_data);

    return 0;
}






//////////////
/*

#include <openssl/evp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 4096

int encrypt_file(char *input_file, char *output_file, unsigned char *key, unsigned char *iv) {
    EVP_CIPHER_CTX *ctx;
    int len;
    unsigned char in_buf[BUFFER_SIZE], out_buf[BUFFER_SIZE];

    FILE *in_fp = fopen(input_file, "rb");
    FILE *out_fp = fopen(output_file, "wb");

    if (!in_fp || !out_fp) {
        printf("Error opening input/output file\n");
        return -1;
    }

    ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);

    while ((len = fread(in_buf, 1, BUFFER_SIZE, in_fp)) > 0) {
        EVP_EncryptUpdate(ctx, out_buf, &len, in_buf, len);
        fwrite(out_buf, 1, len, out_fp);
    }

    EVP_EncryptFinal_ex(ctx, out_buf, &len);
    fwrite(out_buf, 1, len, out_fp);

    EVP_CIPHER_CTX_free(ctx);

    fclose(in_fp);
    fclose(out_fp);

    return 0;
}

int decrypt_file(char *input_file, char *output_file, unsigned char *key, unsigned char *iv) {
    EVP_CIPHER_CTX *ctx;
    int len;
    unsigned char in_buf[BUFFER_SIZE], out_buf[BUFFER_SIZE];

    FILE *in_fp = fopen(input_file, "rb");
    FILE *out_fp = fopen(output_file, "wb");

    if (!in_fp || !out_fp) {
        printf("Error opening input/output file\n");
        return -1;
    }

    ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);

    while ((len = fread(in_buf, 1, BUFFER_SIZE, in_fp)) > 0) {
        EVP_DecryptUpdate(ctx, out_buf, &len, in_buf, len);
        fwrite(out_buf, 1, len, out_fp);
    }

    EVP_DecryptFinal_ex(ctx, out_buf, &len);
    fwrite(out_buf, 1, len, out_fp);

    EVP_CIPHER_CTX_free(ctx);

    fclose(in_fp);
    fclose(out_fp);

    return 0;
}


*/
