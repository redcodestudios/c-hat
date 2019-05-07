src = $(wildcard src/*.c)
obj = $(src:.c=.o)
dep = $(obj:.o=.d)  # one dependency file for each source

LDFLAGS = -lGL -lpng -lz -lm -lrt

c-hat: $(obj)
		$(CC) -o $@ $^ $(LDFLAGS)

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