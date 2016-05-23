//Count Sort Codigo del libro

void Count_SORT (int a [], int n )
{
    int i , j , count ;
    int∗ temp = malloc( n ∗sizeof(int));
    for ( i = 0; i < n ; i ++) {
        count = 0;
        for ( j = 0; j < n ; j ++)
            if ( a [ j ] < a [ i ])
                count ++;
            else if ( a [ j ] == a [ i ] && j < i )
                count ++;
        temp [ count ] = a [ i ];
}
memcpy (a,temp,n ∗sizeof(int));
free (temp);
}

SOLUCION DEL EJERCICIO 5.3
------------------------------------

ITEM A : If we try to parallelize the for i loop (the outer loop), which variables
       should be private and which should be shared?
       - i(compartida), n(compartida), count(privada)
ITEM B : If we parallelize the for i loop using the scoping you specified in the
  previous part, are there any loop-carried dependences? Explain your answer.
        - No hay problema porque al ser count privado, no habra problemas porque cada thread tiene su stack
ITEM C :Can we parallelize the call to memcpy ? Can we modify the code so that this
                 part of the function will be parallelizable?
       - Si podemos paralelizarlo entre comillas porque no es del todo paralelo
         ya que al ponerlo como sección crítica solo lo puede ejecutar un thread a la vez.

                 #pragma omp paralell for num_threads(thread_count)
                 {
                     for ( i = 0; i < n ; i ++) {
                             count = 0;
                             for ( j = 0; j < n ; j ++)
                                 if ( a [ j ] < a [ i ])
                                     count ++;
                                 else if ( a [ j ] == a [ i ] && j < i )
                                     count ++;
                             temp [ count ] = a [ i ];
                     }
                #pragma omp critical  //para que solo un thread a la vez pueda modificar.
                    memcpy (a,temp,n ∗sizeof(int));
                    free (temp);
                 }

ITEM D :
    ///////////////
    #include<time.h>
    #include<omp.h>
    #include<stdio.h>
    #include<stdlib.h>

    void Count_Sort(int a[],int n){
        int i , j ;
        int my_rank      = omp_get_thread_num();
        int thread_count = omp_get_num_threads();
        int temp[n];
        for ( i = 0;i<n ;i ++) {
            int count = 0;
            for ( j = 0;j < n ; j ++)
                if ( a [j]< a [ i ])
                    count ++;
                else if ( a [j] == a[i] && j < i )
                    count ++;
            temp[count]= a[i];
        }
        //#pragma omp critical  //para que solo un thread a la vez pueda modificar.
        for(int i=0;i<n;i++){
            a[i] = temp[i];
        }
    }

    int main(int argc,char* argv[])
    {
        int n = 10000;
        int a[n];
        for(int i=0;i<n;i++)
        {
            a[i] = i;
        }
        // TIEMPO DE INICIO
        double t_tot = 0;
        int thread_count = strtol(argv[1],NULL,10);
        clock_t ini=clock();
        ///////////////////////////////
        #pragma omp paralell num_threads(thread_count)
        Count_Sort(a,n);

        t_tot += (((double)(clock()-ini))/CLOCKS_PER_SEC);
        printf("Tiempo total: %f " ,t_tot);

        return 0;
    }
    /////////////////////////////////////////////////
ITEM E : How does the performance of your parallelization of Count sort com-
        pare to serial Count sort ? How does it compare to the serial qsort
        library function?

  -Con un thread: Salida
        [capi@Capi Escritorio]$ ./alg 1
        Tiempo total: 0.442777

    -Con cuatro threads: Salida
        [capi@Capi Escritorio]$ ./alg 4
        Tiempo total: 0.396027
