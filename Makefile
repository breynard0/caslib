.SILENT:
run: init main
	echo "Running..."
	./work/main

init:
	mkdir -p work

main: deps
	echo "Building..."
	gcc -g -pg -c src/main.c -o work/main.o
	gcc -g -pg work/*.o -o work/main

deps: auto-deps
	gcc -g -pg -c src/utils.c -o work/utils.o
	gcc -g -pg -c src/dutils.c -o work/dutils.o
	gcc -g -pg -c src/root.c -o work/root.o
	gcc -g -pg -c src/trig.c -o work/trig.o
	gcc -g -pg -c src/atrig.c -o work/atrig.o
	gcc -g -pg -c src/gcf.c -o work/gcf.o
	gcc -g -pg -c src/flags.c -o work/flags.o
	gcc -g -pg -c src/pow.c -o work/pow.o
	gcc -g -pg -c src/fraction.c -o work/fraction.o
	gcc -g -pg -c src/log.c -o work/log.o

auto-deps:
	gcc -g -pg -c auto-generated/cordic_constants.c -o work/cordic_constants.o
	gcc -g -pg -c auto-generated/powers_two.c -o work/powers_two.o

gen_sources:
	mkdir -p auto-generated
	python scripts/cordic_constants.py
	python scripts/powers_2.py
