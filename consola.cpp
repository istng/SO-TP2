#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utility>
#include <string>
#include <list>
#include <iostream>
#include <sstream>
#include "consola.hpp"
#include "HashMap.hpp"
#include "mpi.h"

using namespace std;

#define CMD_LOAD    "load"
#define CMD_ADD     "addAndInc"
#define CMD_MEMBER  "member"
#define CMD_MAXIMUM "maximum"
#define CMD_QUIT    "quit"
#define CMD_SQUIT   "q"
#define FINISHED "FINISHED"
#define AAI_TAG 1

static unsigned int np;

// Crea un ConcurrentHashMap distribuido
static void load(list<string> params) {



    char message[] = "load ";
    /* Avisamos a los nodos que se preparen para la transmision */
    if (MPI_Bcast(message, strlen(message)+1, MPI_CHAR, 0, MPI_COMM_WORLD) != MPI_SUCCESS){
      printf("Ups...\n");
      exit(1);
    }

    for (list<string>::iterator it=params.begin(); it != params.end(); ++it) {

       int node;
       string m = "CONTINUE " + *it;
       char* file = const_cast<char*>(m.c_str());

       /* Atendemos el request de algun nodo */
       if(MPI_Recv(&node, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE)!=MPI_SUCCESS){
         printf("Ups...\n");
         exit(1);
       }
       /* Enviamos el archivo a ese nodo */
       MPI_Send(file, strlen(file)+1, MPI_CHAR, node, 0, MPI_COMM_WORLD);
    }

    /* Avisamos a los nodos que finalizo la transmision */
    for (size_t rank = 1; rank < np; rank++) {
      char message[] = "FINISHED ";
      MPI_Send(message,strlen(message)+1, MPI_INT, rank, 0, MPI_COMM_WORLD);
    }

    /* recibimos los otros mensajes que quedaron colgados */
    for (size_t i = 1; i < np; i++) {
      int crap;
      if(MPI_Recv(&crap, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE)!=MPI_SUCCESS){
        printf("Ups...\n");
        exit(1);
      }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    cout << "La listá esta procesada" << endl;
}

// Esta función debe avisar a todos los nodos que deben terminar
static void quit() {
    // TODO: Implementar
    char message[] = "quit ";
    if (MPI_Bcast(message, strlen(message)+1, MPI_CHAR, 0, MPI_COMM_WORLD) != MPI_SUCCESS){
      printf("Ups...\n");
      exit(1);
    }
    cout << "Todos los nodos han terminado." << endl;
}

// Esta función calcula el máximo con todos los nodos
static void maximum() {
    pair<string, unsigned int> result;

    // TODO: Implementar

    char message[] = "maximum ";
    /* Avisamos a los nodos que se preparen para la transmision */
    if (MPI_Bcast(message, strlen(message)+1, MPI_CHAR, 0, MPI_COMM_WORLD) != MPI_SUCCESS){
      printf("Ups...\n");
      exit(1);
    }


    HashMap hashMap;

    for (size_t rank = 1; rank < np; rank++) {
      /* Indico al nodo rank que estoy listo para recibir */
      MPI_Send(&rank, 1, MPI_INT, rank, 0, MPI_COMM_WORLD);

      bool finished = false;
      while(!finished){
        char msg[BUFFER_SIZE];
        if(MPI_Recv(msg, BUFFER_SIZE, MPI_CHAR, rank, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE)!=MPI_SUCCESS){
          printf("Ups...\n");
          exit(1);
        }

        char* key = strtok(msg, " ");
        char *value = strtok(NULL, " ");


        if(strncmp(key,FINISHED,sizeof(FINISHED))!=0){
          unsigned int count = stoi(string(value),NULL,10);
          for (size_t i = 0; i < count; i++) {
            hashMap.addAndInc(string(key));
          }
        }else{
          finished = true;
        }
      }
    }
    MPI_Barrier(MPI_COMM_WORLD);
    result = hashMap.maximum();
    cout << "El máximo es <" << result.first <<"," << result.second << ">" << endl;
}

// Esta función busca la existencia de *key* en algún nodo
static void member(string key) {

  string m = "member " + key;
  char* message = const_cast<char*>(m.c_str());
  MPI_Status s;

  bool esta = false;
  /* Brodcast request to all nodes */
  if (MPI_Bcast(message, strlen(message)+1, MPI_CHAR, 0, MPI_COMM_WORLD) != MPI_SUCCESS){
    printf("Ups...\n");
    exit(1);
  }
  for (size_t i = 1; i < np; i++) {
    int node_response;
    if (MPI_Recv(&node_response, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &s) != MPI_SUCCESS){
      printf("Ups...\n");
      exit(1);
    }
    if(node_response==1) esta = true;
  }
    cout << "El string <" << key << (esta ? ">" : "> no") << " está" << endl;
}


// Esta función suma uno a *key* en algún nodo
static void addAndInc(string key) {

    string m = "addAndInc " + key;
    char* message = const_cast<char*>(m.c_str());
    unsigned int winner;

    int status = MPI_Bcast(message, strlen(message)+1, MPI_CHAR, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);

    status = MPI_Recv(&winner, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    status = MPI_Bcast(&winner, 1, MPI_INT, 0, MPI_COMM_WORLD);

    /* recibimos los otros mensajes que quedaron colgados */
    for (size_t i = 1; i < np - 1; i++) {
      status = MPI_Recv(&winner, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    if (status!=MPI_SUCCESS){
        printf("Ups...\n");
        exit(1);
    }
    cout << "Agregado: " << key << endl;
}


/* static int procesar_comandos()
La función toma comandos por consola e invoca a las funciones correspondientes
Si devuelve true, significa que el proceso consola debe terminar
Si devuelve false, significa que debe seguir recibiendo un nuevo comando
*/

static bool procesar_comandos() {

    char buffer[BUFFER_SIZE];
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

        load(params);
        return false;
    }

    printf("Comando no reconocido");
    return false;
}

void consola(unsigned int np_param) {
    np = np_param;
    printf("Comandos disponibles:\n");
    printf("  "CMD_LOAD" <arch_1> <arch_2> ... <arch_n>\n");
    printf("  "CMD_ADD" <string>\n");
    printf("  "CMD_MEMBER" <string>\n");
    printf("  "CMD_MAXIMUM"\n");
    printf("  "CMD_SQUIT"|"CMD_QUIT"\n");

    bool fin = false;
    while (!fin) {
        printf("> ");
        fflush(stdout);
        fin = procesar_comandos();
    }
}
