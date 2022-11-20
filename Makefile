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
#IL=-I /Users/stepanfilimonov/boost-iosx/boost -L /Users/stepanfilimonov/boost-iosx/boost/stage/lib
# WER=-Wall -Wextra -Werror -ansi

all: clean $(TARGET)

$(TARGET):
	$(CC) $(STD) $(LIBS) -o $(TARGET) $(SRCS)

build: $(TARGET)

clean:
	rm -rf $(TARGET)

t1: # тест внутри класса
	$(CC) $(STD) $(LIBS) -o $(TARGET) $(SRCS) -D TEST_METHODS=1

t2: # тест снаружи класса
	$(CC) $(STD) $(LIBS) -o $(TARGET) $(SRCS) -D TEST_CLASS=1 
