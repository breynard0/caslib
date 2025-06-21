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

deps: auto-deps
	gcc -c src/utils.c -o work/utils.o
	gcc -c src/dutils.c -o work/dutils.o
	gcc -c src/root.c -o work/root.o
	gcc -c src/trig.c -o work/trig.o
	gcc -c src/atrig.c -o work/atrig.o
	gcc -c src/gcf.c -o work/gcf.o
	gcc -c src/flags.c -o work/flags.o

auto-deps:
	gcc -c auto-generated/cordic_constants.c -o work/cordic_constants.o

gen_sources:
	mkdir -p auto-generated
	python scripts/cordic_constants.py
