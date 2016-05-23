//EJERCICIO 5.4

for ( row = n − 1; row >= 0; row −− ) {
    x [ row ] = b [ row ];
    for ( col = row +1; col < n ; col ++)
        x [ row ] − = A [ row ][ col ]∗ x [ col ];
    x [ row ] /= A [ row ][ row ];
}
ITEM A: Determine whether the outer loop of the row-oriented algorithm can be
       parallelized.
       -  Si puede ser paralelizado pero agregando ciertas restricciones, primero veamos que
        un thread id=0 podría hacer x[row] = b[row] y otro thread id=1, podria hacer x[row]/= A[row][row]
        entonces debemos de hacer que x sea privada para que cada thread maneje ambas operaciones en su propia
        stack.
ITEM B : Determine whether the inner loop of the row-oriented algorithm can be
        parallelized.
        - Puede ser paralelizado tranquilamente, no hay dependencia de datos menos un (write/read) a la vez.

for ( row = 0; row < n ; row ++)
    x [ row ] = b [ row ];

for ( col = n − 1; col >= 0; col −− ) {
  x [ col ] /= A [ col ][ col ];
  for ( row = 0; row < col ; row ++)
     x [ row ] − = A [ row ][ col ]∗ x [ col ];
  }
ITEM C: Determine whether the (second) outer loop of the column-oriented algorithm
can be parallelized.
  - Se puede paralelizar sin problemas, no hay dependencia de datos porque solo se hace una operación que es x[col]
    y no genera problemas.
ITEM D: Determine whether the inner loop of the column-oriented algorithm can be
parallelized.
  - De igual manera se puede paralelizar porque independientemente de si un thread u otro modifica no habra
    dependencia de datos.

ITEM E: Write one OpenMP program for each of the loops that you determined could
           be parallelized. You may find the single directive useful—when a block
           of code is being executed in parallel and *******a sub-block should be executed by
           only one thread***********, the sub-block can be modified by a #pragma omp single
           directive. ******The threads in the executing team will block at the end of the
           directive until all of the threads have completed it****(2)************ .


           PARA EL CASO 1 : ROW-ORIENTED
           -----------------------------
           #pragma omp parallel for num_threads(thread_count)
            for ( row = n − 1; row >= 0; row −− ) {
                  int x[n];     ///Creando mi x privado(toda variable dentro de una directiva es privada)
                  x [ row ] = b [ row ];
           #pragma omp critical                    //Con esto hago la parte 2 del escrito
                  for ( col = row +1; col < n ; col ++)
                      x [ row ] − = A [ row ][ col ]∗ x [ col ];
                  x [ row ] /= A [ row ][ row ];
           }


           PARA EL CASO 1 : COLUM-ORIENTED
           --------------------------------
           #pragma omp parallel for num_threads(thread_count)
           for ( col = n − 1; col >= 0; col −− ) {
                x [ col ] /= A [ col ][ col ];
                for ( row = 0; row < col ; row ++)
                     x [ row ] − = A [ row ][ col ]∗ x [ col ];
           }

ITEM F:
    Modify your parallel loop with a schedule(runtime) clause and test the
    program with various schedules. If your upper triangular system has 10,000
    variables, which schedule gives the best performance?
    /////modificando 
    
    #include <stdio.h>
    #include <omp.h>

    int main (void)
    {
      int i,row,col;
      int n = 10;
      int b[n];
    //  int x[n];
      for(int i=0;i<n;i++){
         b[i]=i;
         x[i]=i; } //Lleno ambos vecotres
      #pragma omp parallel num_threads(8)
      {
        #pragma omp for schedule(runtime) //RUntime solo recibe un parametro
        for ( row = n −1; row >= 0; row−− ) {
            int x[n];     ///Creando mi x privado(toda variable dentro de una directiva es privada)
            x[row]= b[row];
            printf("[1] iteracion %0d, id_thread %0d\n",row, omp_get_thread_num());
        #pragma omp critical                    //Con esto hago la parte 2 del escrito
        for(col = row +1; col < n ; col++){
                x[row] −= A[row][col]∗x[col];
                x[row] /= A[row][row];
            printf("[2] iteracion %0d, id_thread %0d\n",row, omp_get_thread_num());}
       }
      }
      return 0;
    }

    ////////CON UPPER TRIANGULAR SYSTEM ANALISIS 
    static  : si porque sabemos que en la ultima linea de la matriz el calculo
              es sencillo solo una variable, las demas lineas tienen mas variables, y
              de acuerdo a eso los threads son asignados en un inicio antes de q el bucle inicie.
    dynamic : No se adecua a este problema (0 500 501 100)
    auto    : No porque el compilador o sistema determinaria el schedule
    