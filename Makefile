CC = gcc
# 可执行文件
TARGET = network
# C文件
SRCS = Matrix.c main.c network.c
# 目标文件
OBJS = $(SRCS:.c=.o)
# 库文件
DLIBS = -lopencv_core -lopencv_imgproc -lopencv_highgui -lm
# 链接为可执行文件
$(TARGET):$(OBJS)
	$(CC) -o $@ $^ $(DLIBS)  
clean:
	rm -rf $(TARGET) $(OBJS)
# 编译规则 $@代表目标文件 $< 代表第一个依赖文件
%.o:%.c
	$(CC) -o $@ -c $<