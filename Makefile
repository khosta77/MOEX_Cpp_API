TARGET=./main
CC=g++
SRCS=\
	./main.cpp \
	./utilities.cpp
LIBS=\
    -lboost_system \
    -pthread \
    -lc++

STD=-std=c++17  # Версия cpp
IL=-I /Users/stepanfilimonov/boost-iosx/boost -L /Users/stepanfilimonov/boost-iosx/boost/stage/lib
# WER=-Wall -Wextra -Werror -ansi

all: clean $(TARGET)

$(TARGET):
	$(CC) $(STD) $(IL) $(LIBS) -o $(TARGET) $(SRCS)

build: $(TARGET)

clean:
	rm -rf $(TARGET)
