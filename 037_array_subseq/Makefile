CFLAGS = -ggdb3 -std=gnu99 -Wall 
myProgram: maxSeq.o test-subseq.o
	gcc -o myProgram maxSeq.o test-subseq.o
maxSeq.o: maxSeq.c
	gcc $(CFLAGS) -c maxSeq.c
test-subseq.o: test-subseq.c
	gcc $(CFLAGS) -c test-subseq.c
.PHONY: clean
clean:
	rm -f *.o *.c~ *.h~ *~

