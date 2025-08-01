# Makefile for the ConnectedComponents assignment

CXX = g++
CXXFLAGS = -Wall -std=c++20 -g
TARGET = findcomp
TEST_TARGET = runTests

SRCS = main.cpp PGMimage.cpp PGMimageProcessor.cpp ConnectedComponent.cpp
OBJS = $(SRCS:.cpp=.o)

TEST_SRCS = tests.cpp PGMimage.cpp PGMimageProcessor.cpp ConnectedComponent.cpp
TEST_OBJS = $(TEST_SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Build and run tests
tests: $(TEST_TARGET)
	./$(TEST_TARGET)

run: $(TARGET)
	./$(TARGET) -t 35 -m 50 -w output.pgm Birds.pgm -p

$(TEST_TARGET): $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TEST_OBJS) $(TARGET) $(TEST_TARGET) output.pgm

.PHONY: all tests run clean