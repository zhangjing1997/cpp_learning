CFLAGS = -std=gnu99 -pedantic -Wall
myProgram: power.o test-power.o
	gcc -o myProgram power.o test-power.o
power.o: power.c 
	gcc $(CFLAGS) -c power.c
testpower.o: test-power.c 
	gcc $(CFLAGS) -c test-power.c
.PHONY: clean
clean:
	rm -f myProgram *.o *c~ *.h~
