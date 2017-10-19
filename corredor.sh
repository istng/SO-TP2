#!/bin/bash

#Calculo de distribución (nos importa cada palabra y cada rank, por eso el mismo corpus y nada de avg)
mpiexec -np 8 ./dist_hashmap < ./tests/exps/corpus1000 2> "./resultados/distr/1000.csv"
#mpiexec -np 5 ./dist_hashmap < ./tests/exps/corpus1 2> "./resultados/distr/2.csv"
#mpiexec -np 5 ./dist_hashmap < ./tests/exps/corpus1 2> "./resultados/distr/3.csv"
#mpiexec -np 5 ./dist_hashmap < ./tests/exps/corpus1 2> "./resultados/distr/4.csv"
#mpiexec -np 5 ./dist_hashmap < ./tests/exps/corpus1 2> "./resultados/distr/5.csv"


#Calculo de tiempos con variación de procesos (no nos interesa el rank, nos interesa el tiempo, pero de addAndInc en general, no por palabra, por eso sin avg)
#for i in {6..8..1}
#	do
#		mpiexec -np $i ./dist_hashmap < ./tests/exps/corpus1 2> "./resultados/var/$i/rc1.csv"
#		mpiexec -np $i ./dist_hashmap < ./tests/exps/corpus2 2> "./resultados/var/$i/rc2.csv"
#		mpiexec -np $i ./dist_hashmap < ./tests/exps/corpus3 2> "./resultados/var/$i/rc3.csv"
#		mpiexec -np $i ./dist_hashmap < ./tests/exps/corpus4 2> "./resultados/var/$i/rc4.csv"
#		mpiexec -np $i ./dist_hashmap < ./tests/exps/corpus5 2> "./resultados/var/$i/rc5.csv"
#done