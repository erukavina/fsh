# FSH (File System Hash)
A simple hashing algorithm in C that generates a unique hash value based on the contents of files within a specified directory.

It uses libssl:
```bash
sudo apt-get install libssl-dev
```

Build from source:
```
gcc -o fsh main.c -lssl -lcrypto
```