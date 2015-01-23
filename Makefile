CC := gcc
LD := gcc

PROGRAM := sim
C_SRCS := $(wildcard src/*.c)

OBJS := $(patsubst %.c,%.o,$(C_SRCS))

INCLUDE := -Iinclude

BASEFLAGS := -g

WARNFLAGS   := -Wall

CFLAGS := -std=c99 $(BASEFLAGS) $(WARNFLAGS) $(INCLUDE)
LDFLAGS := $(BASEFLAGS)

all : $(PROGRAM)

$(PROGRAM): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) $(LIBS) -o $@

clean:
	$(RM) -f $(OBJS) $(PROGRAM)

%.o: %.c Makefile
	$(CC) $(CFLAGS) -c $< -o $@


