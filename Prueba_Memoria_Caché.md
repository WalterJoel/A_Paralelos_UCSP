# A_Paralelos_UCSP
Trabajos de Laboratorio



#include <iostream>

using namespace std;


#define MAX  100

double A[MAX][MAX], x[MAX], y[MAX];

int main()
{
    /**Caso de estudio #1 **/
    for(int i = 0; i < MAX; i++){
        for (int j = 0; j < MAX; j++){
            y[i] += A[i][j]* x[j];
    }
    
    }
    /**Caso de estudio #2 **/
    for (int j = 0; j < MAX; j++){
        for (int i = 0; i < MAX; i++){
            y[i] += A[i][j]*x[j];
        }
    }
    
    return 0;
    /*Los tiempos fueron tomados del mismo codeblocks*/
}
