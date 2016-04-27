/* File:     serial_linked_list .c
 * Purpose:  Implement a linked list as a Parallel program (based on Pthreads) with read-write locks for the entire linked list
 *           Implementation should support Member( ), Insert( ), and Delete( ) functions.
 *           Populate the linked list with n random, but unique values.
 *           Each value should be between 0 and 2^16 - 1.
 *           Then perform m random Member, Insert, and Delete operations on the link list.
 *           Let mMember, mInsert, and mDelete be the fractions of operations of each type.
 *           You may use any values within 0 and 2^16 – 1 while performing these three operations.
 *           However, to simplify the implementation, a new value inserted into the list cannot be a
 *           value already in the list (it may be a value that was initially added to the list, but later removed).
 *
 * Compile:  gcc -g -Wall -o parallel_link_list_2 parallel_link_list_2.c -pthread
 *
 * Run:      ./parallel_link_list_2 <number of threads><n> <m> <mMember> <mInsert> <mDelete>
 *           n is the number of initial unique values in the Link List.
 *           m is number of random Member, Insert, and Delete operations on the link list.
 *           mMember is the fractions of operations of Member operation.
 *           mInsert is the fractions of operations of Insert operation.
 *           mDelete is the fractions of operations of Delete operation.
 *
 * Input:    none
 *
 * Output:   This version prints the elapsed time required for
 *           Parallel program (based on Pthreads) with read-write locks for the entire linked list  calculations.
 *
 * Notes:
 *          CS4532 Concurrent Programming
 *          Take Home Lab 1
 *          Due – Jun 28 before 11:55 PM
 *
 * Date:    07/25/2014
 *
 * Author : Wijebandara WMNC
 *          100598M
 *          Computer Science and Engineering
 *
 * Email : chameerawijebandara@gmail.com
 *
 */

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
pthread_rwlock_t rwlock;
struct list_node_s* head = NULL;

int n;
int m;
float mMember;
float mInsert;
float mDelete;
int count_member=0;
int count_insert=0;
int count_Delete=0;

int member( int value, struct  list_node_s* head_p );
int insert(int value, struct list_node_s** head_pp);
int Delete (int value, struct list_node_s** head_pp);
int printList( struct  list_node_s* head_p );
void* thread_oparation(void* rank);


/* Only executed by main thread */
void Get_args(int argc, char* argv[]);
void Usage(char* prog_name);
double Serial_pi(long long n);

/* Main function */
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
        if(!insert(r,&head))
        {
            i--;
        }
    }


    //printf("%f\n",mMember);
    //printf("%f\n",mInsert);


    thread_handles = (pthread_t*) malloc (thread_count*sizeof(pthread_t));

    start = clock();
    pthread_rwlock_init(&rwlock, NULL);

    for (thread = 0; thread < thread_count; thread++)
    {
        pthread_create(&thread_handles[thread], NULL,thread_oparation , (void*)thread);
    }

    for (thread = 0; thread < thread_count; thread++)
    {
        pthread_join(thread_handles[thread], NULL);
    }

    pthread_rwlock_destroy(&rwlock);
    finish = clock();
    elapsed = (finish - start)/CLOCKS_PER_SEC;

    printf("Elapsed time = %e seconds\n", elapsed);

    //printf("%.10f,\n", elapsed);
    //printf("Member operation count = %d\n",count_member);
    //printf("Insert operation count = %d\n",count_insert);
    //printf("Delete operation count = %d\n",count_Delete);
    //printList(head);
    return 0;
}/*main*/

/*------------------------------------------------------------------
 * Function:       thread_oparation
 * Purpose:        Compleetea the link list oparations by the thread running this
 * In arg:         rank
 * Ret val:        ignored
 * Globals in:     n, thread_count, mMember, mInsert, mDelete
 * Global in/out:  count_member, count_insert, count_Delete
 */
void* thread_oparation(void* rank)
{
    long my_rank = (long) rank;
    double factor, my_sum = 0.0;
    long long i;
    long long my_m = m/thread_count;
    for( i=0; i< my_m; i++ )
    {

        float prob = (rand()%10000/10000.0);
        //printf("%f\n",prob);


        int r = rand()%65536;
        if(prob < mMember)
        {
            pthread_rwlock_rdlock(&rwlock);
            member(r,head);
            count_member++;
            pthread_rwlock_unlock(&rwlock);
        }
        else if(prob < mMember + mInsert )
        {
            pthread_rwlock_wrlock(&rwlock);
            insert(r,&head);
            count_insert++;
            pthread_rwlock_unlock(&rwlock);
        }
        else
        {
            pthread_rwlock_wrlock(&rwlock);
            Delete(r,&head);
            count_Delete++;
            pthread_rwlock_unlock(&rwlock);
        }

    }


   return NULL;
}  /* Thread_sum */

/*------------------------------------------------------------------
 * Function:       member
 * Purpose:        Check if the given values is in the link list
 * In arg:         value, head_p
 * Globals in:
 * Global in/out:
 * Return val: Return 1 if value exist otherwise 0
 */
int member( int value, struct  list_node_s* head_p )
{
    struct list_node_s* curr_p = head_p;

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
}/* member */

/*------------------------------------------------------------------
 * Function:       insert
 * Purpose:        Add new values in to link list
 * In arg:         value, head_p
 * Globals in:
 * Global in/out:
 * Return val: Return 1 if value successfully add to the list otherwise 0
 */
int insert(int value, struct list_node_s** head_pp)
{
    struct list_node_s* curr_p = *head_pp;
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
            *head_pp = temp_p;
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
}   /*insert*/


/*------------------------------------------------------------------
 * Function:       Delete
 * Purpose:        remove values from the link list
 * In arg:         value, head_p
 * Globals in:
 * Global in/out:
 * Return val: Return 1 if value successfully remove from the list otherwise 0
 */
int Delete (int value, struct list_node_s** head_pp)
{
    struct list_node_s* curr_p = *head_pp;
    struct list_node_s* pred_p = NULL;

    while(curr_p != NULL && curr_p->data < value)
    {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if(curr_p != NULL && curr_p -> data < value)
    {
        if(pred_p == NULL){
            *head_pp = curr_p->next;
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

}   /*Delete*/


/*------------------------------------------------------------------
 * Function:    Get_args
 * Purpose:     Get the command line args
 * In args:     argc, argv
 * Globals out: thread_count, n, m, mMember, mInsert, mDelete
 */
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
int printList( struct  list_node_s* head_p )
{
    struct list_node_s* curr_p = head_p;

    while(curr_p != NULL)
    {
        printf("%d ",curr_p->data);
        curr_p = curr_p->next;
    }
    printf("\n");
}
