#!/bin/bash
mkdir -p ./build_u2f
for filename in ./board/*.h; do
        #./MyProgram.exe "$filename" "Logs/$(basename "$filename" .txt)_Log$i.txt"
        rm -f WORDCLOCK_*.*
        rm -f *.uf2
        cmake . -DCMAKE_BUILD_TYPE=Release -DWORDCLOCK_BOARD=$(basename "$filename" .h)  && make -j4
        cp -f *.uf2 ./build_u2f
done


for filename in ./board/*.h; do
        #./MyProgram.exe "$filename" "Logs/$(basename "$filename" .txt)_Log$i.txt"
        rm -f WORDCLOCK_*.*
        rm -f *.uf2
        cmake . -DCMAKE_BUILD_TYPE=Debug -DWORDCLOCK_BOARD=$(basename "$filename" .h)  && make -j4
        cp -f *.uf2 ./build_u2f
done

# FINAL CLEANUP
rm -f WORDCLOCK_*.*
rm -f *.uf2