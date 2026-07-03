# src/myfile/Makefile
CC = gcc
CFLAGS = -Wall -I../../include -g -fPIE

# 库名称
LIBNAME := myfile
# 静态库输出名：libmyfile.a
LIB := lib$(LIBNAME).a
# 最终库输出目录（比如 ./obj/src/myfile/）
OBJDIR ?= ./obj/myfile
# 最终 .a 文件路径
LIB_PATH := $(OBJDIR)/$(LIB)

# 源文件
SRCS := myfile.c
OBJS := $(SRCS:.c=.o)
OBJS := $(patsubst %.o,$(OBJDIR)/%.o,$(OBJS))

all: $(LIB_PATH)

# 编译规则：生成obj目录下的.o文件
$(OBJDIR)/%.o: %.c
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# 打包静态库规则
$(LIB_PATH): $(OBJS)
	ar rcs $(LIB_PATH) $(OBJS)

clean:
	rm -rf $(OBJDIR)/*.o $(LIB_PATH)
