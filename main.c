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

int flag = 0;
void manejador(int ids)
{

    switch(ids)
    {

        case SIGUSR1:
            flag = 1;
            break;

    }
}

void printSolution(int * sol);
int isSafe(int * maze, int* sol, int x, int y);
int solveMaze(int * maze);
int solveMazeUtil(int * tablero, int x, int y, int * solucion, int);

int main(int argc, const char * argv[])
{
    int myid, numprocs;
    char hostname[MPI_MAX_PROCESSOR_NAME];
    int  longitud;
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
    
    
    MPI_Get_processor_name(hostname, &longitud);
    
    int * tablero = malloc(sizeof(int)*N*N);
    int casillas = N*N;
    srand(time(NULL));
    int * aux = tablero;
    int * final = tablero + casillas;
    int ** soluciones = NULL;
    for(; aux < final ; aux++)
    {
        *aux = rand()%5;
    }
    //int * solucion = malloc(sizeof(int) * myid * (N*N)/numprocs);
    //int * secciones = tablero +  (myid * (N*N)/numprocs);
    //#pragma omp parallel
    {     

        /*if(myid==0)
        {
            signal(SIGUSR1, manejador);
            //printf("???\n");
            int numero = 0;
            for(int i = 1; i < numprocs; i++)
	        {
	            while (flag = 0)
	            {
                    int * temp;
                    MPI_Recv(&temp,N*N,MPI_INT,i,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
                    *soluciones = (int *)realloc(*soluciones,(numero+1)*sizeof(int *));
                    numero++;
                    *(soluciones+numero) = temp;

                }
                flag = 1;
                printf("numero de soluciones: %d\n", numero);
            }
        }
        */
        //else
        {
            //#pragma omp parallel
            //if (omp_get_thread_num() == 0)
                solveMaze(tablero);
            //else
            /*{
                for (int i = 0; i < numprocs; i++)
                {
                    int continuar;
                    MPI_Recv(&continuar,1,MPI_INT,i,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);   
                }
            }
            */
        }

        
    }
    MPI_Finalize();
    
    return 0;
}

void printSolution(int * sol)
{
    for (int i = 0; i < N*N; i++)
    {
        printf(" %3d ", *(sol+i));
        if (i % N == N-1)
            printf("\n");
    }
    
    MPI_Send(sol,N*N,MPI_INT,0,0,MPI_COMM_WORLD);
}

int isSafe(int * maze, int* sol, int x, int y)
{
    if(x >= 0 && x < N && y >= 0 && y < N && *(maze + (y*N + x)) && !*(sol + (y*N + x)))
    {
        //printf("%d\n", *(sol + (y*N + x)));
        return 1;
    }
 
    return 0;
}

int solveMaze(int * maze)
{
    int * sol = malloc(sizeof(int)*N*N);
    solveMazeUtil(maze,0,0,sol,0);
    /*if(solveMazeUtil(maze, 0, 0, sol, 0) == 0)
    {
        printf("Solucion no existe");
        return 0;   
    }*/
    
    //printSolution(sol);
    return 1;
}

int solveMazeUtil(int * maze, int x, int y, int * sol, int current)
{
    if(x == N-1 && y == N-1)
    {
        *(sol+ (y*N + x)) = current;
        printSolution(sol);
        *(sol+ (y*N + x)) = 0;
        //return 1;
    }
 
    if(isSafe(maze, sol, x, y))
    {
        *(sol+ (y*N + x))= current;
        current++;
 
        if (solveMazeUtil(maze, x+1, y, sol, current))
        {
            return 1;
        }
        

        if (solveMazeUtil(maze, x, y+1, sol, current))
            return 1;
        if (solveMazeUtil(maze, x-1, y, sol, current))
            return 1;
            if (solveMazeUtil(maze, x, y-1, sol, current))
            return 1;
    
        *(sol+ (y*N + x)) = 0;
        current--;
        return 0;
    }   
 
    return 0;
}
