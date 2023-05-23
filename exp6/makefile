.phony : all test clean

fakeCopy=fakecopy
myCopy=mycopy

all: $(fakeCopy).c $(myCopy).c
	gcc -o $(fakeCopy) $(fakeCopy).c
	gcc -o $(myCopy) $(myCopy).c

clean : 
	rm -f $(fakeCopy)
	rm -f $(myCopy)
	rm -f test.txt
	rm -f ./test1.txt
	rm -rf ./test
	rm -rf ./exp
	rm -f result.txt

test : clean all
	touch result.txt

	touch test.txt
	@echo this is a test >> test.txt
	@echo '执行 文件=>文件 测试'
	./$(myCopy) ./test.txt ./test1.txt >> result.txt
	@echo "\n\n" >> result.txt
	@tree . >> result.txt
	@echo "\n\n" >> result.txt
	@diff test.txt test1.txt >> result.txt

	@echo '执行 文件=>目录 测试'
	@echo "\n\n\n\n\n====================\n\n\n\n\n" >> result.txt
	@mkdir ./test
	./$(myCopy) ./test.txt ./test >> result.txt
	@echo "\n\n" >> result.txt
	@tree ./test >> result.txt
	
	@echo '执行 目录=>目录 测试'
	@echo "\n\n\n\n\n====================\n\n\n\n\n" >> result.txt
	@mkdir ./exp
	./$(myCopy) ../exp3 ./exp >> result.txt
	@echo "\n\n" >> result.txt
	@tree ../exp3 >> result.txt
	@echo "\n\n" >> result.txt
	@tree ./exp >> result.txt
	