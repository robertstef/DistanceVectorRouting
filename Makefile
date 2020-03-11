# Robert Stefanyshin
# ras867
# 11137532
# CMPT434
# A3

CC = gcc -Wall -Wextra -Wpedantic
ROUTER = userinput.o router.o socketsetup.o routerfcns.o socketmanage.o \
		 list.o

.PHONY: all
all: router

router: $(ROUTER)
	$(CC) -g $(ROUTER) -o router

router.o: router.c userinput.h
	$(CC) -g -c router.c -o router.o

routerfcns.o: routerfcns.h routerfcns.c
	$(CC) -g -c routerfcns.c -o routerfcns.o

socketmanage.o: socketmanage.h list.h socketmanage.c
	$(CC) -g -c socketmanage.c -o socketmanage.o

socketsetup.o: socketsetup.h socketsetup.c
	$(CC) -g -c socketsetup.c -o socketsetup.o

list.o: list.c list.h
	$(CC) -g -c list.c -o list.o

userinput.o: userinput.h userinput.c
	$(CC) -g -c userinput.c -o userinput.o

.PHONY: clean
clean:
	rm -f *.o router
