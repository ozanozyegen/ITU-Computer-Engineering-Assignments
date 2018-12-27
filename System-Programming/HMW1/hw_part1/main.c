#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void nextgeneration(int* gen, int* nextgen, int rulenum, int* lastitem);

int main( int argc, char *argv[] ){

	int rulenum,gennum,size;
	
	int i =0;
	FILE *fp = fopen(argv[1],"rb");
	int *gen , *nextgen;
	fscanf(fp,"%d\n",&size);
	gen = malloc (sizeof (int) * (size+2));
	nextgen = malloc (sizeof (int) * (size+2));
	int counter = 1;
	//char dummychar;
	
	
	gen[0]=0;
	nextgen[0]=0;
	gen[size+1]=0;
	nextgen[size+1]=0;
	
	
	while (!feof(fp)){
		fscanf(fp,"%d",&gen[counter]);
		counter++;
	}
	
	printf("1D Cellular Automata Program\n");
	printf("Please write the Rule Number (0 to 255) \n");
	printf("Rule Number: ");
	scanf("%d",&rulenum);
	printf("Please write the Number of Generations  \n");
	printf("Generation Number: ");
	scanf("%d",&gennum);


    for(i=0;i< size;i++){
        printf("%d",gen[i]);
    }
    printf("\n");

	for(counter=0;counter<gennum;counter++){
		nextgeneration(gen,nextgen,rulenum,&gen[size+1]);
		for(i=0;i< size;i++){
			printf("%d",nextgen[i]);
		}
		for(i=0;i< size;i++){
			gen[i]=nextgen[i];
		}
		
		printf("\n");
	}
	
	
	
	
	free(gen);
    free(nextgen);
	fclose(fp);
	return 0;
}

