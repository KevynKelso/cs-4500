#!/bin/bash

# simple script for testing each of our optimizations, each optimization
# applies the optimization of the previous. e.g. `local_list` also uses the
# mutex_lock instead of try_lock, and `unbounded` uses both mutex_lock and
# local_list functionality

rm ./list-forming.csv
echo "num_threads,k,try_lock,lock,local_list,unbounded" >> list-forming.csv

gcc ./list-forming.c -pthread -o list-forming.out
gcc ./my_list-forming.c -pthread -o my_list-forming.out
gcc ./localListForming.c -pthread -o localListForming.out
gcc ./list-forming-not-cpu-bound.c -pthread -o list-forming-not-cpu-bound.out

for k in {200..2000..200}
do
    for i in {1..50}
    do
        try_lock=$(./list-forming.out $i $k)
        lock=$(./my_list-forming.out $i $k)
        local_list=$(./localListForming.out $i $k)
        unbounded=$(./list-forming-not-cpu-bound.out $i $k)
        echo "$i,$k,$try_lock,$lock,$local_list,$unbounded" >> list-forming.csv
    done
done

sed -i 's/Total run time is//g' list-forming.csv
sed -i 's/microseconds.//g' list-forming.csv

