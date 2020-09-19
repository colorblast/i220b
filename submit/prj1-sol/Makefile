TARGET = hamming-encode
SRC_FILES = hamming.c \
main.c \
hamming.h \
README \
Makefile \

CFLAGS = -g -Wall -std=c11
LDFLAGS = -lm

hamming-decode: $(TARGET)
	ln -s -f $< $@

$(TARGET): hamming.o main.o
	$(CC) hamming.o main.o $(LDFLAGS) -o $@

.PHONY: clean
clean:
	rm -f *.o $(PROJECT) $(PROJECT).tar.gz

depend:
	$(CC) -MM  $(CPPFLAGS) *.c

# automatically generated dependencies from make depend
hamming.o: hamming.c hamming.h
main.o: main.c hamming.h