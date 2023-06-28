myprog: myprog.o cyl.o circle.o
	gcc -o $@ $^

%.o : %.c
	gcc -c $<

myprog.o: cyl.h
cyl.o: cyl.h circle.h
circle.o: circle.h

clean:
	rm myprog *.o

.PHONY: clean

