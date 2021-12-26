#include <stdio.h>
#include <iostream>  
#include "mpi.h" 
#define n 1000 
using namespace std; 

void smod5(void *a, void *b, int *l, MPI_Datatype *type)
{ 
    int i; 
    for (i = 0; i < *l; i++) 
    ((int *)b)[i] = (((int *)a)[i] + ((int *)b)[i]) % 5; 
} 

void max(void *a, void *b, int *l, MPI_Datatype *type)
{ 
    for (int j = 0; j < n; j++) 
    { 
    if (((int *)a)[j] > ((int *)b)[j]) 
    { 
    ((int *)b)[j] = ((int *)a)[j]; 
    } 
    }
} 


int main(int argc, char **argv) 
{ 
    int rank, size, i; 
    int a[n]; 
    int b[n]; 
    MPI_Init(&argc, &argv); 
    MPI_Op op, maxVal; 
    MPI_Comm_size(MPI_COMM_WORLD, &size); 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    for (i = 0; i < n; i++) 
    a[i] = i + rank + 1; 
    cout << "RP: " << rank << " a[0]: " << a[0] << endl;
    MPI_Op_create(&smod5, 1, &op); 
    MPI_Reduce(a, b, n, MPI_INT, op, 0, MPI_COMM_WORLD); 
    MPI_Op_free(&op); 
    if (rank == 0) cout << " "; 
    MPI_Op_create(&max, 1, &maxVal); 
    MPI_Reduce(a, b, n, MPI_INT, maxVal, 0, MPI_COMM_WORLD); 
    MPI_Op_free(&maxVal); 
    if (rank == 0) 
    { 
    cout << "Hadmade calculation: \t\tb[1] = " << b[1] << "\tb[10] = " << b[10] << "\tb[100] = " << b[100] <<"\tb[999] = " << b[999] << endl; 
    } 
    MPI_Reduce(a, b, n, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD); 
    if (rank == 0)
    { 
    cout << " MPI_MAX calculation : \tb[1] = " << b[1] << "\tb[10] = " << b[10] << "\tb[100] = " << b[100] <<"\tb[999] = " << b[999] << endl; 
    } 
    MPI_Finalize(); 
} 
