CC = gcc
# Base flag 
CFLAGS_BASE = -Wall -Wextra -pedantic -I/opt/homebrew/opt/libomp/include
LDFLAGS_BASE = -L/opt/homebrew/opt/libomp/lib -lomp

SOURCES = $(wildcard *.c) $(wildcard scenes/*.c) $(wildcard core/*.c) $(wildcard math/*.c) $(wildcard rendering/*.c) $(wildcard materials/*.c) $(wildcard textures/*.c) $(wildcard hittables/*.c)
OBJECTS = $(SOURCES:.c=.o)

# Target Default
GRTB: CFLAGS = $(CFLAGS_BASE) -O3 -Xpreprocessor -fopenmp
GRTB: LDFLAGS = $(LDFLAGS_BASE)
GRTB: $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o GRTB
	rm -f $(OBJECTS)

# Target Debug 
debug: CFLAGS = $(CFLAGS_BASE) -g -O0
debug: LDFLAGS = $(LDFLAGS_BASE)
debug: $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o GRTB_debug
	dsymutil GRTB_debug
	rm -f $(OBJECTS)

# Target Sanitize
sanitize: CFLAGS = $(CFLAGS_BASE) -g -O0 -fsanitize=address -fno-omit-frame-pointer
sanitize: LDFLAGS = -fsanitize=address
sanitize: $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o GRTB_sanitize
	rm -f $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f GRTB GRTB_debug GRTB_sanitize $(OBJECTS)
	rm -rf *.dSYM


# Target Profiling
profile: CFLAGS = $(CFLAGS_BASE) -O3 -g -Xpreprocessor -fopenmp
profile: LDFLAGS = $(LDFLAGS_BASE)
profile: $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o GRTB_profile
	dsymutil GRTB_profile
	rm -f $(OBJECTS)

