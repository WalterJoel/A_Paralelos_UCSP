#include <iostream>

#include <stdlib.h>
#include <pthread.h>
#include <time.h>

using namespace std;

void* function(void* id)
{
    return NULL;
}

int main(){

    int num_threads;
    cout<<"NUMERO DE THREADS QUE DESEA CREAR ";
    cin>>num_threads;

    // TIEMPO DE INICIO
    double t_tot = 0;
    pthread_t* threads= new pthread_t[num_threads];

    for (int i = 0; i < num_threads; i++)
    {
        clock_t ini=clock();  // MIdo el tiempo de la creacion de un thread
        pthread_create(&threads[i],NULL,function,NULL);
        pthread_join(threads[i],NULL);
        t_tot += (((double)(clock()-ini))/CLOCKS_PER_SEC); //Agrego el tiempo de creacion de cada thread
    }
    t_tot = t_tot/num_threads;
    delete(threads);
    cout<<"Tiempo total para crear: "<<num_threads<<" threads "<<t_tot<<endl;
    return 0;
}