CC = gcc
CFLAGS = -Wall -Wextra -pedantic -Xpreprocessor -fopenmp -I/opt/homebrew/opt/libomp/include
LDFLAGS = -L/opt/homebrew/opt/libomp/lib -lomp
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)

GRTB: $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o GRTB
	rm -f $(OBJECTS)

debug: CFLAGS = -Wall -Wextra -g -O0
debug: LDFLAGS =
debug: $(OBJECTS)
	$(CC) $(OBJECTS) -o GRTB_debug
	dsymutil GRTB_debug
	rm -f $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f GRTB GRTB_debug $(OBJECTS)
