thrd:thrdcreat.o task1.o task2.o cleanup.o
	gcc -o thrd thrdcreat.o task1.o task2.o cleanup.o -lpthread
thrdcreat.o:thrdcreat.c thread.h
	gcc -c thrdcreat.c -lpthread
task1.o:task1.c
	gcc -c task1.c -lpthread
clean:
	rm -f thread *.o thrd


