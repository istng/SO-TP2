#include "nodo.hpp"
#include "HashMap.hpp"
#include "mpi.h"
#include <unistd.h>
#include <stdlib.h>

using namespace std;

#define NODO_LOAD    "load"
#define NODO_ADD     "addAndInc"
#define NODO_MEMBER  "member"
#define NODO_MAXIMUM "maximum"
#define NODO_QUIT    "quit"
#define NODO_SQUIT   "q"


void nodo(unsigned int rank) {
    printf("Soy un nodo. Mi rank es %d \n", rank);

    // TODO: Implementar
    // Creo un HashMap loal

    HashMap h_loal = HashMap();
	  char message[BUFFER_SIZE];  	/* mensaje */

    while (true) {
        // TODO: Procesar mensaje

        if(MPI_Bcast(message, BUFFER_SIZE, MPI_CHAR, 0, MPI_COMM_WORLD)!=MPI_SUCCESS){
          printf("Ups...\n");
          exit(1);
        }

   	    char *cmd = strtok(message, " ");
   	    char *param = strtok(NULL, " ");

   	    //printf("CMD: %s , ", cmd);
   	    //printf("Param: %s \n", param );

		    if (strncmp(cmd, NODO_ADD, sizeof(NODO_ADD))==0) {
    		    node_add(h_loal, param, rank);
    	  }
        if (strncmp(cmd,NODO_MEMBER,sizeof(NODO_MEMBER))==0){
            node_member(h_loal,param);
        }
        if (strncmp(cmd, NODO_QUIT, sizeof(NODO_QUIT))==0 ||
            strncmp(cmd, NODO_SQUIT, sizeof(NODO_SQUIT))==0){
            // TODO: Implementar QUIT
            MPI_Finalize();
            printf("El nodo %d salio correctamente.\n",rank);
            exit(0);
        }
        if (strncmp(cmd,NODO_LOAD,sizeof(NODO_LOAD))==0){
            // TODO: Implementar LOAD
        }
        if (strncmp(cmd,NODO_MAXIMUM,sizeof(NODO_MAXIMUM))==0){
            // TODO: Implementar maximum
        }
    }
}

void trabajarArduamente() {
    int r = rand() % 2500000 + 500000;
    usleep(r);
}


void node_member(HashMap& hash_map, char* key){
  int la_tengo = hash_map.member(key) ? 1 : 0;
  trabajarArduamente();
  MPI_Send(&la_tengo, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
}







void node_add(HashMap& hash_map, char* key, unsigned int rank){
	unsigned int winner;
	MPI_Request request;
  /* Esperamos a los nodos */
  MPI_Barrier(MPI_COMM_WORLD);
  /* Enviamos nuestro rango */
  trabajarArduamente();
  MPI_Isend(&rank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);

  /* Esperamos a recibir quien sera el encargado de agregar key */
  if (MPI_Bcast(&winner, 1, MPI_INT, 0, MPI_COMM_WORLD) != MPI_SUCCESS){
    printf("Ups...\n");
    exit(1);
  }
  MPI_Barrier(MPI_COMM_WORLD);
  if (winner == rank) hash_map.addAndInc(key);
}
