all:
	gcc -Wall -g -std=gnu99 code/main.c code/source/lines.c code/source/util.c code/source/vehicles.c code/source/btree.c code/source/btree-lines.c code/source/btree-vehicles.c -o main
mem:
	gcc -g3 -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer -Werror -Wall -std=gnu99 code/main.c code/source/lines.c code/source/util.c code/source/vehicles.c code/source/btree.c code/source/btree-lines.c code/source/btree-vehicles.c -o main
run:
	./main
