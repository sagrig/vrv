bin		:= vrv

CC		:= clang

CFLAGS		:=
CFLAGS		+= -I .
CFLAGS		+= -Wall
CFLAGS		+= -Wextra
CFLAGS		+= -Werror
CFLAGS		+= -pedantic
CFLAGS		+= -pedantic-errors
CFLAGS		+= -std=c99

CPPFLAGS	:=
CPPFLAGS	+= -MD

LDFLAGS		:=
LDFLAGS		+= -L .
LDFLAGS		+= -Wl,--build-id=0x$(shell git rev-parse HEAD)

OBJS		:=
OBJS		+= csr.o
OBJS		+= core.o
OBJS            += btree.o
OBJS            += ins.o
OBJS		+= main.o

.DEFAULT_GOAL	:= $(bin)

$(bin): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ -c $<

.PHONY: clean
clean:
	@rm -f *.d *.o $(bin)

include $(wildcard *.d)
