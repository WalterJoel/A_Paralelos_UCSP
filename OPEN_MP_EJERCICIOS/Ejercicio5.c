//EJERCICIO 5
//ENUNCIADO : = Use OpenMP to implement a program that does Gaussian elimination.

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>

#define MAXN 5000  /* Max value of N */
int N = 10; //size matrix

float A[MAXN][MAXN], B[MAXN], X[MAXN];

void gauss();

void Llenado_Datos() {
  int row, col;
  printf("\nInitializando todo...\n");
  for (col = 0; col < N; col++) {
    for (row = 0; row < N; row++) {
      A[row][col] = (float)rand() / 32768.0;
    }
    B[col] = (float)rand() / 32768.0;
    X[col] = 0.0;
  }
}

int main() {

  Llenado_Datos();
  gauss();
}

void gauss() {
  int i, row, col;

  for (i = 0; i < N - 1; i++) {
    #pragma omp parallel num_threads(2) private(i)
    #pragma omp parallel for
    for (row = i + 1; row < N; row++) {  //Notemos una (write/read)
      float multiplier = A[row][i] / A[i][i]; //Declaramos una variable privada multiplier para evitar la dependencia
      for (col = i; col < N; col++) {    //en multiplier conseguimos el valor a multiplicar
          A[row][col] -= A[i][col] * multiplier; //a cada elemento de la columna aplicamos el multiplier
      }
      B[row] -= B[i] * multiplier;
    }
  }
  for (row = N - 1; row >= 0; row--) {
    X[row] = B[row];
    for (col = N-1; col > row; col--) {
      X[row] -= A[row][col] * X[col];
    }
    X[row] /= A[row][row];
  }
}
