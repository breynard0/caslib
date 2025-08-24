.SILENT:

# I will write this absolute path, then I will see myself out
# I just want to finish this :(
RAYLIB_FLAGS = -O3 -Wall /home/breynard/git-cache/raylib/src/libraylib.web.a -I/usr/include /home/breynard/git-cache/raylib/src/libraylib.web.a -s USE_GLFW=3 -s ASYNCIFY --shell-file /home/breynard/CProjects/caslib/dist/shell.html

gcc_call = gcc -I./include -I/usr/include/ -g -pg -c $(1) -o $(2).o
#gcc_call = emcc -I./include $(RAYLIB_FLAGS) -g -pg -c $(1) -o $(2).a

run: init build
	echo "Running..."
	./work/main

profile: run
	gprof work/main > profile.txt

init:
	mkdir -p work

build: deps
	echo "Building..."
	@$(call gcc_call,src/debug_algebra.c,work/debug_algebra)
	@$(call gcc_call,src/debug_display.c,work/debug_display)
	@$(call gcc_call,src/debug.c,work/debug)
	@$(call gcc_call,src/main.c,work/main)
	gcc -L/usr/lib64/ -lraylib -g -pg work/*.o -o work/main

	#emcc $(RAYLIB_FLAGS) -o dist/game.html -DPLATFORM_WEB -g -pg work/*.a

deps: raylib auto-deps operations algebra display

algebra:
	@$(call gcc_call,src/algebra/lex.c,work/lex)
	@$(call gcc_call,src/algebra/parse.c,work/parse)
	@$(call gcc_call,src/algebra/solve_consts.c,work/solve_consts)
	@$(call gcc_call,src/algebra/valid.c,work/valid)
	@$(call gcc_call,src/algebra/equation_objects.c,work/equation_objects)
	@$(call gcc_call,src/algebra/expansion.c,work/expansion)
	@$(call gcc_call,src/algebra/collection.c,work/collection)
	@$(call gcc_call,src/algebra/cull.c,work/cull)
	@$(call gcc_call,src/algebra/cauchy.c,work/cauchy)
	@$(call gcc_call,src/algebra/rearrange.c,work/rearrange)
	@$(call gcc_call,src/algebra/evaluate.c,work/evaluate)
	@$(call gcc_call,src/algebra/bundan.c,work/bundan)
	@$(call gcc_call,src/algebra/isolation.c,work/isolation)
	@$(call gcc_call,src/algebra/bisection.c,work/bisection)
	@$(call gcc_call,src/algebra/parentheses.c,work/parentheses)
	@$(call gcc_call,src/algebra/yun.c,work/yun)

operations:
	@$(call gcc_call,src/operations/utils.c,work/utils)
	@$(call gcc_call,src/operations/dutils.c,work/dutils)
	@$(call gcc_call,src/operations/root.c,work/root)
	@$(call gcc_call,src/operations/trig.c,work/trig)
	@$(call gcc_call,src/operations/atrig.c,work/atrig)
	@$(call gcc_call,src/operations/gcf.c,work/gcf)
	@$(call gcc_call,src/operations/flags.c,work/flags)
	@$(call gcc_call,src/operations/pow.c,work/pow)
	@$(call gcc_call,src/operations/fraction.c,work/fraction)
	@$(call gcc_call,src/operations/log.c,work/log)
	@$(call gcc_call,src/operations/derivative.c,work/derivative)
	@$(call gcc_call,src/operations/poly_div.c,work/poly_div)

display:
	@$(call gcc_call,src/display/draw.c,work/draw)
	@$(call gcc_call,src/display/buttons.c,work/buttons)
	@$(call gcc_call,src/display/letters.c,work/letters)
	@$(call gcc_call,src/display/alt_buttons.c,work/alt_buttons)

auto-deps:
	@$(call gcc_call,auto-generated/cordic_constants.c,work/cordic_constants)
	@$(call gcc_call,auto-generated/powers_two.c,work/powers_two)

gen_sources:
	mkdir -p auto-generated
	python scripts/cordic_constants.py
	python scripts/powers_2.py

raylib:
