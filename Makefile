.PHONY: default clean

OBJS = adder-demo grover-demo simple-demo toffoli-demo ts-demo

# https://www.gnu.org/software/make/manual/html_node/Automatic-Variables.html

%: %.c
	$(CC) -Wall -O0 -o $@ $< quantum-gates.c -lm

default:
	for FILE in $(OBJS) ; do make $$FILE ; done

clean:
	rm -f $(OBJS)
