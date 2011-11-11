#!/bin/sh

gcc src/*.c -Iinclude -o noflags -lgomp
gcc src/*.c -O1 -Iinclude -o O1 -lgomp
gcc src/*.c -O2 -Iinclude -o O2 -lgomp
gcc src/*.c -O3 -Iinclude -o O3 -lgomp

echo done compiling...

echo noflags
for file in perf*.dat; do
    ./noflags -f $file -d 0.25
done

echo "O1"
for file in perf*.dat; do
    ./O1 -f $file -d 0.25
done

echo "O2"
for file in perf*.dat; do
    ./O2 -f $file -d 0.25
done

echo "O3"
for file in perf*.dat; do
    ./O3 -f $file -d 0.25
done
