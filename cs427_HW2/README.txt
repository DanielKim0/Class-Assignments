Files:
comparer.cpp, comparer.hpp, comparer.o: Files containing the Comparer class that I wrote for this homework.
datapack.cpp, datapack.hpp, datapack.o: Files containing the DataPack class with pre-written code and some modules I added for the purpose of this exercise,
tools.cpp, tools.hpp, tools.o: Files containing pre-written tools.
main.cpp: File containing the main function of the entire program.
Makefile: File that makes the executable for this program.
comparer: Executable program to run.

How to run the code:
First, run make in the submission directory.
Then, run the executable file named comparer.

Design choices:
I encapsulated the merge functionality into a new class because I wanted to leave code that makes a new DataPack in an entirely new class to reduce confusion, instead of creating new DataPacks from DataPacks themselves. I left as much code that accesses or deals with the DataPack within the DataPack itself, instead of leaving it in the Comparer, and gave the Comparer as little information as possible to complete the task. For example, I wrote two public functions, next and append, to access the next value in a DataPack and to add a new value to a DataPack, respectively. This approach is better than, for example, passing in a pointer to the DataPack's numerical data because the Comparer doesn't need all of that information, and the Comparer accessing too much information could pose a danger to the program.
