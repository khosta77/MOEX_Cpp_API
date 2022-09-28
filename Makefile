TARGET=./main
CC=g++
SRCS=\
	./main.cpp
LIBS=\
    -lboost_system \
    -pthread

#WALL=\
#    -Wall \
#    -Wextra \
#    -Werror \
#    -ansi
SVN=17  # Версия cpp

all: clean $(TARGET)

$(TARGET):
	$(CC) -std=c++$(SVN) -lm -o $(TARGET) $(addprefix -I,$(HDRS)) $(CFLAGS) $(SRCS) $(LIBS)

build: $(TARGET)

clean:
	rm -rf $(TARGET)