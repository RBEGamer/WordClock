rm wordclock.*
cmake . && make -j4 && ../picotool/picotool load -f wordclock.uf2 && ../picotool/picotool reboot