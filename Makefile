CC = gcc
# ��ִ���ļ�
TARGET = network
# C�ļ�
SRCS = Matrix.c main.c network.c
# Ŀ���ļ�
OBJS = $(SRCS:.c=.o)
# ���ļ�
DLIBS = -lopencv_core -lopencv_imgproc -lopencv_highgui -lm
# ����Ϊ��ִ���ļ�
$(TARGET):$(OBJS)
	$(CC) -o $@ $^ $(DLIBS)  
clean:
	rm -rf $(TARGET) $(OBJS)
# ������� $@����Ŀ���ļ� $< �����һ�������ļ�
%.o:%.c
	$(CC) -o $@ -c $<