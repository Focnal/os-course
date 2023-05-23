.PHONY : compile clean

compile : consumer.c producer.c message.h
	gcc -o producer producer.c -lrt
	gcc -o consumer consumer.c -lrt


clean : 
	rm -f ./producer
	rm -f ./consumer

