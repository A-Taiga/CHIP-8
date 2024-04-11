CC = clang++
CCFLAGS = -std=c++20 -O0 -g -Wall -Wextra -I /usr/local/include
CXXFLAG = `pkg-config --libs --cflags sdl2`
OBJDIR = obj/
VPATH = src: obj:
EXENAME = Chip8
SOURCES = main.cpp emulator.cpp window.cpp
OBJECTS := $(SOURCES:%.cpp=$(OBJDIR)%.o)
.PHONY: clean run

all: $(EXENAME)

$(EXENAME): $(OBJECTS)
	@echo linking
	$(CC) $(CCFLAGS) $(CXXFLAG) $^ -o $@
	@echo build success

$(OBJECTS): $(OBJDIR)%.o: %.cpp
	$(CC) $(CCFLAGS) -c $< -o $@

clean:
	-rm $(OBJDIR)*.o $(EXENAME)
run:
	./$(EXENAME)











