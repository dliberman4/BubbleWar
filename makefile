COBJS=getnum.o archivos.o backend.o frontend.o
CFLAGS= -Wall -Wuninitialized -ansi -pedantic
TPE: $(COBJS)
	gcc -o TPE $(COBJS) $(CFLAGS)
getnum.o:getnum.h
archivos.o:archivos.h
backend.o:backend.h
typedefs.o:typedefs.h

clean:
	rm *.o

All: clean TPE

