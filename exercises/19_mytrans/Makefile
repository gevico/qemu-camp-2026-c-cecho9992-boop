CC = gcc
CFLAGS = -Wall -g -O2

TARGET = 19_mytrans
SRCS = 19_mytrans.c mytrans.c myhash.c
OBJS := $(SRCS:.c=.o)

all: $(TARGET)

# 编译所有.o目标文件（自动规则，可选，也可省略）
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 链接所有obj生成可执行程序（核心缺失代码）
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
