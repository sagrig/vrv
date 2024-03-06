bin		:= vrv

VPATH		:=
VPATH		+= .
VPATH		+= ext
VPATH		+= cmn

CC		:= clang

CFLAGS		:=
CFLAGS		+= $(foreach p,$(VPATH),-I $(p))
CFLAGS		+= -Wall
CFLAGS		+= -Wextra
CFLAGS		+= -Werror
CFLAGS		+= -Wno-gnu-statement-expression
CFLAGS		+= -pedantic
CFLAGS		+= -pedantic-errors
CFLAGS		+= -std=c99
CFLAGS		+= -O2

CPPFLAGS	:=
CPPFLAGS	+= -MD

LDFLAGS		:=
LDLAGS		+= $(foreach p,$(VPATH),-I $(p))
LDFLAGS		+= -Wl,--build-id=0x$(shell git rev-parse HEAD)

OBJS		:=
OBJS		+= csr.o
OBJS		+= core.o
OBJS            += btree.o
OBJS            += ins.o
OBJS		+= main.o
OBJS		+= i32.o
OBJS		+= ext.o

.DEFAULT_GOAL	:= $(bin)

$(bin): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ -c $<

.PHONY: clean
clean:
	@rm -f $(bin) $(foreach p,$(VPATH),$(p)/*.o $(p)/*.d)

include $(wildcard $(foreach p,$(VPATH),$(p)/*.d))
