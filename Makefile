COMPILER = gcc
# Change it to compile a program
# using different compiler.

OPTIONS = -fopenmp -O3 --std=c17 -lm -shared
# These options will be passed to compiler.
# | Options explanation: ===============================|
# | -fopenmp	| enables OpenMP support.		|
# | -O3		| turns extreme optimizations on.	|
# | --std=c17	| enables C17 support.			|
# | -lm		| links 'libm' library to executable.	|
# |=====================================================|

SOURCES = *.c io/*.c effects/*.c synth/*.c
# These source files will be compiled into
# complete executable file.
# In CLI syntax '*.c' is all files with '.c' extension.

DEST_DIR = build
# Destination directory. It'll be created,
# if it's not exist.

DEST_FILE = quant
# Destination executable file.

all: | $(DEST_DIR)
	@echo [$(COMPILER)] Compiling with options: $(OPTIONS)
	@$(COMPILER) $(SOURCES) -o $(DEST_DIR)/$(DEST_FILE) $(OPTIONS)

	@echo -------------------------------
	@echo [$@] Compilation is done!
	@echo -------------------------------
# END OF "all: | $(DEST_DIR)"

$(DEST_DIR):
# Creating directory if it's not exist.
	@echo [$@] Creating directory...
	@mkdir $@
# END OF "$(DEST_DIR):"
