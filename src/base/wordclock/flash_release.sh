rm WORDCLOCK_*.*
rm *.uf2
cmake . -DCMAKE_BUILD_TYPE=Release  && make -j4 && ../picotool/picotool load -f *.uf2 && ../picotool/picotool reboot