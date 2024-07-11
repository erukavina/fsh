#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include <dirent.h>

#define BUF_SIZE 8192

void sha256_hash(const char *input, char *output) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, input, strlen(input));
    SHA256_Final(hash, &sha256);
    
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(output + (i * 2), "%02x", hash[i]);
    }
    output[64] = '\0';
}

void hash_directory(const char *dirname, char *output_hash) {
    DIR *dir;
    struct dirent *entry;
    char file_hash[65];
    SHA256_CTX ctx;

    SHA256_Init(&ctx);

    if (!(dir = opendir(dirname))) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) { // regular file
            FILE *file;
            char filepath[1024];
            snprintf(filepath, sizeof(filepath), "%s/%s", dirname, entry->d_name);

            if (!(file = fopen(filepath, "r"))) {
                perror("fopen");
                continue;
            }

            char buf[BUF_SIZE];
            size_t bytesRead;
            while ((bytesRead = fread(buf, 1, sizeof(buf), file)) > 0) {
                SHA256_Update(&ctx, buf, bytesRead);
            }

            fclose(file);
        }
    }

    closedir(dir);

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_Final(hash, &ctx);

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(output_hash + (i * 2), "%02x", hash[i]);
    }
    output_hash[64] = '\0';
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char hash_output[65];
    hash_directory(argv[1], hash_output);
    printf("%s\n", hash_output);

    return EXIT_SUCCESS;
}
