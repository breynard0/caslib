.SILENT:

OPT_LEVEL = -O0
#PLATFORM = -m32
PLATFORM =

# I will write this absolute path, then I will see myself out
# I just want to finish this :(
RAYLIB_FLAGS = $(OPT_LEVEL) -Wall /home/breynard/git-cache/raylib/src/libraylib.web.a -I/usr/include /home/breynard/git-cache/raylib/src/libraylib.web.a -s USE_GLFW=3 -s STACK_SIZE=4MB -s ASYNCIFY --shell-file /home/breynard/CProjects/caslib/dist/shell.html

 gcc_call = gcc -I./include -I/usr/include/ $(OPT_LEVEL) $(3) $(PLATFORM) -g -pg -c $(1) -o $(2).o
#gcc_call = emcc -I./include $(RAYLIB_FLAGS) -g -pg -c $(1) -o $(2).a

run: build
	echo "Running..."
	./work/main

profile: run
	gprof work/main > profile.txt

init:
	mkdir -p work
	rm work/* || true

build: deps
	echo "Building..."
	@$(call gcc_call,src/debug_algebra.c,work/debug_algebra)
	@$(call gcc_call,src/debug_display.c,work/debug_display)
	@$(call gcc_call,src/debug.c,work/debug)
	@$(call gcc_call,src/main.c,work/main)
	#gcc -m32 -g -pg work/main.o work/debug.o work/debug_algebra.o work/caslib.o $(OPT_LEVEL) -o work/main
	gcc -L/usr/lib64/ -lraylib $(PLATFORM) -g -pg work/main.o work/debug.o work/debug_algebra.o work/debug_display.o work/caslib.o $(OPT_LEVEL) -o work/main


webbuild: deps
	echo "Building..."
	@$(call gcc_call,src/debug_algebra.c,work/debug_algebra)
	@$(call gcc_call,src/debug_display.c,work/debug_display)
	@$(call gcc_call,src/debug.c,work/debug)
	@$(call gcc_call,src/main.c,work/main)
	emcc $(RAYLIB_FLAGS) -o dist/game.html -DPLATFORM_WEB -g -pg work/*.a

deps: library

library: init auto-deps operations algebra display
	ld --relocatable work/*.o -o work/caslib.o
	#ld -m elf_i386 --relocatable work/*.o -o work/caslib.o

algebra:
	@$(call gcc_call,src/algebra/equation_objects.c,work/equation_objects,-nostdlib)
	@$(call gcc_call,src/algebra/solve_consts.c,work/solve_consts,-nostdlib)
	@$(call gcc_call,src/algebra/parentheses.c,work/parentheses,-nostdlib)
	@$(call gcc_call,src/algebra/collection.c,work/collection,-nostdlib)
	@$(call gcc_call,src/algebra/expansion.c,work/expansion,-nostdlib)
	@$(call gcc_call,src/algebra/rearrange.c,work/rearrange,-nostdlib)
	@$(call gcc_call,src/algebra/bisection.c,work/bisection,-nostdlib)
	@$(call gcc_call,src/algebra/isolation.c,work/isolation,-nostdlib)
	@$(call gcc_call,src/algebra/evaluate.c,work/evaluate,-nostdlib)
	@$(call gcc_call,src/algebra/valid.c,work/valid,-nostdlib)
	@$(call gcc_call,src/algebra/parse.c,work/parse,-nostdlib)
	@$(call gcc_call,src/algebra/cauchy.c,work/cauchy,-nostdlib)
	@$(call gcc_call,src/algebra/bundan.c,work/bundan,-nostdlib)
	@$(call gcc_call,src/algebra/cull.c,work/cull,-nostdlib)
	@$(call gcc_call,src/algebra/yun.c,work/yun,-nostdlib)
	@$(call gcc_call,src/algebra/lex.c,work/lex,-nostdlib)

operations:
	@$(call gcc_call,src/operations/derivative.c,work/derivative,-nostdlib)
	@$(call gcc_call,src/operations/poly_div.c,work/poly_div,-nostdlib)
	@$(call gcc_call,src/operations/fraction.c,work/fraction,-nostdlib)
	@$(call gcc_call,src/operations/dutils.c,work/dutils,-nostdlib)
	@$(call gcc_call,src/operations/utils.c,work/utils,-nostdlib)
	@$(call gcc_call,src/operations/atrig.c,work/atrig,-nostdlib)
	@$(call gcc_call,src/operations/flags.c,work/flags,-nostdlib)
	@$(call gcc_call,src/operations/root.c,work/root,-nostdlib)
	@$(call gcc_call,src/operations/trig.c,work/trig,-nostdlib)
	@$(call gcc_call,src/operations/gcf.c,work/gcf,-nostdlib)
	@$(call gcc_call,src/operations/pow.c,work/pow,-nostdlib)
	@$(call gcc_call,src/operations/log.c,work/log,-nostdlib)

display:
	@$(call gcc_call,src/display/draw.c,work/draw,-nostdlib)
	@$(call gcc_call,src/display/buttons.c,work/buttons,-nostdlib)
	@$(call gcc_call,src/display/letters.c,work/letters,-nostdlib)
	@$(call gcc_call,src/display/alt_buttons.c,work/alt_buttons,-nostdlib)

auto-deps:
	@$(call gcc_call,auto-generated/cordic_constants.c,work/cordic_constants,-nostdlib)
	@$(call gcc_call,auto-generated/powers_two.c,work/powers_two,-nostdlib)

gen_sources:
	mkdir -p auto-generated
	python scripts/cordic_constants.py
	python scripts/powers_2.py
