#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <time.h>

struct list_node_s
{
    int data;
    struct list_node_s* next;
};

const int MAX_THREADS = 1024;

long thread_count;
pthread_rwlock_t rwlock; //write del pthread
struct list_node_s* raiz = NULL; //ptro

int n;
int m;
float mMember;
float mInsert;
float mDelete;
int count_member=0;
int count_insert=0;
int count_Delete=0;


/*Variables de Condicion y Mutex*/
pthread_cond_t condi_read;
pthread_cond_t condi_write;

//MIS FUNCIONES A IMPLEMENTAR
void my_read_lock();
void my_write_lock();
void lista_vacia();
pthread_mutex_t mutex;
int hacer_Algo=0;
//////////////////////////////////////////////////////
int member( int value, struct  list_node_s* raiz_p );
int insert(int value, struct list_node_s** raiz_pp);
int Delete (int value, struct list_node_s** raiz_pp);
int printList( struct  list_node_s* raiz_p );

void* thread_oparation(void* rank);


/* Only executed by main thread */
void Get_args(int argc, char* argv[]);
void Usage(char* prog_name);
double Serial_pi(long long n);

/* Main function */



void my_read_lock()
{
   pthread_mutex_lock(&mutex); // cuando quiero leer solo leer
   while(lista_vacia())
     pthread_cond_wait(&condi_read, &mutex); /*Suspende la ejecucion de un thread hasta q la condicion ocurra*/
     hacer_Algo++;
   pthread_mutex_unlock(&mutex); //Desbloqueo mi mutex
}

void my_read_unlock()
{
   pthread_mutex_lock(&list_mutex);
   if(--hacer_Algo == 0)
      pthread_cond_broadcast(&condi_write); /*CUando la condicion de mi lista_vacia ocurre Despierta el thread*/
   pthread_mutex_unlock(&mutex);
}

void my_write_lock()
{
   pthread_mutex_lock(&mutex);
   while(hacer_Algo)
      pthread_cond_wait(&condi_write,&mutex);
}

void my_write_unlock()
{
   pthread_cond_broadcast(&condi_read);
   pthread_mutex_unlock(&mutex);
}

int main(int argc, char* argv[])
{
    int i=0;
    long       thread;  /* Use long in case of a 64-bit system */
    pthread_t* thread_handles;
    double start, finish, elapsed;

    /* read command line arguments */
    Get_args(argc, argv);

    /* initially populating the link list */
    for(;i<n;i++)
    {
        int r = rand()%65536;
        if(!insert(r,&raiz))
        {
            i--;
        }
    }
    thread_handles = (pthread_t*) malloc (thread_count*sizeof(pthread_t)); // new pthread

    // AQUI EMPIEZA A ESCRIBIR PTHREAD
    start = clock();
    /***************************** *ESTO DEBO MODIFICAR
    pthread_rwlock_init(&rwlock, NULL);

    for (thread = 0; thread < thread_count; thread++)
    {
        //LLAMA ALA OPERACION QUE EJECUTA EL TRHREAD
        pthread_create(&thread_handles[thread], NULL,thread_oparation , (void*)thread);
    }
    //eSPERA QUE TERMINE C/U
    for (thread = 0; thread < thread_count; thread++)
    {
        pthread_join(thread_handles[thread], NULL);
    }

    pthread_rwlock_destroy(&rwlock); *************/
    finish = clock();
    elapsed = (finish - start)/CLOCKS_PER_SEC;

    printf("Elapsed time = %e seconds\n", elapsed);

    return 0;
}


void* thread_oparation(void* rank)
{
    long my_rank = (long) rank;
    double factor, my_sum = 0.0;
    long long i;
    long long my_m = m/thread_count;
    for( i=0; i< my_m; i++ )
    {

        float prob = (rand()%10000/10000.0);
        int r = rand()%65536;
        if(prob < mMember)
        {
            pthread_rwlock_rdlock(&rwlock);
            member(r,raiz);
            count_member++;
            pthread_rwlock_unlock(&rwlock);
        }
        else if(prob < mMember + mInsert )
        {
            pthread_rwlock_wrlock(&rwlock);
            insert(r,&raiz);
            count_insert++;
            pthread_rwlock_unlock(&rwlock);
        }
        else
        {
            pthread_rwlock_wrlock(&rwlock);
            Delete(r,&raiz);
            count_Delete++;
            pthread_rwlock_unlock(&rwlock);
        }

    }


   return NULL;
}
////funcion MEMBER
int member( int value, struct  list_node_s* raiz_p )
{
    struct list_node_s* curr_p = raiz_p;

    while( curr_p != NULL && curr_p->data < value )
    {
        curr_p = curr_p->next;
    }

    if(curr_p == NULL || curr_p->data > value)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
//INSERT
int insert(int value, struct list_node_s** raiz_pp)
{
    struct list_node_s* curr_p = *raiz_pp;
    struct list_node_s* pred_p = NULL;
    struct list_node_s* temp_p = NULL;

    while(curr_p !=NULL && curr_p->data < value)
    {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if(curr_p == NULL || curr_p->data > value)
    {
        temp_p = malloc(sizeof(struct list_node_s));
        temp_p->data = value;
        temp_p->next = curr_p;

        if(pred_p == NULL)
        {
            *raiz_pp = temp_p;
        }
        else
        {
            pred_p->next = temp_p;
        }
        return 1;

    }
    else
    {
        return 0;
    }
}

///  delete
///
int Delete (int value, struct list_node_s** raiz_pp)
{
    struct list_node_s* curr_p = *raiz_pp;
    struct list_node_s* pred_p = NULL;

    while(curr_p != NULL && curr_p->data < value)
    {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if(curr_p != NULL && curr_p -> data < value)
    {
        if(pred_p == NULL){
            *raiz_pp = curr_p->next;
            free(curr_p);
        }
        else
        {
            pred_p->next = curr_p->next;
            free(curr_p);
        }
        return 1;

    }
    else
    {
        return 0;
    }

}
/*consigue argumentos de los paramteros*/
void Get_args(int argc, char* argv[]) {
    if (argc != 7)
    {
        Usage(argv[0]);
    }
    thread_count = strtol(argv[1], NULL, 10);
    if (thread_count <= 0 || thread_count > MAX_THREADS)
    {
        Usage(argv[0]);
    }

    n = (int) strtol(argv[2], (char **)NULL, 10);
    m = (int) strtol(argv[3], (char **)NULL, 10);

    mMember = (float) atof(argv[4]);
    mInsert = (float) atof(argv[5]);
    mDelete = (float) atof(argv[6]);

   if (n <= 0 || m <= 0 || mMember + mInsert + mDelete!=1.0) Usage(argv[0]);

}
void Usage(char* prog_name) {
   fprintf(stderr, "usage: %s <number of threads> <n> <m> <mMember> <mInsert> <mDelete>\n", prog_name);
   fprintf(stderr,"n is the number of initial unique values in the Link List.\n");
   fprintf(stderr,"m is number of random Member, Insert, and Delete operations on the link list.\n");
   fprintf(stderr,"mMember is the fractions of operations of Member operation.\n");
   fprintf(stderr,"mInsert is the fractions of operations of Insert operation.\n");
   fprintf(stderr,"mDelete is the fractions of operations of Delete operation.\n");

   exit(0);
}
int printList( struct  list_node_s* raiz_p )
{
    struct list_node_s* curr_p = raiz_p;

    while(curr_p != NULL)
    {
        printf("%d ",curr_p->data);
        curr_p = curr_p->next;
    }
    printf("\n");
}

/*AGREGANDO */

/*-----------------------------------------------------------------*/
//Solo pregunto
int  lista_vacia(void) {
   if (raiz == NULL)
      return 1;
   else
      return 0;
}

