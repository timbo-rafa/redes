#!/bin/bash
for i in $(seq 0 10)
do
  echo $i
  ./servidor 1400 $i > servidor${i}.txt 2>&1 &
  ./cliente 127.0.0.1 1400 > ${i}0.txt 2>&1 &
  ./cliente 127.0.0.1 1400 > ${i}1.txt 2>&1 &
  ./cliente 127.0.0.1 1400 > ${i}2.txt 2>&1 &
  ./cliente 127.0.0.1 1400 > ${i}3.txt 2>&1 &
  ./cliente 127.0.0.1 1400 > ${i}4.txt 2>&1 &
  ./cliente 127.0.0.1 1400 > ${i}5.txt 2>&1 &
  ./cliente 127.0.0.1 1400 > ${i}6.txt 2>&1 &
  ./cliente 127.0.0.1 1400 > ${i}7.txt 2>&1 &
  ./cliente 127.0.0.1 1400 > ${i}8.txt 2>&1 &
  ./cliente 127.0.0.1 1400 > ${i}9.txt 2>&1 &
  echo "read p"
  read p
  pkill -P $$
done
