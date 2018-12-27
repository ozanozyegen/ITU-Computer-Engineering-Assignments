gcc my_fuse.c base64.c -o my_fuse -D_FILE_OFFSET_BITS=64 -lfuse -lgdbm
./my_fuse -d fuse
