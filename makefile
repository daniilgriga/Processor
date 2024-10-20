all: assm proc

assm:
	make -f makefile_assm

proc:
	make -f makefile_proc

clean:
	rm -f build/*.o $(EXECUTABLE)
