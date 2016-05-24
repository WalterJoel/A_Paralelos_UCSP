
//HISTOGRAMA
//referencia http://www.vitutor.com/estadistica/descriptiva/a_6.html
//Argumentos que recibe  <numero de threads> <numero de elementos> <numero de histogramas por enteros>

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int max_integer(float number)
{
    return (int)number;
}

int thread_size; //cantidad de thredas
int tam_vec;     // numero de elementos
int random_ini;
int random_fin;

float* random_array=NULL;
int* contador=NULL;

//para un bloque
void histogram()
{
    if(random_array && contador)
    {
        int size=tam_vec/thread_size; //bloque para cada histograma
        int id=omp_get_thread_num();   //consigo id de los threads
        int init=id*size;              //rango del bloqe histograma
        int fin=init+size;             //rango final del bloqe histograma
        for(int i=init;i<fin;++i)       ///ENtonces en un bloque que lo hace un thread elijo
        {
            #pragma omp critical  //ovbio porque ....
            ++(contador[max_integer(random_array[i])]); ///tengo 5 intervalos -> si voy de la pos 1 al 5 imaginando
            //bloques de 5, segun la pos de 1 a 5, incremento el contador en el intervalo X segun la frecuencia
        }
    }
}

int main(int argc, char **argv)
{
    if(argc!=4)
    {
        printf("ES Necesario 3 Parámetros\n Argumentos <numero de threads> <numero de elementos> <limite aleatorio>\n");
        return 0;
    }
    const float REC = 100.0;
    thread_size     = atoi(argv[1]);
    tam_vec         = atoi(argv[2]);
    random_ini      = atoi(argv[3])*REC;
    random_fin      = random_ini/REC;
    ///validaciones,
    /// como dividire el num de elementos entre el num de threads para asignarle cada bloque
    if(tam_vec%thread_size!=0 || random_ini<=0) ///se puede perder datos si no es exacto
    {
        printf("Número de threads no es múltiplo o es mayor que el tamaño del vector o el límite del aleatorio es negativo\n");
        return 0;
    }
    //VECTOR
    
        srand(time(NULL));
        random_array=(float*)malloc(tam_vec * sizeof(float)); //lleno el array de randoms
        for(unsigned int i=0;i<tam_vec;++i)
        {
            random_array[i]=(rand()%random_ini)/REC; ///limito el random
            printf("%f ",random_array[i]);
        }
        printf("\n\n");
    
    //CONTADOR
    
        contador=(int*) malloc(random_fin * sizeof(int)); //reservo memoria
        for (unsigned int i = 0; i < random_fin; ++i)
            contador[i]=0; //contador en cada pos=0
    
    // en paralelo
    #pragma omp parallel num_threads(thread_size)
    {
        histogram();
    }
    //IMPRIMIR, retorno la altura de los histogramas
    if(contador)
    {
        for (unsigned int i = 0; i < random_fin; ++i)
            printf("%d ",contador[i]);
        printf("\n");
        free(contador);
    }
    //libero memoria si esque se ha creado
    if(random_array)
        free(random_array);
    return 0;
}

////mostrando salida

/* [capi@Capi OPEN_MP_EJERCICIOS]$ ./ejercicio1 5 20 5
3.240000 0.640000 1.230000 0.790000 1.610000 2.200000 3.070000 1.530000 3.540000 2.350000
 0.000000 3.160000 4.610000 4.220000 4.180000 4.470000 2.860000 4.140000 3.990000 1.970000 

3 4 3 5 5 
*/