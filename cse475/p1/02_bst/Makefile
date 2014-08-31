#
#  Origin:  Lehigh CSE Department
#  Course:  CSE 375/475
#  Date:    2013-08-27
#  Version: 1
#
#  Description: A basic makefile.  Simply typing 'make' should build the
#  program.  Note that typing "BITS=64 make" will build a 64-bit executable
#  instead of a 32-bit executable.
#
#  For the purposes of the assignment, this file should be treated as
#  read-only.
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
LDFLAGS	 = -m$(BITS)

#
# The basenames of the cpp files that this program uses
#
CXXFILES = p1 tests map_t config_t

#
# The executable we will build
#
TARGET	 = p1

#
# Create the .o names from the CXXFILES
#
OFILES	 = $(patsubst %, %.o, $(CXXFILES))

#
# Create .d files to store dependency information, so that we don't need to
# clean every time before running make
#
DEPS	 = $(patsubst %, %.d, $(CXXFILES))

#
# Default rule builds the executable
#
all: $(TARGET)

#
# clean up everything... there's no point in partial cleanups.
#
clean:
	@echo cleaning up...
	@rm -f $(TARGET) $(OFILES) $(DEPS)

#
# Autorule for building .o files from .cpp files
#
%.o: %.cpp
	@echo [CXX] $< "-->" $@
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

#
# Link rule for building the target from .o files
#
$(TARGET): $(OFILES)
	@echo [LD] $^ "-->" $@
	@$(CXX) -o $@ $^ $(LDFLAGS)

#
# Remember that 'all' and 'clean' aren't real targets
#
.PHONY: all clean

#
# Pull in all dependencies
#
-include $(DEPS)
