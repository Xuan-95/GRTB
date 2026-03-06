CC = gcc
CFLAGS = -Wall -Wextra -pedantic -Xpreprocessor -fopenmp -I/opt/homebrew/opt/libomp/include
LDFLAGS = -L/opt/homebrew/opt/libomp/lib -lomp
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)

GRTB: $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o GRTB
	rm -f $(OBJECTS)

debug: CFLAGS += -g -O0
debug: $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o GRTB
	dsymutil GRTB
	rm -f $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f GRTB $(OBJECTS)
