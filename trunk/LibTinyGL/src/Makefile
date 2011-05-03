SOURCES = api.o arrays.o clear.o clip.o error.o get.o glu.o image_util.o init.o			\
			light.o list.o matrix.o memory.o misc.o msghandling.o select.o specbuf.o	\
			texture.o vertex.o zbuffer.o zdither.o zline.o zmath.o ztriangle.o
			
CFLAGS = -I./include -I../include
CC = gcc


.PHONY: all
all: $(SOURCES)
	mkdir -p $(LIB_FOLDER)
	ar cqs $(PROJ_LIB_A) $(SOURCES)
	$(CC) -shared -Wall -fPIC -o $(PROJ_LIB_SO) $(SOURCES)
	mv $(PROJ_LIB_SO) $(LIB_FOLDER)/$(PROJ_LIB_SO)
	mv $(PROJ_LIB_A) $(LIB_FOLDER)/$(PROJ_LIB_A)


.PHONY: clean
clean:
	rm -f *.o
	rm -f .libs/*.*
	rmdir -p .libs


$(SOURCES): %.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@
