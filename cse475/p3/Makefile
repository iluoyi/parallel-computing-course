#
#  Origin:  Lehigh CSE Department
#  Course:  CSE 375/475
#  Date:    2013-09-30
#  Version: 1
#
#  Description: A basic makefile.  Simply typing 'make' should build the
#  program.  Note that typing "BITS=64 make" will build a 64-bit executable
#  instead of a 32-bit executable.
#

#
# This defaults bits to 32, but allows it to be overridden on the command
# line
#
BITS	 ?= 32

#
# Basic compiler configuration and flags
#
CXX	 = g++
CXXFLAGS = -MMD -ggdb -O3 -std=gnu++11 -m$(BITS) 
#-Wall
LDFLAGS	 = -m$(BITS) -lrt -ltbb

#
# Filenames provided by programmer
#
CXXFILES = AIPlayer AIPlayer1 AIPlayer2 AIPlayer3 GameBoard Reversi
TARGET   = reversi

#
# Auto-generated stuff
#
OFILES	 = $(patsubst %, %.o, $(CXXFILES))
DEPS	 = $(patsubst %, %.d, $(CXXFILES))
INCLUDE	 = /proj/spear1/tbb41_20130613oss/include

all: $(TARGET)

clean:
	@echo cleaning up...
	@rm -f $(TARGET) $(OFILES) $(DEPS)

%.o: %.cpp
	@echo [CXX] $< "-->" $@
	@$(CXX) $(CXXFLAGS) -c -o $@ $< -I $(INCLUDE)

$(TARGET): $(OFILES)
	@echo [LD] $^ "-->" $@
	@$(CXX) -o $@ $^ $(LDFLAGS) -I $(INCLUDE)

.PHONY: all clean

-include $(DEPS)
