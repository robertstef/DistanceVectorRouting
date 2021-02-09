CC = gcc -Wall -Wextra -Wpedantic
VPATH = src:test:build
ROUTERDEP = userinput.o \
            router.o \
            socketsetup.o \
            socketmanage.o \
		    routertable.o \
		    list.o

ROUTEROBJ = build/userinput.o \
			build/router.o \
			build/socketsetup.o \
			build/socketmanage.o \
			build/routertable.o \
			build/list.o

.PHONY: all
all: router

# Compile executables
router: $(ROUTERDEP)
	$(CC) -g $(ROUTEROBJ) -o bin/router

# Object files for router
router.o: src/router_redo.c
	$(CC) -g -c  src/router_redo.c -o build/router.o

routertable.o: src/routertable.h src/routertable.c
	$(CC) -g -c src/routertable.c -o build/routertable.o

socketmanage.o: src/sman_redo.h src/sman_redo.c src/list.h src/socketsetup.h
	$(CC) -g -c src/sman_redo.c -o build/socketmanage.o

socketsetup.o: src/socketsetup.h src/socketsetup.c
	$(CC) -g -c src/socketsetup.c -o build/socketsetup.o

list.o: src/list.c src/list.h
	$(CC) -g -c src/list.c -o build/list.o

userinput.o: src/userinput.h src/userinput.c
	$(CC) -g -c src/userinput.c -o build/userinput.o

.PHONY: clean
clean:
	rm -f build/*.o bin/router
