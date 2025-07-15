.SILENT:

# define gcc_call
# 	$(eval $@_SOURCE = $(1))
# 	$(eval $@_DEST = $(2))
# 	gcc -g -pg -c ${@_SOURCE} -o ${@_DEST}
# endef
gcc_call = gcc -I./include -g -pg -c $(1) -o $(2)
	
run: init build
	echo "Running..."
	./work/main
	
profile: run
	gprof work/main > profile.txt

init:
	mkdir -p work

build: deps
	echo "Building..."
	@$(call gcc_call,src/debug_algebra.c,work/debug_algebra.o)
	@$(call gcc_call,src/debug.c,work/debug.o)
	@$(call gcc_call,src/main.c,work/main.o)
	gcc -g -pg work/*.o -o work/main

deps: auto-deps operations algebra
	
algebra:
	@$(call gcc_call,src/algebra/lex.c,work/lex.o)
	@$(call gcc_call,src/algebra/parse.c,work/parse.o)
	@$(call gcc_call,src/algebra/solve_consts.c,work/solve_consts.o)
	@$(call gcc_call,src/algebra/valid.c,work/valid.o)
	@$(call gcc_call,src/algebra/equation_objects.c,work/equation_objects.o)
	@$(call gcc_call,src/algebra/expansion.c,work/expansion.o)
	@$(call gcc_call,src/algebra/collection.c,work/collection.o)
	@$(call gcc_call,src/algebra/cull.c,work/cull.o)
	@$(call gcc_call,src/algebra/cauchy.c,work/cauchy.o)
	@$(call gcc_call,src/algebra/rearrange.c,work/rearrange.o)
	
operations:
	@$(call gcc_call,src/operations/utils.c,work/utils.o)
	@$(call gcc_call,src/operations/dutils.c,work/dutils.o)
	@$(call gcc_call,src/operations/root.c,work/root.o)
	@$(call gcc_call,src/operations/trig.c,work/trig.o)
	@$(call gcc_call,src/operations/atrig.c,work/atrig.o)
	@$(call gcc_call,src/operations/gcf.c,work/gcf.o)
	@$(call gcc_call,src/operations/flags.c,work/flags.o)
	@$(call gcc_call,src/operations/pow.c,work/pow.o)
	@$(call gcc_call,src/operations/fraction.c,work/fraction.o)
	@$(call gcc_call,src/operations/log.c,work/log.o)

auto-deps:
	@$(call gcc_call,auto-generated/cordic_constants.c,work/cordic_constants.o)
	@$(call gcc_call,auto-generated/powers_two.c,work/powers_two.o)

gen_sources:
	mkdir -p auto-generated
	python scripts/cordic_constants.py
	python scripts/powers_2.py
