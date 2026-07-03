# src/mytrans/Makefile
CC = gcc
CFLAGS = -Wall -I../../include

# 库名称
LIBNAME := mytrans
# 静态库输出名：libmytrans.a
LIB := lib$(LIBNAME).a
# 最终库输出目录（比如 ./obj/src/mytrans/）
OBJDIR ?= ./obj/mytrans
# 最终 .a 文件路径
LIB_PATH := $(OBJDIR)/$(LIB)

# 源文件
SRCS := mytrans.c
OBJS := $(SRCS:.c=.o)
OBJS := $(patsubst %.o,$(OBJDIR)/%.o,$(OBJS))

all: $(LIB_PATH)

# 编译.c生成对应目录下的.o目标文件
$(OBJDIR)/%.o: %.c
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# 将所有.o打包为静态库 libmytrans.a
$(LIB_PATH): $(OBJS)
	ar rcs $(LIB_PATH) $(OBJS)

clean:
	@rm -rf $(OBJDIR)/*.o $(LIB_PATH)
