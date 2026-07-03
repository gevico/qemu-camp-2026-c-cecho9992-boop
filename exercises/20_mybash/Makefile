# =============================================
# 主 Makefile
# 功能：
#   - mybash：生成二进制，放到 bin/
#   - 其他模块（mysed, myfile, mywc, mytrans）：生成静态库 .a，放到 obj/xxx/，.o 放到 obj/xxx/（或 out/，但建议统一）
#   - 所有子模块 Makefile 应支持 OBJDIR 和 BINDIR 参数
# =============================================

PWD := $(shell pwd)
# 全局目录定义
OBJDIR := $(PWD)/obj
BINDIR := $(PWD)/bin
INCDIR := $(PWD)/include

# 所有子模块目录（静态库：生成 .a）
SUBDIRS := \
	mysed \
	myfile \
	mywc \
	mytrans

# 各模块静态库路径
LIBS := \
	$(OBJDIR)/mysed/libmysed.a \
	$(OBJDIR)/myfile/libmyfile.a \
	$(OBJDIR)/mywc/libmywc.a \
	$(OBJDIR)/mytrans/libmytrans.a

# 主程序源码路径
MAIN_SRC := $(PWD)/src/mybash/main.c

# 默认目标：构建 mybash（依赖其他库）
all: mybash

# 通用规则：构建每个子模块（静态库）
$(SUBDIRS):
	mkdir -p $(OBJDIR)/$@
	$(MAKE) -C $(PWD)/src/$@ OBJDIR=$(OBJDIR)/$@ INCDIR=$(INCDIR)

# 构建 mybash（可执行文件，依赖上面的库）
mybash: $(SUBDIRS)
	mkdir -p $(BINDIR)
	gcc $(MAIN_SRC) -I$(INCDIR) $(LIBS) -o $(BINDIR)/mybash

# 清理所有生成的文件
clean:
	rm -rf $(OBJDIR) $(BINDIR)
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $(PWD)/src/$$dir clean OBJDIR=$(OBJDIR)/$$dir; \
	done

.PHONY: all $(SUBDIRS) mybash clean
