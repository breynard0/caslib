.SILENT:
run: init main
	echo "Running..."
	./work/main

init:
	mkdir -p work

main: deps
	echo "Building..."
	gcc -c src/main.c -o work/main.o
	gcc work/*.o -o work/main

deps:
	gcc -c src/utils.c -o work/utils.o
	gcc -c src/futils.c -o work/futils.o
	gcc -c src/root.c -o work/root.o
	gcc -c src/trig.c -o work/trig.o

gen_sources:
	mkdir -p auto-generated
	python scripts/cordic_constants.py
