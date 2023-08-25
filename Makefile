CC = gcc
CFLAGS = -std=c99 -Wall -Werror -pedantic -g
LDFLAGS = -lm
OBJECTS = main.o endianness.o pixel.o imagen.o argumentos.o filtro.o procesamiento.o
PROGRAM = 20202_tp1

all: $(PROGRAM)

$(PROGRAM): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(PROGRAM) $(LDFLAGS)

endianness.o: endianness.c endianness.h
	$(CC) $(CFLAGS) -c endianness.c

procesamiento.o: procesamiento.c procesamiento.h
	$(CC) $(CFLAGS) -c procesamiento.c

pixel.o: pixel.c pixel.h 
	$(CC) $(CFLAGS) -c pixel.c

filtro.o: filtro.c pixel.h imagen.h filtro.h
	$(CC) $(CFLAGS) -c filtro.c

imagen.o: imagen.c pixel.h imagen.h endianness.h
	$(CC) $(CFLAGS) -c imagen.c

argumentos.o: argumentos.c argumentos.h pixel.h imagen.h filtro.h
	$(CC) $(CFLAGS) -c argumentos.c


main.o: main.c argumentos.h procesamiento.h pixel.h imagen.h filtro.h
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -vf *.o $(PROGRAM)
