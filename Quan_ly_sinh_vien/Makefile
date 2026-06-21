CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic
SRC = src/main.cpp src/DataStore.cpp src/Services.cpp src/GuiGenerator.cpp src/Ui.cpp
TESTSRC = tests/test_runner.cpp src/Services.cpp

all: student_manager

student_manager: $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o student_manager

test_runner: $(TESTSRC)
	$(CXX) $(CXXFLAGS) $(TESTSRC) -o test_runner

test: test_runner
	./test_runner

clean:
	rm -f student_manager test_runner

export-gui: student_manager
	printf "11\n0\n" | ./student_manager
