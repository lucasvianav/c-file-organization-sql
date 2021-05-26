all:
	gcc -Wall -g -std=gnu99 source-code/main.c source-code/lines.c source-code/util.c source-code/vehicles.c -o main
mem:
	gcc -g3 -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer -Werror -Wall -std=gnu99 source-code/main.c source-code/lines.c source-code/util.c source-code/vehicles.c -o main
run:
	./main