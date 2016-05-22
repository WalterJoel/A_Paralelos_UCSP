#include <iostream>
#include <stdio.h>
#include <math.h>

using namespace std;

//Tenemos un cuadrado de lado 2 y un circulo de radio 1
//Ahora la probabilidad de que un punto dado este en el circulo seria de p/4
//Luego se multiplica por 4 a toda la frecuencia de sucesos.. Formula


int main()
{
    int num_hits; //Numero de dardos que lanzaremos
    int i;
    cin>>num_hits;
    double x, pi, step, sum = 0.0;
    step = 1.0/(double)num_hits;  // Radio del circulo/las veces que lanzamos el dardo

    //REDUCTION hace copias locales dentro de la directiva paralela al final se combinan en un solo valor
    // haciendo el conteo de exitos dentro del circulo de radio 1(Monte Carlo)
    #pragma omp parallel for private(x) reduction (+:sum)  //Utilizamos private por el stack de cada thread
    for (i=0;i< num_hits; i++)
    {
    //Notamso el problema de dependencia de datos finding read/write
        x = (i+0.5)*step;
        sum+= 4.0/(1.0+x*x);
    }
    pi = step * sum;   //Al final multiplicamos 4 por
    printf("pi es aproximadamente %f \n",pi);
    return 0;
}


