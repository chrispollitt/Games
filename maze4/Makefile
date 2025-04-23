# Makefile for maze race game

# Run arguments
RUN_ARGS ?= -g100 -r2 -m4 -t2 -p0

# Compiler to use
CXX    = g++
CC     = gcc
CATLOG = catlog
SUMLOG = sumlog
TRACEPREFIX = trace
TRACESUFFIX = .perf

# operating system
OS := $(shell uname -s)

# Compiler flags
CFLAGS = -Wall -Wextra -g -Werror -pedantic -std=c17 -Wshadow -Wconversion -Wunused -Wformat=2 
#CFLAGS = -g -Wall -std=c11
ifeq ($(PROFILE),1)
CXXFLAGS = -pg -O0
else
CXXFLAGS = -Wall -Wextra -g -Werror -pedantic -std=c++17 -Wshadow -Wconversion -Wunused -Wformat=2 
#CXXFLAGS = -g -Wall -std=c++11
endif
ifeq ($(DEBUG),1)
CXXFLAGS += -DDEBUG=1
endif

# Linker flags (needed for backtrace_symbols to find names)
ifeq ($(PROFILE),1)
LDFLAGS = -pg
else
LDFLAGS = -rdynamic
endif

# Libraries to link
LIBS = -lncursesw -lm

# Target executable name
TARGET = maze4

# Source files
ifeq ($(TRACE),1)
INSTRUMENT_FLAGS = -finstrument-functions
CSRCS   = trace.c
else
CSRCS   = 
INSTRUMENT_FLAGS = 
endif
CXXSRCS = $(TARGET).cc
SRCS    = $(CSRCS) $(CXXSRCS)

# Object files
COBJS   = $(CSRCS:.c=.o)
CXXOBJS = $(CXXSRCS:.cc=.o)
OBJS    = $(COBJS) $(CXXOBJS)

# Installation directories
PREFIX = /usr/local
BINDIR = $(PREFIX)/bin
MANDIR = $(PREFIX)/share/man/man6
# Default target
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(OBJS) Makefile
	$(CXX) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

# Rule to build object files
trace.o: trace.c
	$(CC) $(CFLAGS) -c trace.c -o $@

%.o: %.cc
	$(CXX) $(CXXFLAGS) $(INSTRUMENT_FLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f *.o $(TARGET) maze4_high_scores.dat maze4_logfile.txt *$(TRACESUFFIX) *.err *.log

# Run the program
run: $(TARGET) $(CATLOG).py
ifeq ($(TRACE),1)
	-rm -f $(CATLOG).log $(TRACEPREFIX)_resolved$(TRACESUFFIX)
	@echo "Running with TRACE=1: starting $(CATLOG) in background..."
	@./$(CATLOG).py $(TARGET) $(TRACEPREFIX)$(TRACESUFFIX) 2> $(CATLOG).log &
endif
ifeq ($(PROFILE),1)
	time ./$(TARGET) $(RUN_ARGS)
else
	./$(TARGET) $(RUN_ARGS)
endif

# show trace
trace: run $(SUMLOG).py
ifeq ($(TRACE),1)
	@echo "Waiting for $(CATLOG) to finish processing..."; \
		while pgrep -f "$(CATLOG).[p]y $(TARGET)" > /dev/null; do \
			sleep 0.5; \
		done; \
		sleep 0.5; \
		echo "Log processing complete."
	-rm -f $(SUMLOG).log
	./$(SUMLOG).py $(TRACEPREFIX)_resolved$(TRACESUFFIX) > $(TRACEPREFIX)_calltree$(TRACESUFFIX) 2> $(SUMLOG).log
	less -iS $(TRACEPREFIX)_calltree$(TRACESUFFIX)
endif

profile: run
ifeq ($(PROFILE),1)
	gprof ./$(TARGET) gmon.out > analysis.txt
	cat analysis.txt
endif

perf: $(TARGET)
	perf record -g -o perf.data ./$(TARGET) $(RUN_ARGS)
	perf report -g 'graph,0.5,caller'

# static analysis target
lint:
	clang-tidy $(SRCS) -- 

format:
	clang-format -i $(SRCS)

# Install target
install: $(TARGET) maze4.6
	install -d $(BINDIR)
	install -m 755 $(TARGET) $(BINDIR)
	install -d $(MANDIR)
	install -m 644 maze4.6 $(MANDIR)/maze4.6

# Uninstall target
uninstall:
	rm -f $(BINDIR)/$(TARGET)
	rm -f $(MANDIR)/maze4.6

# Phony targets
.PHONY: all clean run trace lint install uninstall
.PRECIOUS: %.o %.cc %.i %.ii
