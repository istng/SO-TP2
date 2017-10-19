#!/bin/bash

for i in {1..5..1}
	do
		python3 exps.py > ./exps/corpus$i
done