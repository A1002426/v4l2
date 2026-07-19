CC = /opt/atk-dlrk356x-toolchain/bin/aarch64-buildroot-linux-gnu-gcc

TARGET = v4l2

SRCS = main.c $(wildcard src/*.c)

OBJS = $(SRCS:.c=.o)


INC_DIR = ./inc


ION_UAPI = /home/alientek/rk3568_sdk/kernel/drivers/staging/android/uapi


CFLAGS = -Wall -O2 -g
CFLAGS += -I$(INC_DIR)  -I$(ION_UAPI)
CFLAGS += -D_GNU_SOURCE   


# 默认目标
all: $(TARGET)


$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ 

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 清理
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean