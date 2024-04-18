EXENAME = Chip8
CXX = clang++
CXXFLAGS = -std=c++20 -g -Wall -Wextra -Werror -Wformat -I imgui -I src/include
UNAME_S := $(shell uname -s)
VPATH = src: imgui:
OBJ_DIR = obj/
SOURCES = main.cpp emulator.cpp window.cpp event_handler.cpp imgui.cpp imgui_demo.cpp imgui_draw.cpp imgui_tables.cpp
SOURCES += imgui_widgets.cpp imgui_impl_sdl2.cpp imgui_impl_sdlrenderer2.cpp
OBJECTS = $(SOURCES:%.cpp=$(OBJ_DIR)%.o)
LIBS = 

ifeq ($(UNAME_S), Darwin)
	ECHO_MESSAGE = "OS X compiled"
	LIBS += -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo `sdl2-config --libs`
	LIBS += -L/usr/local/lib -L/opt/local/lib
	CXXFLAGS += `sdl2-config --cflags`
	CXXFLAGS += -I/usr/local/include -I/opt/local/include
endif


all: $(EXENAME)
	@echo $(ECHO_MESSAGE)

$(EXENAME): $(OBJECTS)
	$(CXX) -o $@ $^ $(CXXFLAG) $(LIBS)

$(OBJECTS): $(OBJ_DIR)%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

.PHONY: clean run

run:
	./Chip8

clean:
	rm -f $(EXENAME) $(OBJECTS) imgui.ini
















