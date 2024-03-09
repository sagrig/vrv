bin		:= vrv
bintb           := vrvtb

# DEBUG
D		?=

VPATH		:=
VPATH		+= .
VPATH		+= ext
VPATH		+= cmn
VPATH		+= test

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
ifeq (1,$(D))
CFLAGS		+= -DVRV_DEBUG
endif

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
OBJS		+= i32.o
OBJS		+= ext.o

ifeq (1,$(D))
OBJS		+= maintb.o
else
OBJS		+= main.o
endif

ifeq (1,$(D))
.DEFAULT_GOAL	:= $(bintb)
else
.DEFAULT_GOAL	:= $(bin)
endif

$(bin) $(bintb): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

.PHONY: .FORCE
.FORCE:
	@:

%.o: %.c .FORCE
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ -c $<


.PHONY: clean
clean:
	@rm -f $(bin) $(bintb) $(foreach p,$(VPATH),$(p)/*.o $(p)/*.d)

include $(wildcard $(foreach p,$(VPATH),$(p)/*.d))
