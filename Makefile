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

LIBS := -lm

all : $(PROGRAM)

$(PROGRAM): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) $(LIBS) -o $@

clean:
	$(RM) -f $(OBJS)
	$(RM) -f *.png *.dot *.pdf *.dat *.svg

%.o: %.c Makefile
	$(CC) $(CFLAGS) -c $< -o $@

distclean:
	$(RM) -f $(OBJS) $(PROGRAM)
	$(RM) -f *.png *.dot *.pdf *.dat *.svg

