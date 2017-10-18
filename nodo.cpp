#include "nodo.hpp"
#include "HashMap.hpp"
#include "mpi.h"
#include <unistd.h>
#include <stdlib.h>
#include <string>

using namespace std;

#define NODO_LOAD    "load"
#define NODO_ADD     "addAndInc"
#define NODO_MEMBER  "member"
#define NODO_MAXIMUM "maximum"
#define NODO_QUIT    "quit"
#define NODO_SQUIT   "q"
#define NODE_FINISHED "FINISHED"

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
            node_load(h_loal,rank);
        }
        if (strncmp(cmd,NODO_MAXIMUM,sizeof(NODO_MAXIMUM))==0){
            // TODO: Implementar maximum
            node_maximum(h_loal,rank);
        }
    }
}

void trabajarArduamente() {
    int r = rand() % 2500000 + 500000;
    usleep(r);
}



/****************************************************************/

void node_member(HashMap& hash_map, char* key){
  int la_tengo = hash_map.member(key) ? 1 : 0;
  trabajarArduamente();
  MPI_Send(&la_tengo, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
}

/****************************************************************/

void node_load(HashMap& hash_map, unsigned int rank){
  MPI_Request request;
  char message[BUFFER_SIZE];

  while(true){
    /* Enviamos request al master */
    trabajarArduamente();
    MPI_Isend(&rank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);
    /* Espereamos ordenes */

    if(MPI_Recv(message, BUFFER_SIZE, MPI_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE)!=MPI_SUCCESS){
      printf("Ups...\n");
      exit(1);
    }
    char *cmd = strtok(message, " ");
    char *params = strtok(NULL, " ");

    if (strncmp(cmd,NODE_FINISHED,sizeof(NODE_FINISHED))==0) break;
    /* Cargamos el hash_map */
    string F_name(params);
    hash_map.load(F_name);
  }
  MPI_Barrier(MPI_COMM_WORLD);
}


/****************************************************************/

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

/****************************************************************/

void node_maximum(HashMap& hash_map, unsigned int rank){

  /* Espero mi turno */
  int crap;
  if(MPI_Recv(&crap, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE)!=MPI_SUCCESS){
    printf("Ups...\n");
    exit(1);
  }
  /* Comienzo la transmision */
  trabajarArduamente();
  HashMap::iterator it = hash_map.begin();
  unsigned int total_count = 0;

  while(total_count < hash_map.size() ){
    /* buscamos la proxima palabra a ser enviada */
    pair<string,unsigned int> next_word = getNextWord(hash_map,it,total_count);
    string m = next_word.first + " " + to_string(next_word.second);
    char* message = const_cast<char*>(m.c_str());
    MPI_Send(message, strlen(message)+1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
  }

  /* Aviso a la terminal que finalizo la transmision */
  char msg[] = "FINISHED  crap";
  MPI_Send(msg, strlen(msg)+1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
  MPI_Barrier(MPI_COMM_WORLD);
}






pair<string,unsigned int> getNextWord(HashMap& h,HashMap::iterator&it,unsigned int& total_count ){
  pair<string,unsigned int> res = make_pair(*it,0);
  while(total_count < h.size() && !res.first.compare(*it)){
    res.second++;
    total_count++;
    it++;
  }

  return res;
}
