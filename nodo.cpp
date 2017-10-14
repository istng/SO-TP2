#include "nodo.hpp"
#include "HashMap.hpp"
#include "mpi.h"
#include <unistd.h>
#include <stdlib.h>

using namespace std;

#define NODO_ADD     "addAndInc"
#define NODO_MEMBER  "member"
#define NODO_MAXIMUM "maximum"

void nodo(unsigned int rank) {
    printf("Soy un nodo. Mi rank es %d \n", rank);

    // TODO: Implementar
    // Creo un HashMap loal

    HashMap h_loal = HashMap();
	char message[BUFFER_SIZE];  	/* mensaje */
	//unsigned int winner;
	//MPI_Request request;
	



    while (true) {
        // TODO: Procesar mensaje

    	int status = MPI_Bcast(message, BUFFER_SIZE, MPI_CHAR, 0, MPI_COMM_WORLD);
    	
   	    char *cmd = strtok(message, " ");
   	    char *param = strtok(NULL, " ");

   	    printf("CMD: %s , ", cmd);
   	    printf("Param: %s \n", param );

		if (strncmp(cmd, NODO_ADD, sizeof(NODO_ADD))==0) {
    		node_add(h_loal, param, rank);
    	}

    	/*
    	if (status!=MPI_SUCCESS){
	        printf("Ups...\n");
	        exit(1);
    	}
    	MPI_Barrier(MPI_COMM_WORLD);

    	unsigned int r = rank;
    	MPI_Isend(&r, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);

    	status = MPI_Bcast(&winner, 1, MPI_INT, 0, MPI_COMM_WORLD);
    	if (status!=MPI_SUCCESS){
	        printf("Ups...\n");
	        exit(1);
    	}
    	MPI_Barrier(MPI_COMM_WORLD);

    	if (winner == rank){
    		printf("GANE PA %d \n", rank);
    	} else {
    		printf("SOY UN PETE %d \n", rank);
    	}
*/



    	/*MPI_Send(
    void* data,
    int count,
    MPI_Datatype datatype,
    int destination,
    int tag,
    MPI_Comm communicator)



    	*/

    	//procesar_comandos(message);


    	printf("MENSAJE: %s \n",message);

    }
}

void trabajarArduamente() {
    int r = rand() % 2500000 + 500000;
    usleep(r);
}

void node_add(HashMap& hash_map, char* key, unsigned int rank){
	unsigned int winner;
	MPI_Request request;
	//MPI_Status status;

   	MPI_Barrier(MPI_COMM_WORLD);
   	unsigned int r = rank;
   	MPI_Isend(&r, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);
   	int status = MPI_Bcast(&winner, 1, MPI_INT, 0, MPI_COMM_WORLD);
   	if (status!=MPI_SUCCESS){
        printf("Ups...\n");
        exit(1);
   	}

   	MPI_Barrier(MPI_COMM_WORLD);
   	if (winner == rank){
		hash_map.addAndInc(key);
		if(hash_map.member(key)) printf("ROCK %d\n", rank);
   	} else {
   		printf("SOY UN PETE %d \n", rank);
   	}

}



/*
static bool procesar_comandos(char * buffer) {

    //char buffer[BUFFER_SIZE];
    size_t buffer_length;
    char *res, *first_param, *second_param;




    // Mi mamá no me deja usar gets :(
	res = fgets(buffer, sizeof(buffer), stdin);

    // Permitimos salir con EOF
    if (res==NULL)
        return true;

    buffer_length = strlen(buffer);
    // Si es un ENTER, continuamos
    if (buffer_length<=1)
        return false;

    // Sacamos último carácter
    buffer[buffer_length-1] = '\0';

    // Obtenemos el primer parámetro
    first_param = strtok(buffer, " ");

    if (strncmp(first_param, CMD_QUIT, sizeof(CMD_QUIT))==0 ||
        strncmp(first_param, CMD_SQUIT, sizeof(CMD_SQUIT))==0) {

        quit();
        return true;
    }

    if (strncmp(first_param, CMD_MAXIMUM, sizeof(CMD_MAXIMUM))==0) {
        maximum();
        return false;
    }

    // Obtenemos el segundo parámetro
    second_param = strtok(NULL, " ");
    if (strncmp(first_param, CMD_MEMBER, sizeof(CMD_MEMBER))==0) {
        if (second_param != NULL) {
            string s(second_param);
            member(s);
        }
        else {
            printf("Falta un parámetro\n");
        }
        return false;
    }

    if (strncmp(first_param, CMD_ADD, sizeof(CMD_ADD))==0) {
        if (second_param != NULL) {
            string s(second_param);
            addAndInc(s);
        }
        else {
            printf("Falta un parámetro\n");
        }
        return false;
    }

    if (strncmp(first_param, CMD_LOAD, sizeof(CMD_LOAD))==0) {
        list<string> params;
        while (second_param != NULL)
        {
            string s(second_param);
            params.push_back(s);
            second_param = strtok(NULL, " ");
        }

        //load(params);
        return false;
    }

    printf("Comando no reconocido");
    return false;
}*/