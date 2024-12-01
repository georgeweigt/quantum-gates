.PHONY: default clean

# https://www.gnu.org/software/make/manual/html_node/Automatic-Variables.html

%: %.c quantum-gates.h
	$(CC) -Wall -O0 -o $@ $<

default:
	for FILE in $$(basename -a -s .c *.c) ; do make $$FILE ; done

clean:
	rm -f a.out $$(basename -a -s .c *.c)
