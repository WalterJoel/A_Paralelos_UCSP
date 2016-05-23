//Ejercicio 5.1

//Use OpenMP to implement a producer-consumer program in which some of the
//threads are producers and others are consumers. The producers read text from
//a collection of files, one per producer. They insert lines of text into a single
//shared queue. The consumers take the lines of text and tokenize them. Tokens
//are “words” separated by white space. When a consumer finds a token, it writes it to stdout .

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <queue>
#include <omp.h>
#include <iostream>
#include <time.h>

using namespace std;

#define requeriments 10

class Producer{
public:
    int prices_gomitas = 0;
    void Request(int id){
        if(id ==1){
            cout<<"Quiero saber el precio de las gomitas" <<endl;
        }
        else if(id ==2){
            cout<<"Quiero saber el precio de los chicles" <<endl;
        }
        else {
            cout<<"Quiero saber el precio de las papitas" <<endl;
        }
    }
};
class Consumer{
  public:
    ///Productos
    int price_gomitas = 0; //ID=1
    int price_chicles = 0; //ID=2
    int price_papitas = 0; //ID=3
    Consumer(int m,int y,int p){
        this->price_gomitas   = m;
        this->price_chicles  = y;
        this->price_papitas   = p;
    }
    void Response(int id){ //Responde el precio segun el ID de cada producto
        if(id ==1){
            cout<<"El precio de las gomitas es: "<<this->price_gomitas<<endl;
        }
        else if(id ==2){
            cout<<"El precio de los chicles es: "<<this->price_chicles<<endl;
        }
        else {
            cout<<"El precio de las papitas es: "<<this->price_papitas<<endl;
        }

    }
};

int main() {
    Producer P;
    Consumer C(10,5,2); //Asigno los precios de los productos
#pragma omp parallel for schedule(static, 1)
   for(int i=0;i<requeriments;i++){
       int my_rank = omp_get_thread_num();
       cout<<"my_rank"<<my_rank<<endl; //para ver q thread se ejcuta
       int id = rand()%2;
       P.Request(id);
       C.Response(id);
   }
   return 0;
}