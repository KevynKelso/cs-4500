#!/bin/bash

echo "num_threads,try_lock_lock,local_list" >> list-forming.csv

for i in {1..50}
do
    try_lock=$(./list-forming.out $i)
    lock=$(./my_list-forming.out $i)
    local_list=$(./localListForming $i)
    echo "$i,$try_lock,$lock,$local_list" >> list-forming.csv
done

sed -i 's/Total run time is//g' list-forming.csv > list-forming.csv
sed -i 's/microseconds.//g' list-forming.csv > list_forming.csv

