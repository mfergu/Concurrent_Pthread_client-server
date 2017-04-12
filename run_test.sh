#! /bin/bash

make clean
rm temp1.* temp2.*
sleep 1
make &>compile_out.txt
cp temp1 temp1.txt; cp temp2 temp2.txt
grep -rnw "compile_out.txt" -e "error"
clear
./server 1234
