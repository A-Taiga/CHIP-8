CC = clang++
CCFLAGS = -std=c++20 -O0 -g -Wall -Wextra -I /usr/local/include
CXXFLAG = `pkg-config --libs --cflags sdl2`
OBJDIR = obj/
SRCDIR = src/
VPATH = src: obj:
EXENAME = Chip8
SOURCES = main.cpp emulator.cpp window.cpp
OBJECTS := $(SOURCES:.cpp=.o)

.PHONY: clean run
all:: $(EXENAME)

$(EXENAME): $(OBJECTS)
	@echo linking
	$(CC) $(CCFLAGS) $(CXXFLAG)  -I /usr/local/include $^ -o $@
	@echo build success

$(OBJDIR)main.o: main.cpp emulator.hpp
	@echo building $@
	$(CC) $(CCFLAGS) -c $< -o $@

$(OBJDIR)emulator.o: emulator.cpp emulator.hpp window.hpp
	@echo building $@
	$(CC) $(CCFLAGS) -c $< -o $@

$(OBJDIR)window.o: window.cpp window.hpp
	@echo building $@
	$(CC) $(CCFLAGS) -c $< -o $@

clean:
	-rm $(OBJDIR)*.o $(EXENAME)
run:
	./$(EXENAME)











