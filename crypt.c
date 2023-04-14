#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <zlib.h>

// define the encryption key and IV
#define KEY_SIZE 32
#define IV_SIZE 16
static unsigned char key[KEY_SIZE] = "this_is_a_secret_key_123456";
static unsigned char iv[IV_SIZE] = "1234567890abcdef";

// define a function to compress and encrypt data
unsigned char *compress_and_encrypt(unsigned char *data, size_t data_len, size_t *encrypted_data_len) {
    unsigned char *compressed_data = NULL;
    size_t compressed_data_len = 0;
    compress2(NULL, &compressed_data_len, data, data_len, Z_DEFAULT_COMPRESSION);
    compressed_data = malloc(compressed_data_len);
    if (!compressed_data) {
        return NULL;
    }
    compress2(compressed_data, &compressed_data_len, data, data_len, Z_DEFAULT_COMPRESSION);
    
    // initialize encryption context
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        free(compressed_data);
        return NULL;
    }
    if (!EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) {
        free(compressed_data);
        EVP_CIPHER_CTX_free(ctx);
        return NULL;
    }

    // calculate required buffer size for encrypted data
    int block_size = EVP_CIPHER_CTX_block_size(ctx);
    *encrypted_data_len = compressed_data_len + block_size;
    unsigned char *encrypted_data = malloc(*encrypted_data_len);
    if (!encrypted_data) {
        free(compressed_data);
        EVP_CIPHER_CTX_free(ctx);
        return NULL;
    }

    // encrypt the compressed data
    int out_len;
    if (!EVP_EncryptUpdate(ctx, encrypted_data, &out_len, compressed_data, compressed_data_len)) {
        free(compressed_data);
        free(encrypted_data);
        EVP_CIPHER_CTX_free(ctx);
        return NULL;
    }
    int final_len;
    if (!EVP_EncryptFinal_ex(ctx, encrypted_data + out_len, &final_len)) {
        free(compressed_data);
        free(encrypted_data);
        EVP_CIPHER_CTX_free(ctx);
        return NULL;
    }
    out_len += final_len;

    // cleanup and return encrypted data
    free(compressed_data);
    EVP_CIPHER_CTX_free(ctx);
    return encrypted_data;
}

// define a function to decrypt and decompress data
unsigned char *decrypt_and_decompress(unsigned char *encrypted_data, size_t encrypted_data_len, size_t *decrypted_data_len) {
    // initialize decryption context
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        return NULL;
    }
    if (!EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) {
        EVP_CIPHER_CTX_free(ctx);
        return NULL;
    }

    // calculate required buffer size for decrypted data
    int block_size = EVP_CIPHER_CTX_block_size(ctx);
    *decrypted_data_len = encrypted_data_len + block_size;
    unsigned char *decrypted_data = malloc(*decrypted_data_len);
    if (!decrypted_data) {
        EVP_CIPHER_CTX_free(ctx);
        return NULL;
    }

    // decrypt the encrypted data
    int out_len;
    if (!EVP_DecryptUpdate(ctx, decrypted_data, &out_len, encrypted_data, encrypted_data_len)) {
        free(decrypted_data);
       
