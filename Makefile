.PHONY: default clean

OBJS := $(shell basename -a -s .c *.c)

# https://www.gnu.org/software/make/manual/html_node/Automatic-Variables.html

%: %.c quantum-gates.h
	$(CC) -Wall -O0 -o $@ $< -lm

default:
	for FILE in $(OBJS) ; do make $$FILE ; done

clean:
	rm -f a.out $(OBJS)
