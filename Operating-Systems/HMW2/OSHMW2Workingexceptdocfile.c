/*
 * BLG 312E - Operating Systems
 * 150120115 - Ozan Özyeğen
 * Homework - 2
 * April 1st, 2015
 */
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define NUM 3

/*
#define SEMKEY 8
#define SEMKEY2 18
#define SEMKEY3 28
#define SEMKEY4 38
*/
FILE *in_file, *out_file;
int counter=0;//Global counter for the doctors


typedef struct Nodes{
    int w_time;
    struct Node* next;
}Node;
//Node *front, *back;
typedef struct Queue{
    Node *front;
    Node *back;
    //Total Number of elements in the queue
    int num;
}Queue;

void push(Queue *queue,int new_w_time){
    Node *newnode = malloc(1*sizeof(*newnode));
    if (newnode==NULL){
        printf("malloc() is failed!\n");
        return;
    }
    newnode->w_time = new_w_time;
    newnode->next=NULL;

    if(queue->front==NULL&&queue->back==NULL){
        //printf("Empty list, adding newnode : %d\n",newnode->w_time);
        queue->front=newnode;
        queue->back=newnode;
    }else if(queue->front==NULL||queue->back==NULL){
        printf("There is an error at queue!");
        return;
    }else{
        //printf("List not empty, adding element\n");
        queue->back->next = newnode;
        queue->back = newnode;
    }
    queue->num++;
    //printf("%d",queue->num);//For Debug purposes
}
int pull(Queue *queue){
    Node *temp;
    int data;
    if(queue->front==NULL){
        printf("List is empty!");
    }
    else if(queue->front==NULL||queue->back==NULL){
        printf("Unknown error on list pull function\n");
    }
    temp=queue->front;
    data=temp->w_time;
    queue->front=queue->front->next;
    free(temp);
    if(queue->front==NULL)
        queue->back=queue->front;
    queue->num--;
    return data;
}
void free_all(Queue *queue){
    Node *a;
    while(queue->front){
        a=queue->front;
        queue->front=queue->front->next;
        free(a);
    }
    queue->back=queue->front;
}
void print_all(Queue *queue){
    Node *temp=queue->front;
    if(queue->front){
        for(temp=queue->front;temp;temp=temp->next)
            printf("Wait time: %d",temp->w_time);
    }
}
void Queue_create(Queue *queue){
    queue->front = NULL;
    queue->back = NULL;
    queue->num = 0;
}
int Queue_peek(Queue *queue, int index){
    Node *temp = queue->front;
    while(index>0){
        temp=temp->next;
    }
    return temp->w_time;
}

//Semaphores defined
int sem_queue,sem_output,sem_waitdoc;
int nurse_done=0; //Flag to show nurse is done
//sem_signal(sem_doc,1) P(v) Increase
//sem_wait(sem_doc,1) V(v) Decrement op
Queue queue;


int sem_create(int val){
    int semid=semget(IPC_PRIVATE, 1, 0700|IPC_CREAT);//Create Semaphore
    semctl(semid,0,SETVAL,val); //val is the initial value
    return semid;
}
void sem_destroy(int semid)
{
    semctl(semid, 0, IPC_RMID, 0);
}
void sem_signal(int semid,int val){
    struct sembuf semaphore;
    semaphore.sem_num = 0;
    semaphore.sem_op=val;
    semaphore.sem_flg=SEM_UNDO;
    semop(semid, &semaphore, 1);
}//V(S)
void sem_wait(int semid, int val){
    struct sembuf semaphore;
    semaphore.sem_num=0;
    semaphore.sem_op=(-1*val);
    semaphore.sem_flg =SEM_UNDO;
    semop(semid,&semaphore,1);
}//P(S)

void *Register(void *arg){
    //printf("Nurse called");
    //sem_signal(sem_output,1);

    int n;
    int counter = 0; //Overrides the global doctor counter, this counter is for nurse.
    while(!feof(in_file)){
        if(feof(in_file)) break;
        fscanf(in_file,"%d\n",&n);
        sleep(2); // Wait for 2 minutes to register
        counter++;
        sem_wait(sem_queue,1);
        //printf("QUEUE_SEMAPHORE VALUE : %d\n",semctl(sem_queue,0,GETVAL,0));
        push(&queue,n);
        sem_signal(sem_queue,1);
        //printf("QUEUE_SEMAPHORE VALUE : %d\n",semctl(sem_queue,0,GETVAL,0));
        sem_wait(sem_output,1);
        fprintf(out_file,"Patient %d registered%d\n",counter,n);
        printf("Nurse: Patient %d is registered\n",counter);
        sem_signal(sem_output,1);
        //printf("DOC_SEMAPHORE VALUE : %d\n",semctl(sem_waitdoc,0,GETVAL,0));
        sem_signal(sem_waitdoc,1);//V(sem_waitdoc) Increment, Doc can enter now
    }
    nurse_done=1;
    pthread_exit(NULL);
}
//Function of the doctors
void *Call(void *threadid){
    int* id = (int*) threadid;
    int treat_time;
    //sleep(4);//Wait until the first two patients are entered.
    //printf("Doc entered.\n");
    //printf("DOC_SEMAPHORE VALUE : %d\n",semctl(sem_waitdoc,0,GETVAL,0));
    sem_wait(sem_waitdoc,1);//wait for the nurses signal
    //printf("First wait for doc passed.\n");
    while(1){
        sem_wait(sem_queue,1);
        if(queue.front==NULL&&nurse_done){
            sem_signal(sem_queue,1);
            break;
        }
        sem_signal(sem_queue,1);
        //Wait for Nurses signal
        if(counter!=0)
            sem_wait(sem_waitdoc,1);

        //sleep(5);
        //printf("Called Doc.\n");
        sem_wait(sem_queue,1);
        treat_time = pull(&queue);
        //printf("%d\n",treat_time);
        sem_signal(sem_queue,1);
        //Treating
        sleep(treat_time);
        sem_wait(sem_output,1);
        counter++;
        printf("Doctor %d: Patient %d is treated\n",*id,counter);
        fprintf(out_file,"Doctor %d: Patient %d is treated\n",*id,counter);
        sem_signal(sem_output,1);
    }
    printf("Doc finished.\n");
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    /*
    //Create Semaphores
    sem_queue = semget(SEMKEY, 1, 0700|IPC_CREAT);
    sem_output = semget(SEMKEY2, 1, 0700|IPC_CREAT);
    sem_waitdoc = semget(SEMKEY3, 1, 0700|IPC_CREAT);
    semctl(sem_queue,0,SETVAL,1);
    semctl(sem_output,0,SETVAL,1);
    semctl(sem_waitdoc,0,SETVAL,0);
    */
    Queue_create(&queue);//Empty Queue created
    sem_queue = sem_create(1);
    sem_output = sem_create(1);
    sem_waitdoc = sem_create(0);



    //Document open
    if(argc!=3){
        printf("Errroorr");
        return 2;
    }
    else{
        in_file = fopen(argv[1],"r");
        out_file = fopen(argv[2],"w+");
        if(in_file ==0||out_file==0)
        printf("Couldn't open the file");
   }

   pthread_t nurse,doc1,doc2;
   int rc;
   int *i = malloc(sizeof(int));
   int *j = malloc(sizeof(int));
   *i=1;
   *j=2;
   rc = pthread_create(&nurse,NULL,Register,NULL);
   if(rc){
       printf("ERROR in Nurse");
   }
   rc = pthread_create(&doc1,NULL,Call,(void *)i);
   if(rc){
       printf("ERROR in Doc1");
   }
   rc = pthread_create(&doc2,NULL,Call,(void *)j);
   if(rc){
       printf("ERROR in Doc2");
   }



   /*
   printf("DOC_SEMAPHORE VALUE : %d\n",semctl(sem_waitdoc,0,GETVAL,0));
   sem_signal(sem_waitdoc,1);
   printf("DOC_SEMAPHORE VALUE : %d\n",semctl(sem_waitdoc,0,GETVAL,0));
   sem_wait(sem_waitdoc,1);
   printf("DOC_SEMAPHORE VALUE : %d\n",semctl(sem_waitdoc,0,GETVAL,0));
   sem_wait(sem_waitdoc,1);
   printf("DOC_SEMAPHORE VALUE : %d\n",semctl(sem_waitdoc,0,GETVAL,0));
   sem_wait(sem_waitdoc,1);
   */
   pthread_exit(NULL);
   free_all(&queue);//Free the queue
   /*
   semctl(sem_queue,0,IPC_RMID,0);
   semctl(sem_output,0,IPC_RMID,0);
   semctl(sem_waitdoc,0,IPC_RMID,0);
   */
   sem_destroy(sem_queue);
   sem_destroy(sem_output);
   sem_destroy(sem_waitdoc);
   return 0;
}

