#!/usr/bin/env bash

if [ $# -ne 2 ]; then
  echo "Usage: ./compare_slow_fast.sh CPPSOURCEFILE CPPSOURCEFILE"
  exit
fi

echo "Compiling and running the C++ file $1..."
#rm big.out.res
g++ -o source $1 && ./source < big.in > big.out.res
#rm random_length.out.res
g++ -o source $1 && ./source < random_length.in > random_length.out.res


#rm big.out.ref
g++ -o source $2 && ./source < big.in > big.out.ref
#rm random_length.out.ref
g++ -o source $2 && ./source < random_length.in > random_length.out.ref

diff big.out.res big.out.ref
diff random_length.out.res random_length.out.ref
echo "If user time + system time < 10 seconds, you should be fine for 
the real test."
