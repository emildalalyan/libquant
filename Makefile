WORK_DIR = temp
# All provided paths MUST BE either
# absolute or relative to the working directory.

COMPILER = gcc
# Change it to compile a program
# using different compiler.

OPTIONS = -fopenmp -O3 --std=c17 -lm
# These options will be passed to compiler.
# | Options explanation: ===============================|
# | -fopenmp    | enables OpenMP support.               |
# | -O3         | turns extreme optimizations on.       |
# | --std=c17   | enables C17 support.                  |
# | -lm         | links 'libm' library.                 |
# |=====================================================|

SOURCES = ../*.c ../io/*.c ../effects/*.c ../synth/*.c
# These source files will be compiled into
# complete executable file.
# In CLI syntax '*.c' is all files with '.c' extension.

DEST_DIR = ../build
# Destination directory. It'll be created,
# if it's not exist.

DEST_FILE = libquant
# Destination library file.

all: | $(DEST_DIR) $(WORK_DIR)
# Step 1: Parameters of compilation
	@echo [$@] Destination library file: $(DEST_DIR)/$(DEST_FILE).a
	@echo [$@] Working directory: $(WORK_DIR)
	@echo -------------------------------

# Step 2: Compiling object files.
	@echo [$(COMPILER)] Compiling object files with options: $(OPTIONS)
	@cd $(WORK_DIR) && @$(COMPILER) -c $(SOURCES) $(OPTIONS)
	
# Step 3: Archiving object files to get static library.
	@echo [ar] Archiving object files into $(DEST_FILE).a...
	@cd $(WORK_DIR) && @ar rcs $(DEST_DIR)/$(DEST_FILE).a *.o

# ^^^ We combine commands with cd to be in working directory. ^^^

# Step 4: We're done!
	@echo -------------------------------
	@echo [$@] Compilation is done!
	@echo -------------------------------
	@echo If you going to clear object files, you have to
	@echo do it manually, because different shells have different
	@echo commands for file removing.
	@echo -------------------------------
# END OF "all: | $(DEST_DIR)"

$(DEST_DIR):
# Creating destination directory if it's not exist.
	@echo [$@] Creating destination directory...
	@mkdir "$@"
# END OF "$(DEST_DIR):"

$(WORK_DIR):
# Creating working directory if it's not exist.
	@echo [$@] Creating working directory...
	@mkdir "$@"
# END OF "$(DEST_DIR):"
