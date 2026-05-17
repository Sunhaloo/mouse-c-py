# remote - GitHub Actions ( compiling the `main.c` file )
build:
	@gcc main.c -Wall -Wextra -pedantic -lgdi32 -lpathcch -o program.exe

# local development ( running and testing )
program: compile run clean

# compile the program
compile:
	@gcc main.c -Wall -Wextra -pedantic -lgdi32 -lpathcch -o program.exe

# run the `program.exe` executable file
run:
	@./program

# remove / clean the `program.exe` executable file
clean:
	@del program.exe
