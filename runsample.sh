#!/usr/bin/env bash

if [ $# -ne 1 ]; then
  echo "Usage: ./runsample.sh CPPSOURCEFILE"
  exit
fi

echo "Compiling and running the C++ file $1..."
rm -f diff.txt
g++ -O2 $1 && time ./a.out < sample.in | diff -Bw sample.out - > diff.txt
if [[ -e diff.txt ]]; then
  if [[ -s diff.txt ]]; then
    echo "Your output does not match the expected output. Diff 
reproduced below:"
    cat diff.txt
  else
    echo "Your output matches the expected output."
  fi
fi
