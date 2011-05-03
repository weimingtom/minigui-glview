CFLAGS = -g
LIBRARIES = -ltinygl -lminigui -lpthread -lpng -ljpeg -lmgext
CC = gcc
PROJ_NAME = mini_sample


.PHONY: proj_mech
proj_mech: mech.o main.o
	$(CC) $(LIBRARIES) -o $(PROJ_NAME) mech.o main.o


.PHONY: proj_gears
proj_gears: gears.o main.o
	$(CC) $(LIBRARIES) -o $(PROJ_NAME) gears.o main.o


.PHONY: proj_spin
proj_spin: spin.o spin.o
	$(CC) $(LIBRARIES) -o $(PROJ_NAME) spin.o main.o


.PHONY: clean
clean:
	rm -f *.o
	rm -f $(PROJ_NAME)


main.o: main.c
	$(CC) -c $(CFLAGS) main.c
	
mech.o: mech.c
	$(CC) -c $(CFLAGS) mech.c

gears.o: gears.c
	$(CC) -c $(CFLAGS) gears.c
	
spin.o: spin.c
	$(CC) -c $(CFLAGS) spin.c
