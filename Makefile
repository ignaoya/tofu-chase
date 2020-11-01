SRC = src/*.c
game: $(SRC)
	gcc -o src/main $(SRC) -Wall -std=c99 -D_DEFAULT_SOURCE -Wno-missing-braces -s -O1 -D_DEFAULT_SOURCE -I/usr/local/include -I. -I/home/ignacio/Documents/programming/c/src -I/home/ignacio/Documents/programming/c/release/include -I/home/ignacio/Documents/programming/c/src/external -L. -L/usr/local/lib -L/home/ignacio/Documents/programming/c/src -L/home/ignacio/Documents/programming/c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -DPLATFORM_DESKTOP
