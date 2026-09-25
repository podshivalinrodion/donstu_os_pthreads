# Current compiler for work
CC		= clang
# Flags for compiling
# all warnings, additional warnings and 
# debug information
CFLAGS  = -Wall -Wextra -g -pthread
# Name of execution file (result of compilation)
TARGET 	= app
# list of object files for linking (result of compiling too)
OBJS	= main.o pthreadfuncs.o

# log file
OUTPUT  = output.log

# Rule for making
# target: dependencies
#		  command
# here target is app
# depends from objs files
# and command for compilation
# always starts from tab!
# $@  - name of target
# $^ - all dependencies
# &< - first dependence
# -o $@ - path for putting result of linking/compilation
# $^ .o - list all .o-files for linking
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(CFLAGS)

# any file .o you can get from .c-file
# and command for compilation one file
%.o: %.c
# compiler + flags + only compiling + first dependencies
	$(CC) $(CFLAGS) -c $< -o $@

# it's only actions
# it's only actions
clean:
	rm -f $(OBJS) $(TARGET) $(OUTPUT)

run: $(TARGET)
	./$(TARGET)

log:
	cat output.log
pc: producer_consumer.c
	$(CC) $(CFLAGS) -o $@ $<

rb: ring_buffer.c
	$(CC) $(CFLAGS) -o $@ $<
# lay target
.PHONY: clean run log pc rb
