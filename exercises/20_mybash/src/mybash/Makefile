CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -I../../include

TARGET = mybash

BINDIR = ../../bin
OBJDIR = ../../obj/mybash
LIBDIR = ../../obj

SRCS = main.c
OBJS = $(SRCS:%.c=$(OBJDIR)/%.o)

LIBS = \
$(LIBDIR)/myfile/libmyfile.a \
$(LIBDIR)/mywc/libmywc.a \
$(LIBDIR)/mysed/libmysed.a \
$(LIBDIR)/mytrans/libmytrans.a

all: $(BINDIR)/$(TARGET)

$(OBJDIR)/%.o: %.c
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BINDIR)/$(TARGET): $(OBJS)
	mkdir -p $(BINDIR)
	$(CC) $(OBJS) $(LIBS) -o $@

clean:
	rm -rf $(OBJDIR) $(BINDIR)/$(TARGET)