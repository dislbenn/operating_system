VPATH = src:include
SRC = src

objects = mybash.c util.o
all: mybash.o myls util.o mybash.e mycd mymkdir myrm mymv mybash mvbin

mybash.o: $(SRC)/mybash.c util.h
	gcc -c $(SRC)/mybash.c
	@echo "main.o created\n"

myls:
	gcc -o myls $(SRC)/myls.c $(SRC)/util.c
	@echo "myls created\n"

util.o: util.c util.h
	gcc -c $(SRC)/util.c
	@echo "util.o created\n"

mybash.e: $(object)
	gcc -o mybash.e $(SRC)/mybash.c util.o
	@echo "mybash.e created\n"

mycd:
	gcc -o mycd $(SRC)/mycd.c $(SRC)/util.c
	@echo "mycd created\n"

mymkdir:
	gcc -o mymkdir $(SRC)/mymkdir.c $(SRC)/util.c
	@echo "mymkdir created\n"

myrm:
	gcc -o myrm $(SRC)/myrm.c $(SRC)/util.c
	@echo "myrm created\n"

mymv:
	gcc -o mymv $(SRC)/mymv.c $(SRC)/util.c
	@echo "mymv created\n"

mybash:
	gcc -o mybash $(SRC)/mybash.c $(SRC)/util.c
	@echo "mybash created\n"


mvbin:
	mv my* bin/
	mv bin/*.e .
	mv bin/*.o .

.PHONY: clean
clean: # no dependencies, not implemented automatically
	rm *.o *.e
