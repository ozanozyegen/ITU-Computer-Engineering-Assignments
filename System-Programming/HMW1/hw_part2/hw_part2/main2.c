#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void next2generation(int* gen, int* nextgen, int rulenum, int sizex, int sizey);
void printthematrix(int* gen,int sizex,int sizey);
void copythematrix(int* dest, int* src, int size);
int i =0, j =0;

int main( int argc, char *argv[] ){

    int gennum,sizex,sizey,total;

    FILE *fprule = fopen(argv[1],"rb");
    FILE *fpseq = fopen(argv[2],"rb");
    int *gen , *nextgen;
    fscanf(fpseq,"%d %d\n",&sizex,&sizey);

    int temp=0;
    int mul=1;
    int rulenum=0;
    for(i = 0; i< 32;i++){
        fscanf(fprule,"%d",&temp);
        rulenum+=mul*temp;
        mul*=2;
    }

    total = (sizex+2)*(sizey+2);
    gen = malloc (sizeof (int) * (total));
    nextgen = malloc (sizeof (int) * (total));

    for(i=0;i<total;i++){
        gen[i]=0;
    }

    for(i=1;i<=sizey;i++){
        for(j=1;j<=sizex;j++){
            fscanf(fpseq,"%d",&gen[i*(sizex+2)+j]);
        }

    }


    printf("2D Cellular Automata Program\n");

    while(1){
        system("clear");
        printthematrix(gen, sizex, sizey);
        next2generation(gen, nextgen, rulenum, sizex, sizey); //Create next gen
        copythematrix(gen, nextgen, total); //Gen=next gen
        getchar();
    }

    free(gen);
    free(nextgen);
    fclose(fprule);
    fclose(fpseq);
    return 0;
}

void printthematrix(int *gen, int sizex, int sizey){

    for(i=1;i<=sizey;i++){
        for(j=1;j<=sizex;j++){
            printf("%d ",gen[i*(sizex+2)+j]);
        }
        printf("\n");
    }
}


void copythematrix(int* dest, int* src, int size){
    for(i=0;i<size;i++){
        dest[i]=src[i];
    }
}
