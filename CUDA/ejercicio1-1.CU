#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cuda.h>
#include <time.h>

using namespace std;


__global__
void SUM_MATRIZ_NORMAL(float* A,float* B,float* C,int rows,int cols)
{
    int i=(blockDim.x*blockIdx.x)+threadIdx.x;
    if(i<(rows*cols))
        A[i]=B[i]+C[i];
}

__global__
void SUM_MATRIZ_COLUMNAS(float* A,float* B,float* C,int rows,int cols)
{
    int index_columns=(blockDim.x*blockIdx.x)+threadIdx.x;
    if(index_columns<cols)
    {
        for(unsigned int i=0;i<rows;++i)
            A[cols*i+index_columns]=B[cols*i+index_columns]+C[cols*i+index_columns];
    }
}

//Indica que la funcion es un kernel y puede ser llamada desde una funcion host para generar grid de threads
__global__
void SUM_MATRIZ(float* A,float* B,float* C,int rows,int cols)
{
	//Block dim me indica el numero de thread en un bloque
	// Cada thread en un bloque tiene su IDX 1 ,2 etc, esto permite a cada thread combinar su THREAD ID X CON BLOCK IDX
	int index=(blockDim.x*blockIdx.x)+threadIdx.x;
	//Si no excede 
	if(index<rows)
	{
		for(int i=0;i<cols;i++)
			A[index*cols+i]=B[index*cols+i]+C[index*cols+i];
	}
}



//FUNCION PARA IMPRIMIR LA MATRIZ
void print_m(float** matrix, int fil,int col)
{
    for(int i=0;i<fil;i++)
    {
        for(int j=0;j<col;j++)
            cout<<matrix[i][j]<<'\t';
        cout<<endl;
    }
}

//Toma dos matrices como entrada B  y C y devuelve el resultado en A 
void CUDA_HOST_DEVICE(float** A,float** B,float** C,int rows,int cols)
{
	int new_size=cols*rows;
	float* host_A = new float[new_size];
	float* host_B = new float[new_size];
	float* host_C = new float[new_size];
	for(int i=0;i<rows;i++)
	{
		for(int j=0;j<cols;j++)
		{
			host_B[i*cols+j]=B[i][j];
			host_C[i*cols+j]=C[i][j];
		}
	}
	//Size para el device
	int d_size=new_size*sizeof(float);
	float* d_A;
	float* d_B;
	float* d_C;
	///Reservo memoria para los vectores pero en el device
	cudaMalloc((void**)&d_A,d_size);
	cudaMalloc((void**)&d_B,d_size);
	cudaMalloc((void**)&d_C,d_size);
	cudaMemcpy(d_B,host_B,d_size,cudaMemcpyHostToDevice);
	cudaMemcpy(d_C,host_C,d_size,cudaMemcpyHostToDevice);
	
	SUM_MATRIZ<<<ceil(new_size/256.0),256>>>(d_A,d_B,d_C,rows,cols);
	
	cudaMemcpy(host_A,d_A,d_size,cudaMemcpyDeviceToHost);

    for(int i=0;i<rows;i++)
    {
        for(int j=0;j<cols;j++)
            cout<<host_A[i*cols+j]<<'\t';
        cout<<endl;
    }
	cudaFree(d_A);
	cudaFree(d_B);
	cudaFree(d_C);
}


int main(){
    int fil  = 0;
    int cols = 0;
    cout<<"Inserte num de Filas y Columnas: ";
    cin>>fil;
    cin>>cols;
    float** A;
    float** B;
    float** C;
    //Memoria para A, B C
    A=new float*[fil];
    B=new float*[fil];
    C=new float*[fil];
    for(unsigned int i=0;i<fil;++i)
    {
        A[i]=new float[cols];
        B[i]=new float[cols];
        C[i]=new float[cols];
        for(unsigned int j=0;j<cols;++j)
        {
            B[i][j]= rand()% 10+1;
            C[i][j]= rand()% 10+1;
            A[i][j]= 0;
        }
    }
    cout<<"Matriz B:"<<endl;
    print_m(B,fil,cols);
    cout<<"Matriz C:"<<endl;
    print_m(C,fil,cols);
    
    cout<<"RESULTADOS Matriz A:"<<endl;
    CUDA_HOST_DEVICE(A,B,C,fil,cols);
    
    return 0;
}