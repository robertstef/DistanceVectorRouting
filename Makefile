CC = gcc -Wall -Wextra -Wpedantic
ROUTER = userinput.o router.o socketsetup.o socketmanage.o \
		 list.o routertable.o

.PHONY: all
all: router

router: $(ROUTER)
	$(CC) -g $(ROUTER) -o router

router.o: router_redo.c
	$(CC) -g -c  router_redo.c -o router.o

routertable.o: routertable.h routertable.c
	$(CC) -g -c routertable.c -o routertable.o

socketmanage.o: sman_redo.h sman_redo.c list.h socketsetup.h
	$(CC) -g -c sman_redo.c -o socketmanage.o

socketsetup.o: socketsetup.h socketsetup.c
	$(CC) -g -c socketsetup.c -o socketsetup.o

list.o: list.c list.h
	$(CC) -g -c list.c -o list.o

userinput.o: userinput.h userinput.c
	$(CC) -g -c userinput.c -o userinput.o

.PHONY: clean
clean:
	rm -f *.o router
