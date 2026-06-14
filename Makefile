CXX = clang++
CXXFLAGS = -Wall -Wextra -std=c++23

TARGET = ls

ifeq ($(OS),Windows_NT)
    TARGET := ls.exe
endif

build:
	$(CXX) $(CXXFLAGS) ls.cpp -o $(TARGET)

