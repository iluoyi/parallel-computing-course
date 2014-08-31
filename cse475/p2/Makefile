#
#  Origin:  Lehigh CSE Department
#  Course:  CSE 375/475
#  Date:    2013-09-11
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
LDFLAGS	 = -m$(BITS) -lrt -ltbb

#
# The basenames of the cpp files that this program uses
#
CXXFILES = p2 ocean config_t

#
# The executable we will build
#
TARGET	 = p2

#
# Create the .o names from the CXXFILES
#
OFILES	 = $(patsubst %, %.o, $(CXXFILES))

#
# Create .d files to store dependency information, so that we don't need to
# clean every time before running make
#
DEPS	 = $(patsubst %, %.d, $(CXXFILES))
INCLUDE	 = /proj/spear1/tbb41_20130613oss/include

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
	@$(CXX) $(CXXFLAGS) -c -o $@ $< -I $(INCLUDE)

#
# Link rule for building the target from .o files
#
$(TARGET): $(OFILES)
	@echo [LD] $^ "-->" $@
	@$(CXX) -o $@ $^ $(LDFLAGS) -I $(INCLUDE)

#
# Remember that 'all' and 'clean' aren't real targets
#
.PHONY: all clean

#
# Pull in all dependencies
#
-include $(DEPS)
