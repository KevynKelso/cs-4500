#!/bin/bash

for i in {1..20}
do
    a=$(./list-forming.out $i)
    b=$(./my_list-forming.out $i)
    echo "$i,$a,$b" >> mutex_lock_vs_try_lock.txt
done



