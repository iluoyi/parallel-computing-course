#
#  Origin:  Lehigh CSE Department
#  Course:  CSE 375/475
#  Date:    2013-10-21
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
CXXFLAGS = -MMD -ggdb -O3 -std=gnu++11 -m$(BITS) -Wall -march=native -mtune=native
LDFLAGS	 = -m$(BITS) -lrt

#
# Filenames provided by programmer
#
CXXFILES = bmharness ListBench
TARGET   = p4

#
# Auto-generated stuff
#
OFILES	 = $(patsubst %, %.o, $(CXXFILES))
DEPS	 = $(patsubst %, %.d, $(CXXFILES))

all: $(TARGET)

clean:
	@echo cleaning up...
	@rm -f $(TARGET) $(OFILES) $(DEPS)

%.o: %.cpp
	@echo [CXX] $< "-->" $@
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

$(TARGET): $(OFILES)
	@echo [LD] $^ "-->" $@
	@$(CXX) -o $@ $^ $(LDFLAGS)

.PHONY: all clean

-include $(DEPS)
