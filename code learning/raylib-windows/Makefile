program: compile run clean

compile:
	@gcc main.c -Wall -Wextra -pedantic -Iinclude -Llib -lraylib -lgdi32 -lwinmm -lopengl32 -lshell32 -o program.exe

run:
	@./program

clean:
	@del program.exe
