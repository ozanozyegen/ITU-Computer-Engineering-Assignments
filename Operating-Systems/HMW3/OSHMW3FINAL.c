/*
 * BLG 312E - Operating Systems
 * 150120115 - Ozan Özyeğen
 * Homework - 3
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
#include <sys/wait.h>

#define NUM 3

#define SEMKEY 8
#define SEMKEY2 18
#define SEMKEY3 28
#define SEMKEY4 38

FILE *in_file, *out_file;
int department_no;//Department No

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
int sem_queue,sem_output,sem_waitdoc,sem_xray;
int nurse_done=0; //Flag to show nurse is done
//sem_signal(sem_doc,1) P(v) Increase
//sem_wait(sem_doc,1) V(v) Decrement op
Queue queue;
char *in_filename;


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

    int n;//Buffer
    int counter = 0; // this counter is for nurse.
    while(!feof(in_file)){
        if(feof(in_file)) break;
        fscanf(in_file,"%d %d\n",&n,&n); //Doctor will read the treat time
        counter++;
        sleep(2); // Wait for 2 minutes to register
        sem_wait(sem_queue,1);
        //printf("QUEUE_SEMAPHORE VALUE : %d\n",semctl(sem_queue,0,GETVAL,0));
        push(&queue,counter);
        sem_signal(sem_queue,1);
        //printf("QUEUE_SEMAPHORE VALUE : %d\n",semctl(sem_queue,0,GETVAL,0));
        sem_wait(sem_output,1);
        //fprintf(out_file,"Patient %d registered\n",counter);
        printf("Department %d Nurse: Patient %d is registered\n",department_no,counter);
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
    int counter=0;//Count of patient
    int treat_time,a,x_ray;
    FILE *doc_read = fopen(in_filename,"r");
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

        /*
        sem_wait(sem_xray,1);
        printf("XRAY_SEMAPHORE VALUE : %d\n",semctl(sem_xray,0,GETVAL,0));
        */
        sem_signal(sem_queue,1);
        //Wait for Nurses signal
        if(counter!=0)
            sem_wait(sem_waitdoc,1);

        //sleep(5);
        //printf("Called Doc.\n");
        sem_wait(sem_queue,1);
        counter = pull(&queue);
        sem_signal(sem_queue,1);
        //Treating
        fseek(doc_read,0,SEEK_SET);
        for(a=0;a<counter;a++)
            fscanf(doc_read,"%d %d",&treat_time,&x_ray);
        //printf("Count: %d, Treat_time : %d,Xray Condition: %d\n",counter,treat_time,x_ray);
        //fscanf(doc_read,"%d",&treat_time);
        sleep(treat_time);
        if(x_ray==1){
            sem_wait(x_ray,1);
            //printf("Department: %d Patient: %d entered Xray %d\n",department_no,counter,x_ray);
            sleep(2);
            sem_signal(x_ray,1);
        }
        //rewind(doc_read);
        sem_wait(sem_output,1);
        if(x_ray==1)
            printf("Department %d Doctor %d: Patient %d is treated (X-Ray taken)\n",department_no,*id,counter);
        else
            printf("Department %d Doctor %d: Patient %d is treated\n",department_no,*id,counter);
        //fprintf(out_file,"Doctor %d: Patient %d is treated\n",*id,counter);
        //fflush(out_file);

        sem_signal(sem_output,1);
    }
    //printf("Doc finished.\n");
    fclose(doc_read);
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    department_no=0;
    //Creating children
    int c,f;
    int child[5];//Children process ids
    for(c=0;c<5;c++){
        f=fork();
        department_no++;
        if(f==-1){
            printf("Fork error....\n");
            exit(1);
        }
        if(f==0){ //Child process
            break;
        }
        child[c]=f;
        //printf("%d\n",child[c]);
    }

    if(f!=0){
        wait(NULL);//Parent waits until the child
        wait(NULL);
        wait(NULL);
        wait(NULL);
        wait(NULL);
    }
    if(f==0){//Child processes
        //printf("I am a child process\n");
        //Handling department number, deptnum is local and different for all processes.However, department_no is global.

        //printf("Child Number: %d , Department Number: %d\n",f,department_no);

        /*
        //Create Semaphores
        sem_queue = semget(SEMKEY, 1, 0700|IPC_CREAT);
        sem_output = semget(SEMKEY2, 1, 0700|IPC_CREAT);
        sem_waitdoc = semget(SEMKEY3, 1, 0700|IPC_CREAT);
        semctl(sem_queue,0,SETVAL,1);
        semctl(sem_output,0,SETVAL,1);
        semctl(sem_waitdoc,0,SETVAL,0);
        */
        sem_xray=semget(SEMKEY, 1, 0700|IPC_CREAT);//Create Semaphore
        semctl(sem_xray,0,SETVAL,3); //val is the initial value

        Queue_create(&queue);//Empty Queue created
        sem_queue = sem_create(1);
        sem_output = sem_create(1);
        sem_waitdoc = sem_create(0);



        //Document open
        if(argc!=6){
            printf("Errroorr");
            printf("%d",argc);
            return 2;
        }
        else{
            in_file = fopen(argv[department_no],"r");//1 changed with deptnum
            if(in_file ==0)
                printf("Couldn't open the file");
            in_filename=argv[department_no];
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
           semctl(sem_xray,0,IPC_RMID,0);

           sem_destroy(sem_queue);
           sem_destroy(sem_output);
           sem_destroy(sem_waitdoc);
           fclose(in_file);
           fclose(out_file);
           free(i);
           free(j);
   }

   return 0;
}

