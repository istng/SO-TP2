#!/bin/bash


for i in {1..50..1}
	do
		echo variando exp1 en $i
		mpiexec -np 5 ./dist_hashmap < ./tests/exp1/corpus1 #> "./tests/exp1/resultados/$i_1.csv"
		mpiexec -np 5 ./dist_hashmap < ./tests/exp1/corpus2 #> "./tests/exp1/resultados/$i_2.csv"
		mpiexec -np 5 ./dist_hashmap < ./tests/exp1/corpus3 #> "./tests/exp1/resultados/$i_3.csv"
		mpiexec -np 5 ./dist_hashmap < ./tests/exp1/corpus4 #> "./tests/exp1/resultados/$i_4.csv"
		mpiexec -np 5 ./dist_hashmap < ./tests/exp1/corpus5 #> "./tests/exp1/resultados/$i_5.csv"
done


for i in {1..5..1}
	do
		for j in {1..50..1}
			do
				echo variando exp2 en i: $i j: $j
				mpiexec -np $i ./dist_hashmap < ./tests/exp1/corpus1 #> "./tests/exp2/resultados/$i_$j_1.csv"
				mpiexec -np $i ./dist_hashmap < ./tests/exp1/corpus2 #> "./tests/exp2/resultados/$i_$j_2.csv"
				mpiexec -np $i ./dist_hashmap < ./tests/exp1/corpus3 #> "./tests/exp2/resultados/$i_$j_3.csv"
				mpiexec -np $i ./dist_hashmap < ./tests/exp1/corpus4 #> "./tests/exp2/resultados/$i_$j_4.csv"
				mpiexec -np $i ./dist_hashmap < ./tests/exp1/corpus5 #> "./tests/exp2/resultados/$i_$j_5.csv"
		done
done


for i in {1..50..1}
	do
		echo variando exp3 en $i
		mpiexec -np 5 ./dist_hashmap < ./tests/exp3/$i #> ./tests/exp3/resultados/$i.csv
done