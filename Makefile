install: Program

Program: obj/main.o obj/hippodrome.o obj/admin.o obj/owner.o obj/jockey.o
	gcc -o bin/Program $^ -l sqlite3

obj/main.o: src/main.c
	gcc -o $@ -c $^


obj/hippodrome.o: src/hippodrome.c
	gcc -o $@ -c $^

obj/admin.o: src/admin.c
	gcc -o $@ -c $^ 

obj/owner.o: src/owner.c
	gcc -o $@ -c $^

obj/jockey.o: src/jockey.c
	gcc -o $@ -c $^

clean:
	rm -rf obj/*.o bin/Program
