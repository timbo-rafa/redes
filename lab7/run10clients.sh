#!/bin/bash
#parallel -jobs 10 ./cliente 127.0.0.1 1400 ::: 0 1 2 3 4 5 6 7 8 9
./cliente 127.0.0.1 1400 > ${1}0.txt 2>&1 &
./cliente 127.0.0.1 1400 > ${1}1.txt 2>&1 &
./cliente 127.0.0.1 1400 > ${1}2.txt 2>&1 &
./cliente 127.0.0.1 1400 > ${1}3.txt 2>&1 &
./cliente 127.0.0.1 1400 > ${1}4.txt 2>&1 &
./cliente 127.0.0.1 1400 > ${1}5.txt 2>&1 &
./cliente 127.0.0.1 1400 > ${1}6.txt 2>&1 &
./cliente 127.0.0.1 1400 > ${1}7.txt 2>&1 &
./cliente 127.0.0.1 1400 > ${1}8.txt 2>&1 &
./cliente 127.0.0.1 1400 > ${1}9.txt 2>&1 &
wait