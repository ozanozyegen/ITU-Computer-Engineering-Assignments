#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "clumsy_ioctl.h"
#define BUFFER_SIZE 250


int main(){

	int file, ret;
	int mode;
	int n =-1;
	char* message;
	char* fileName;
	char* response = NULL;
	fileName = "/dev/clumsy";
	message = "This is a message with test purpose\n";
	file = open(fileName, O_RDWR);
	printf("Start The Test Test!");
	printf("Please choose the mode of clumsy\n");
	printf("Mode: ");
	scanf("%d",&mode);
	if(mode == 1){
		printf("Please choose n value for clumsy\n");
		printf("Value of n: ");
		scanf("%d",&n);
	}
	printf("***********************\n");
	printf("Writed message: %s\n",message);
	printf("***********************\n");
	ioctl(file,CLUMSY_IOCSMODE, mode); 
	ioctl(file,CLUMSY_IOCSNUM, n); 
	ret = write(file, message, strlen(message));
	if(ret<0){
			perror("Failed to write message\n");
			return errno;
	}
	response = malloc(sizeof(char)*BUFFER_SIZE);
	ret = read(file, response, BUFFER_SIZE);
	if(ret < 0){
			perror("Failed to read from file}n");
			return errno;
	}
	printf("Received Message: %s\n", response);
	printf("***********************\n");
	free(response);
	printf("End\n");
	return 0;
}
