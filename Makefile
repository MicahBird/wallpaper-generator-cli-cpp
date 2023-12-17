# COMMENTS BEGIN WITH A HASH

# THE NAME OF YOUR EXECUTABLE
TARGET = wallpaper-generator-cli
# ALL CPP COMPILABLE IMPLEMENTATION FILES THAT MAKE UP THE PROJECT
SRC_FILES = main.cpp Wallpaper.cpp lodepng.cpp

CXX = g++
CXXFLAGS = -O3 -Wall -Wextra -Werror -pedantic-errors -flto -funroll-loops -Ofast -ffast-math -march=native -mtune=native -fomit-frame-pointer -funroll-loops 
# CXXFLAGS_DEBUG = -g
CPPVERSION = -std=c++17

OBJECTS = $(SRC_FILES:.cpp=.o)

ifeq ($(shell echo "Windows"), "Windows")
	TARGET := $(TARGET).exe
	DEL = del
	Q = 
else
	DEL = rm -f
	Q = "
endif

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) -o $@ $^

.cpp.o:
	$(CXX) $(CXXFLAGS) $(CPPVERSION) $(CXXFLAGS_DEBUG) $(CXXFLAGS_ERRORS) -o $@ -c $<

clean:
	$(DEL) -f $(TARGET) $(OBJECTS) Makefile.bak

depend:
	@sed -i.bak '/^# DEPENDENCIES/,$$d' Makefile
	@$(DEL) sed*
	@echo $(Q)# DEPENDENCIES$(Q) >> Makefile
	@$(CXX) -MM $(SRC_FILES) >> Makefile

.PHONY: all clean depend

# DEPENDENCIES
main.o: main.cpp Wallpaper.h CLI11.hpp
Wallpaper.o: Wallpaper.cpp Wallpaper.h lodepng.h
lodepng.o: lodepng.cpp lodepng.h
