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
	gcc -c src/sqrt.c -o work/sqrt.o
