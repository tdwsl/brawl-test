gcc -IC:\SDL2-2.0.16\x86_64-w64-mingw32\include -LC:\SDL2-2.0.16\x86_64-w64-mingw32\lib *.c -O2 -Wall -Wl,-subsystem,windows -o street -lm -lmingw32 -lSDL2main -lSDL2