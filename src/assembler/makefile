-include ../makefile.inc

BUILD=../../build
TESTS=../../tests

CMD=$(BUILD)/asm 

run:
	$(CMD) $(TESTS)/math_exmpl.asm $(TESTS)/math_exmpl.bin

circle:
	$(CMD) $(TESTS)/circle.asm $(TESTS)/circle.bin

squaresolver:
	$(CMD) $(TESTS)/square_solver.asm $(TESTS)/square_solver.bin

factorial:
	$(CMD) $(TESTS)/factorial.asm $(TESTS)/factorial.bin	

build:
	gcc assembler.c $(CCFLAGS) -g -o $(CMD)
