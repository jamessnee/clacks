#! /bin/bash

for i in `seq 100`
  do
    echo ${i}
    ./id_gen_benchmark.out
done

echo "Fin."
