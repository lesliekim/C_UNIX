CC := g++
PREFIX = .
INCS :=  $(PREFIX)
MY_FLAG = -DDEBUG
CFLAGS := $(MY_FLAG) -Wall -Wunreachable-code -Werror -Wsign-compare -g -fPIC -std=c++11
LIBS = -lreadline -lncurses
SOURCE := $(wildcard  *.cpp)
OBJS := $(patsubst %.cpp, %.o, $(SOURCE))
TARGETS=main

all:$(TARGETS)
$(TARGETS) : $(OBJS)
	$(CC) -o $@ $^ $(LIBS) \
		-Wl,-rpath,/usr/lib/gcc/x86_64-linux-gnu/4.7.3/,-rpath,$(PREFIX)/lib
%.o:%.cpp	
	$(CC) $(CFLAGS) $(addprefix -I,$(INCS)) -c $< -o $@
clean:
	rm -rf $(TARGETS) $(OBJS)
