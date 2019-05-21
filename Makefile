src = $(wildcard src/*.c)
obj = $(src:.c=.o)
dep = $(obj:.o=.d)  # one dependency file for each source

LDFLAGS = -lGL -lpng -lz -lm -lrt
THREAD_FLAGS = -pthread
WARNING_FLAGS = -Wall -Wextra

c-hat: $(obj)
		$(CC) -o $@ $^ $(LDFLAGS) $(THREAD_FLAGS)

-include $(dep) # include all dep files in the makefile

# rule to generate a dep file by using the C preprocessor
# (see man cpp for details on the -MM and -MT options)
%.d: %.c
		@$(CPP) $(CFLAGS) $< -MM -MT $(@:.d=.o) >$@

.PHONY: clean
clean:
		rm -f $(obj) c-hat

.PHONY: cleandep
cleandep:
		rm -f $(dep)