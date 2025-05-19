CC = gcc
CFLAGS = -Wall -g -O0 -Iinclude
SRCDIR = src
BUILDDIR = build
TARGET = mysh

ifeq ($(OS),Windows_NT)
	EXT = .exe
else
	EXT =
endif

TARGET_BIN = $(TARGET)$(EXT)

SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(SRCS))

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(TARGET_BIN)

$(TARGET_BIN): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

all: $(TARGET)

clean:
	rm -rf $(BUILDDIR) $(TARGET)$(EXT)

.PHONY: all clean