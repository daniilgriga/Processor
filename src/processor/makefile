-include ../makefile.inc

BUILD=../../build
TESTS=../../tests

SOURCES_LIST = processor.c stack.c
CMD=$(BUILD)/proc

run:
	$(CMD) $(TESTS)/math_exmpl.bin

circle:
	$(CMD) $(TESTS)/circle.bin

squaresolver:
	$(CMD) $(TESTS)/square_solver.bin

factorial:
	$(CMD) $(TESTS)/factorial.bin

build:
	gcc $(SOURCES_LIST) -lm $(CCFLAGS) -o $(CMD)
