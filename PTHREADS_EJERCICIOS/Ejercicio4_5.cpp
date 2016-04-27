#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <queue>
#include <vector>


using namespace std;

typedef int ent;
typedef long l;
ent NUM_TAREAS =  5;
int num_hilos;        //Hilos que iniciaran dormidos
queue<bool> my_queue;
vector<bool> my_vec;

//Llenamos la cola de tares
void add_task_queue(){
    for(int i=0;i<NUM_TAREAS;++i){
        my_queue.push(1); //tareas por realizar
    }
}

void *function(void *id_thread)
{
    l id = (l)id_thread;
    for(int i = 0;i<NUM_TAREAS;++i)
    {
        if(!my_vec[i]  ){ //Osea si mi thread esta dormido
            cout<<"REALIZANDO TAREA "<<endl;
            NUM_TAREAS--;
            my_queue.pop();     //Voy eliminando tareas ya resueltas
            my_vec[id] = true;  //Cada vez que se asigna una tarea, un hilo debe despertar
        }
    }
}

void init(){
    cout<<"Ingrese un numero de hilos: ";
    cin >>num_hilos;
    pthread_t* thread_user = new pthread_t[num_hilos];
    for(int i=0;i<num_hilos;++i){
        my_vec.push_back(false);   //Todos empiezan dormidos
        pthread_create(&thread_user[i],NULL,function,(void*)i); //en cada posicion creo un thread con un ID pos'i'
    }
    ///Esperamos que temrine cada thread
    for( int i=0;i<num_hilos;++i){
            pthread_join(thread_user[i], NULL);}
    //Elimino los threads creados
    delete(thread_user);
}

int main(){
    add_task_queue();
    init();
}
