CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra

# Base directory
BASE_DIR = /Users/saadyarao/EmployeeSchedulerCMPSC483W

# Include directories
INCLUDES = -I$(BASE_DIR)/DataStructure/LinkedListDS -I$(BASE_DIR)/DataStructure/PrototypeV2

# Source files
SRCS = main.cpp \
       $(BASE_DIR)/DataStructure/LinkedListDS/CSVParser.cpp \
       $(BASE_DIR)/DataStructure/LinkedListDS/NurseList.cpp \
       $(BASE_DIR)/DataStructure/LinkedListDS/NurseFunctions.cpp

# Output executable name
TARGET = Testfile

# Default target
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

# Clean up
clean:
	rm -f $(TARGET)

.PHONY: all clean