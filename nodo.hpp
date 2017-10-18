#ifndef _NODO_H
#define _NODO_H

#define BUFFER_SIZE 1024
#include "HashMap.hpp"
#include <string>
#include <utility>      // std::pair, std::make_pair



/* Función que maneja un nodo.
 * Recibe el rank del nodo.
 */
void nodo(unsigned int rank);

/* Simula un tiempo de procesamiento no determinístico.
 */
void trabajarArduamente();


void node_add(HashMap& hash_map, char* key, unsigned int rank);
void node_member(HashMap& hash_map, char* key);
void node_load(HashMap& hash_map, unsigned int rank);
void node_maximum(HashMap& hash_map, unsigned int rank);
std::pair<std::string,unsigned int> getNextWord(HashMap& h,HashMap::iterator&it,unsigned int& total_count );
#endif  /* _NODO_H */
