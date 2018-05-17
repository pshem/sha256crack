#!/bin/bash

#if not arguments were passed, default to dell
word="dell"

if [ $# == 1 ]; then
	word=$1
fi

#pass one argument - the string to hash.
max_len=${#word}
hash=$(echo -n "$word" | sha256sum | cut -d " " -f1)

#How to catch if it's invoked from the wrong place?
cd ./build || mkdir build

#compile for gprof
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_CXX_FLAGS=-p -DCMAKE_EXE_LINKER_FLAGS=-p -DCMAKE_SHARED_LINKER_FLAGS=-p ..
make -j 4

#Just 5 times, because we only need the percentages
for i in {1..5}
do
	./submission "$hash" "$max_len"
	mv gmon.out gmon$i.out
done

#get "time spent in function" %-ages
gprof -pq submission gmon1.out gmon2.out gmon3.out gmon4.out gmon5.out > gmon.txt

#remove temporary files
for i in {1..5}
do
	rm gmon$i.out
done

#compile without gprof
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo ..
make -j 4

/usr/bin/time -v ./submission "$hash" "$max_len"
#get memory usage graph
#valgrind --tool=massif --massif-out-file=memory ./submission "$hash" "$max_len"

#Keep a release version for manual testing
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j 4

#10 times to get the timing data
for i in {1..10}
do
	./submission "$hash" "$max_len" | tail -n 2 | head -n 1 | cut -d' ' -f5 >>timing.csv
done
