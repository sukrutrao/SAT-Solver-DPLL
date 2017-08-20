#! /bin/bash

i=1
for file in uf100-430/*
do
    echo $i
    ./a.out < $file 
    i=$((i+1))
done
