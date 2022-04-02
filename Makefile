Program: obj/main.o obj/hippodrome.o
	gcc -o bin/Program obj/main.o obj/hippodrome.o

obj/main.o: src/main.c
	gcc -o obj/main.o -c src/main.c

obj/hippodrome.o: src/hippodrome.c
	gcc -o obj/hippodrome.o -c src/hippodrome.c

clean:
	rm -rf obj/*.o bin/Program