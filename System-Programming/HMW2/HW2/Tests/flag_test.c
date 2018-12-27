#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#define NR_mycall 355
/*
 * Test Program
 * 
 */
int main(void){
	long y;
	int pid, flag;
	printf("Enter Pid: ");
	scanf("%d",&pid);
	printf("Enter Flag (1 or 0): ");
	scanf("%d",&flag);
	y = syscall(NR_mycall,pid,flag);
	
	printf("Returned value: %s\n",strerror(y));
return y;
}
