.phony : all clean test

all : producer-consumer.c
	gcc -o producer-consumer $< -lpthread

clean : 
	rm -f ./producer-consumer

test : all
	./producer-consumer
