CC = gcc -Wall -Wextra -Wpedantic
VPATH = src:test:build
LTEST = test/ltests
STEST = test/setuptests

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

LTESTDEP = list.o ltests.o
LTESTOBJ = build/list.o build/ltests.o

UCDEP = socketsetup.o uclient.o
UCOBJ = build/socketsetup.o build/uclient.o
USDEP = socketsetup.o userver.o
USOBJ = build/socketsetup.o build/userver.o

TCDEP = socketsetup.o tclient.o
TCOBJ = build/socketsetup.o build/tclient.o
TSDEP = socketsetup.o tserver.o
TSOBJ = build/socketsetup.o build/tserver.o

.PHONY: all
all: router ltests uclient userver tclient tserver

# Compile executables
router: $(ROUTERDEP)
	$(CC) -g $(ROUTEROBJ) -o bin/router

ltests: $(LTESTDEP)
	$(CC) -g $(LTESTOBJ) -o bin/ltests

uclient: $(UCDEP)
	$(CC) -g $(UCOBJ) -o bin/uclient

userver: $(USDEP)
	$(CC) -g $(USOBJ) -o bin/userver

tclient: $(TCDEP)
	$(CC) -g $(TCOBJ) -o bin/tclient

tserver: $(TSDEP)
	$(CC) -g $(TSOBJ) -o bin/tserver

# Object files for router
router.o: src/router.c
	$(CC) -g -c  src/router.c -o build/router.o

routertable.o: src/routertable.h src/routertable.c
	$(CC) -g -c src/routertable.c -o build/routertable.o

socketmanage.o: src/socketmanage.h src/socketmanage.c src/list.h src/socketsetup.h
	$(CC) -g -c src/socketmanage.c -o build/socketmanage.o

socketsetup.o: src/socketsetup.h src/socketsetup.c
	$(CC) -g -c src/socketsetup.c -o build/socketsetup.o

list.o: src/list.c src/list.h
	$(CC) -g -c src/list.c -o build/list.o

userinput.o: src/userinput.h src/userinput.c
	$(CC) -g -c src/userinput.c -o build/userinput.o


# Object files for list tests
ltests.o: $(LTEST)/ltests.c
	$(CC) -g -c -I./src $(LTEST)/ltests.c -o build/ltests.o


# Object files for setup tests
uclient.o: $(STEST)/clientUDP.c src/socketsetup.h
	$(CC) -g -I./src -c $(STEST)/clientUDP.c -o build/uclient.o

userver.o: $(STEST)/serverUDP.c src/socketsetup.h
	$(CC) -g -I./src -c $(STEST)/serverUDP.c -o build/userver.o

tclient.o: $(STEST)/clientTCP.c
	$(CC) -g -I./src -c  $(STEST)/clientTCP.c -o build/tclient.o

tserver.o: $(STEST)/serverTCP.c
	$(CC) -g -I./src -c $(STEST)/serverTCP.c -o build/tserver.o


.PHONY: clean
clean:
	rm -f build/*.o bin/*
