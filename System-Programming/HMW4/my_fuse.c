/*
    FUSE: Filesystem in Userspace
    Copyright (C) 2001-2005  Miklos Szeredi <miklos@szeredi.hu>

    This program can be distributed under the terms of the GNU GPL.
    See the file COPYING.
*/

#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <gdbm.h>
#include "base64.h"


static const char *hello_str = "Hello World!\n";
static const char *hello_path = "/hello";
static const char *database_path = "fuse.gdbm";
static const int block_size = 512;

static GDBM_FILE database;

static int open_database()
{
	printf("XXX Opening database in %s\n", database_path);
	database = gdbm_open(database_path, block_size, GDBM_NEWDB, 0777, NULL);
	if(database == NULL) {
		printf("XXX ERROR: Database NULL \n");
		return -1;
	}
	printf("XXX Storing dummy data\n");
	
	datum key = {"/", 2};
	datum value = {"d", 2};
	gdbm_store(database, key, value, GDBM_INSERT);
	
	return 0;
}

static void close_database()
{
	gdbm_close(database);
	database = NULL;
}

static void print_database()
{
	datum key, data;
	
	for(key = gdbm_firstkey(database); /* get the first key */
        key.dptr != NULL; /* keep going until the end */
        key = gdbm_nextkey(database, key)) /* next key */
    {
        /* fetch data associated to key */
        data = gdbm_fetch(database, key);
        if(data.dptr != NULL)
            printf("Entry found (%s[%d]) : %s[%d]\n", key.dptr, key.dsize, data.dptr, data.dsize);
    }
	
}

static int my_fuse_getattr(const char *path, struct stat *stbuf)
{
	printf("\n### Reading attr: %s\n", path);
	print_database();
	
	datum key, data;
    key.dptr = malloc(strlen(path)+1);
    strcpy(key.dptr, path);
    key.dsize = strlen(key.dptr) + 1;
    data = gdbm_fetch(database, key);
    
    if(data.dptr == NULL) return -ENOENT;

    memset(stbuf, 0, sizeof(struct stat));
    if (data.dptr[0] =='d') {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
    }
    else if (data.dptr[0] == 'f') {
        stbuf->st_mode = S_IFREG | 0755;
        stbuf->st_nlink = 1;
        stbuf->st_size = data.dsize;
    }
    else
    {
		printf("### ERROR: Reading directory: %s\n", path);
	}

    return 0;
}



static int my_fuse_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                         off_t offset, struct fuse_file_info *fi)
{
	printf("XXX my_fuse_readdir\n");
	printf("### PATH: %s\n", path);
	
    filler(buf, ".", NULL, 0);
    filler(buf, "..", NULL, 0);
    
    int path_length = strlen(path);
    
    datum key, data;
    for(key = gdbm_firstkey(database); key.dptr != NULL; key = gdbm_nextkey(database, key))
    {
		int key_length = strlen(key.dptr);
		if(key_length < path_length) continue;
		if(strncmp(key.dptr, path, path_length) != 0) continue;
		
		char* postfix = malloc(key_length - path_length + 1);
		strcpy(postfix, key.dptr + path_length + (path[path_length-1] == '/'?-1:0));
		printf("### KEY: %s\n", key.dptr);
		printf("### POSTFIX: %s\n", postfix);
		if(strlen(postfix+1) > 0 && strchr(postfix+1, '/') == NULL && postfix[0] == '/') {
			printf("### Adding: %s\n", postfix);
			filler(buf, postfix+1, NULL, 0);
		}
	}

	return 0;
}





static int my_fuse_open(const char *path, struct fuse_file_info *fi)
{
	
    return 0;
}



static int my_fuse_read(const char *path, char *buf, size_t size, off_t offset,
                      struct fuse_file_info *fi)
{
	printf("XXX my_fuse_read\n");
	
	datum key, data;
	key.dptr = malloc(strlen(path)+1);
	strcpy(key.dptr, path); 
	key.dsize = strlen(path) + 1;
	
	data = gdbm_fetch(database, key);
	
    if (data.dptr == NULL)
        return -ENOENT;

	int offset64 = Base64encode_len(offset);
	if(offset == 0) offset64 = 0;
	int size64 = Base64encode_len(size);
    size_t len = data.dsize;
    if (offset64 < len) {
        if (offset64 + size64 > len){
            size64 = len - offset64;
        }
        //free(buf);
        //buf = malloc(size*sizeof(char));
        printf("Decoding: %s\n", data.dptr + 1 + offset64);
        memset(buf, 0, offset + size);
		Base64decode(buf, data.dptr + 1 + offset64);
		size = Base64decode_len(data.dptr + 1 + offset64);
		buf[offset + size-1] = 0x0A;
		printf("Decoded: %s\n", buf);
		printf("Last char: %d\n", (int)buf[size-1]);
        //memcpy(buf, data.dptr + 1 + offset, size);
    } else{
        size = 0;
	}
        
    printf("Reading line: %s (%zd) Size:%zd\n", data.dptr, len, size);

    return size;
}

static int my_fuse_mkdir(const char *path, mode_t mode)
{
    (void)path;
    (void)mode;
    printf("XXX my_fuse_mkdir\n");
    
    datum key, data;
	key.dptr = malloc(strlen(path)+1);
	strcpy(key.dptr, path); 
	key.dsize = strlen(path) + 1;
	
	data = gdbm_fetch(database, key);
	
	if(data.dptr == NULL)
	{
			datum key, data;
			key.dptr = malloc(strlen(path)+1);
			strcpy(key.dptr,path);
			key.dsize = strlen(path) + 1;
			data.dptr = malloc(sizeof(char)*2);
			data.dptr[0] = 'd';
			data.dsize = 2;
			printf("Creating directory: %s\n", path);
			int res = gdbm_store(database, key, data, GDBM_INSERT);
			printf("Creating directory: %d\n", res);
			return 0;
	}
    return -EPERM;
}

static int my_fuse_rmdir (const char * path)
{
	printf("XXX my_fuse_rmdir\n");
    
    datum key, data;
	key.dptr = malloc(strlen(path)+1);
	strcpy(key.dptr, path); 
	key.dsize = strlen(path) + 1;
	
	data = gdbm_fetch(database, key);
	
	if(data.dptr != NULL)
	{
			if(data.dptr[0] != 'd') return -ENOTDIR;
			gdbm_delete(database, key);
			free(key.dptr);
			free(data.dptr);
			return 0;
	}
    return -ENOENT;
}

static int my_fuse_mknod(const char *path, mode_t mode, dev_t rdev)
{
    (void)path;
    (void)mode;
    printf("XXX my_fuse_mknod\n");
    
    datum key, data;
	key.dptr = malloc(strlen(path)+1);
	strcpy(key.dptr, path); 
	key.dsize = strlen(path) + 1;
	
	data = gdbm_fetch(database, key);
	
	if(data.dptr == NULL)
	{
			datum key, data;
			key.dptr = malloc(strlen(path)+1);
			strcpy(key.dptr,path);
			key.dsize = strlen(path) + 1;
			data.dptr = malloc(sizeof(char)*2);
			data.dptr[0] = 'f';
			data.dsize = 2;
			printf("Creating file: %s\n", path);
			int res = gdbm_store(database, key, data, GDBM_INSERT);
			printf("Creating file: %d\n", res);
			return 0;
	}
    return -EPERM;
}

static int my_fuse_unlink (const char * path)
{
	printf("XXX my_fuse_unlink\n");
    
    datum key, data;
	key.dptr = malloc(strlen(path)+1);
	strcpy(key.dptr, path); 
	key.dsize = strlen(path) + 1;
	
	data = gdbm_fetch(database, key);
	
	if(data.dptr != NULL)
	{
			if(data.dptr[0] != 'f') return -ENOTDIR;
			gdbm_delete(database, key);
			return 0;
	}
    return -ENOENT;
}

static int my_fuse_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *finfo)
{
    (void)finfo;
    int tempSize = size;
    while(buf[tempSize - 1] == 10){
		tempSize--;
	}
	printf("tempSize: %d\n", tempSize);
	
	datum key, data;
	key.dptr = malloc(strlen(path)+1);
	strcpy(key.dptr, path); 
	key.dsize = strlen(path) + 1;
	
	data = gdbm_fetch(database, key);
	int contentLength = data.dsize + Base64encode_len(size);
	int offset64 = Base64encode_len(offset);
	if(offset == 0) {
		free(data.dptr);
		data.dptr = malloc(Base64encode_len(size)+2);
		data.dptr[0] = 'f';
		data.dsize = Base64encode_len(size)+2;
		offset64 = 0;
	} else if(contentLength > data.dsize){
		char * temp = malloc(contentLength);
		memset(temp, 0, contentLength);
		strncpy(temp, data.dptr, offset64 + 1);
		free(data.dptr);
		data.dptr = temp;
		data.dsize = contentLength;
	}
	
	Base64encode(data.dptr + 1 + offset64, buf,tempSize);
    //strncpy(data.dptr + 1 + offset, buf, size);
    
    gdbm_store(database, key, data, GDBM_REPLACE);
        
    printf("Writing line: %s (%zd) Size:%zd Offset:%zd\n", buf, strlen(buf), size, offset);
    printf("Dptr: %s, Buf: %s\n", data.dptr, buf);

    return size;
}

static int my_fuse_rename (const char * old_path, const char * new_path)
{
	printf("XXX my_fuse_rename\n");
    
    datum key1, key2, data;
	key1.dptr = malloc(strlen(old_path)+1);
	strcpy(key1.dptr, old_path); 
	key1.dsize = strlen(old_path) + 1;
	
	data = gdbm_fetch(database, key1);
	
	key2.dptr = malloc(strlen(new_path)+1);
	strcpy(key2.dptr, new_path); 
	key2.dsize = strlen(key2.dptr)+1;
	
	
	if(data.dptr != NULL)
	{
			printf("Renaming: %s to %s\n", old_path, new_path);
			int res = gdbm_store(database, key2, data, GDBM_INSERT);
			printf("Renaming: %d\n", res);
			if(res == 1) {
				return -EEXIST;
				free(key2.dptr);
			}
			int path_length = strlen(old_path);
			datum key, data;
			for(key = gdbm_firstkey(database); key.dptr != NULL; key = gdbm_nextkey(database, key))
			{	
				//RENAME
				printf("RENAME Key: %s, Old Key: %s\n", key.dptr, key1.dptr);
				int key_length = key.dsize;
				if(key_length < path_length) continue;
				if(strncmp(key.dptr, key1.dptr, strlen(key1.dptr)) != 0) continue;
				if(key.dptr[path_length] != '/') continue;
				
				data = gdbm_fetch(database, key);
				
				key2.dptr = malloc(key_length - strlen(old_path) + strlen(new_path));
				strncpy(key2.dptr, new_path, strlen(new_path));
				strncpy(key2.dptr + strlen(new_path), key.dptr + strlen(old_path), key_length - strlen(old_path));
				key2.dsize = strlen(new_path) - strlen(old_path) + key_length;
				
				printf("Updating: %s to %s\n", key.dptr, key2.dptr); 
				
				gdbm_store(database, key2, data, GDBM_INSERT);
				
			}
			for(key = gdbm_firstkey(database); key.dptr != NULL; )
			{
				//DELETE
				printf("DELETE Key: %s, Old Key: %s\n", key.dptr, key1.dptr);
				int key_length = key.dsize;
				if(key_length < path_length || strncmp(key.dptr, key1.dptr, strlen(key1.dptr)) != 0) 
				{
					key = gdbm_nextkey(database, key);
					continue;
				}
				
				datum temp = key;
				key = gdbm_nextkey(database, key);
				gdbm_delete(database, temp);
				free(temp.dptr);
				
			}
			gdbm_delete(database, key1);
			free(key1.dptr);
			return 0;
			}
	return -ENOENT;
}

static struct fuse_operations my_fuse_oper = {
    .getattr	= my_fuse_getattr,
    .readdir	= my_fuse_readdir,
    .open	= my_fuse_open,
    .read	= my_fuse_read,
    .write 	= my_fuse_write,
    .mkdir	= my_fuse_mkdir,
    .rmdir	= my_fuse_rmdir,
    .mknod = my_fuse_mknod,
    .unlink = my_fuse_unlink,
    .rename = my_fuse_rename,
};

int main(int argc, char *argv[])
{
	int res;
	open_database();
	print_database();
	res = fuse_main(argc, argv, &my_fuse_oper, NULL);
	close_database();
    return res;
}
