.phony : all clean test

EXEC_FILE = pipe

all : $(EXEC_FILE).c
	gcc -o $(EXEC_FILE) $<
	gcc -o $(EXEC_FILE)_named $(EXEC_FILE)_named.c
	gcc -o $(EXEC_FILE)_unnamed $(EXEC_FILE)_unnamed.c

clean :
	find ./ -type f -regex '$(EXEC_FILE)[^c]*' -print0 | xargs -0 rm -f
	rm -f ./$(EXEC_FILE)
	rm -f ./$(EXEC_FILE)_named
	rm -f ./$(EXEC_FILE)_unnamed

test: all
	@echo
	@echo ========= waitpid =========
	@echo
	@./$(EXEC_FILE)
	@echo
	@echo ========= named semaphore =========
	@echo
	@./$(EXEC_FILE)_named
	@echo
	@echo ========= unnamed semaphore =========
	@echo
	@./$(EXEC_FILE)_unnamed