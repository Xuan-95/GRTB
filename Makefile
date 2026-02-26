CC = gcc
CFLAGS = -Wall -Wextra -pedantic
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)

GRTB: $(OBJECTS)
	$(CC) $(OBJECTS) -o GRTB
	rm -f $(OBJECTS)


debug: CFLAGS += -g -O0
debug: $(OBJECTS)
	$(CC) $(OBJECTS) -o GRTB $(LDFLAGS)
	dsymutil GRTB
	rm -f $(OBJECTS)


%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f GRTB $(OBJECTS)
