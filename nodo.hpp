#ifndef _NODO_H
#define _NODO_H

#define BUFFER_SIZE 1024
#include "HashMap.hpp"


/* Función que maneja un nodo.
 * Recibe el rank del nodo.
 */
void nodo(unsigned int rank);

/* Simula un tiempo de procesamiento no determinístico.
 */
void trabajarArduamente();


void node_add(HashMap& hash_map, char* key, unsigned int rank);
void node_member(HashMap& hash_map, char* key);

#endif  /* _NODO_H */
