#include <omp.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <mpi.h>

#define N 10
#define DERECHA 1
#define IZQUIERDA -1
#define ABAJO N
#define ARRIBA -N

void printSolution(int * sol);
int isSafe(int * maze, int x, int y);
int solveMaze(int * maze);
int solveMazeUtil(int * tablero, int x, int y, int * solucion);

int main(int argc, const char * argv[])
{
    int myid, numprocs;
    //int data[MAXSIZE];
    //int rbuf[MAXSIZE];
    //int i, n, x, low, high, myresult = 0, result = 0;
    char hostname[MPI_MAX_PROCESSOR_NAME];
    int  longitud;
    MPI_Init(&argc,&argv);
    //MPI_Init_thread(&argc, &argv, MPI_THREAD_SERIALIZED, &provided);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
    
    
    MPI_Get_processor_name(hostname, &longitud);
    
    int * tablero = malloc(sizeof(int)*N*N);
    int casillas = N*N;
    srand(time(NULL));
    int * aux = tablero;
    int * final = tablero + casillas;
    for(; aux < final ; aux++)
    {
        *aux = 1;
    }
    //int * solucion = malloc(sizeof(int) * myid * (N*N)/numprocs);
    //int * secciones = tablero +  (myid * (N*N)/numprocs);
    //#pragma omp parallel
    {     
        solveMaze(tablero);
        /*int id = omp_get_thread_num();
        //int * secciones = malloc(sizeof(int) * id * (N*N)/numsecciones);    
        if(aux = final)
        if(myid=0)
            solveMaze(tablero);
        else
        {
            
        }
        */
       //if()
        /*#pragma omp for
        for()
        */
        
    }
    //free (tids);
    MPI_Finalize();
    
    return 0;
}

void printSolution(int * sol)
{
    for (int i = 0; i < N*N; i++)
    {
        printf(" %d ", *(sol+i));
        if (i % N == N-1)
            printf("\n");
    }
}

int isSafe(int * maze, int x, int y)
{
    if(x >= 0 && x < N && y >= 0 && y < N && *(maze + (y*N + x)))
        return 1;
 
    return 0;
}

int solveMaze(int * maze)
{
    int * sol = malloc(sizeof(int)*N*N);
    if(solveMazeUtil(maze, 0, 0, sol) == 0)
    {
        printf("Solucion no existe");
        return 0;
    }
 
    printSolution(sol);
    return 1;
}

int solveMazeUtil(int * maze, int x, int y, int * sol)
{
    printf("solve\n");
    if(x == N-1 && y == N-1)
    {
        *(sol+ (y*N + x)) = 1;
        return 1;
    }
 
    if(isSafe(maze, x, y))
    {
        *(sol+ (y*N + x))= 1;
 
        if (solveMazeUtil(maze, x+1, y, sol))
        {

            return 1;
        }
        

        if (solveMazeUtil(maze, x, y+1, sol))
            return 1;
 
        *(sol+ (y*N + x)) = 0;
        printf("adios\n");
        return 0;
    }   
 
    return 0;
}
