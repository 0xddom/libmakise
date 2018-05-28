#!/usr/bin/env bash

g=build/gep
d=../makise-datasets/gep

function run {
    csv=$d/pop$1_$2_$4island_$3mito.csv
    echo $g -g -1 -p $1 -c $2 -d 31 -C -M $3 -i $4 -o $csv
    [ -f $csv ] || $g -g -1 -p $1 -c $2 -d 31 -C -M $3 -i $4 -o $csv
}

mkdir -p $d

run 1000 two-point 10 1 
run 1000 two-point 10 10 
run 1000 two-point 20 1
run 1000 two-point 20 10
run 1000 two-point,one-point 20 1
run 1000 two-point,one-point 20 5

run 100 two-point 10 1 
run 100 two-point 10 10 
run 100 two-point 20 1
run 100 two-point 20 10
run 100 two-point,one-point 20 1
run 100 two-point,one-point 20 5
