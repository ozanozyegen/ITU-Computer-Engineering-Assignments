#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#define NR_mycall 355



int main(void){
	int pid=1, flag=1;
	int niceVal = 11; // nice value
	long y;
	int answer_to_fork, answer_to_flag, answer;
	printf("Welcome to fork() test code\n");
	printf("Please give the parameters to be have power on system \n");
	while(1==1){
		printf("Tip:Your current pid=%d\n",getpid());
		printf("Process id: ");
		scanf("%d",&pid);
		printf("Flag Value: ");
		scanf("%d",&flag);
		printf("Nice Value: ");
		scanf("%d",&niceVal);
		int ret;
		ret = nice(niceVal); // set new nice value
		printf("Do you want to try fork?(1/0): ");
		scanf("%d",&answer_to_fork);
		//getchar();
		if(answer_to_fork == 1){
			printf("Do you want to set the flag now?(Fork does not start yet)(1/0): ");
			scanf("%d",&answer_to_flag);
			if(answer_to_flag == 1){
				y = syscall(NR_mycall,pid,flag);
				printf("Return value of setMyFlag: %s\n",strerror(y));
			}
			pid_t pidId = fork();
			if(pidId == 0){
				printf("Heyyy i am child with id: %d\n",getpid());
				sleep(1000);

			}else if(pidId >0){
				printf("Heyyy I m parent with id: %d\n",getpid());
				sleep(1);
			if(answer_to_flag != 1){
				printf("Do you want to set the flag now?(1/0): ");
				scanf("%d",&answer_to_flag);
				if(answer_to_flag == 1){
					y = syscall(NR_mycall,pid,flag);
					printf("Return value of setMyFlag: %s\n",strerror(y));
				}
			}

			exit(0);
			}
			
		}
		
		printf("Do you want to finish the test?(1/0): ");
		scanf("%d",&answer);
		if(answer == 1) break;
	}
	return 0;
}
