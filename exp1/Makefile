.phony : all clean test

all : diningPhilosophers.c
	gcc -o diningPhilosophers $< -lpthread

test : all
	./diningPhilosophers > ./out.txt
	code ./out.txt

clean :
	rm -f ./diningPhilosophers
	rm -f ./out.txt
