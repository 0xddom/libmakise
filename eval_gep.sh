#!/usr/bin/env bash

pop=1200
cross=two-point,one-point
isl=3
mito=20
len=31
xmes=1

d=../makise-datasets/gep/eval

function run_test {
    csv=$d/$1.csv
    build/gep -g 10000 -p $pop -c $cross -i $isl -M $mito -d $len -x $xmes -o $csv -C
}

mkdir -p $d

for i in {0..9}; do
    run_test $i
done
