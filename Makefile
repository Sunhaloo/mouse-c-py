program: compile run clean

compile:
	@gcc main.c -Wall -Wextra -lgdi32 -o program

run:
	@./program

clean:
	@del program.exe
