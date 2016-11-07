# Assembly-Language-Compiler
Welcome to my Assembly Language Compiler!

How to use the compiler:
1. Download the AssemblyLanguage.cpp file
2. Compile the c++ file
3. Run AssemblyLanguage.cpp
4. Enter your assembly language document into the program by entering the file name (including the .txt) before or after starting the program
5. Hit enter

List of assembly language instructions are below:

LD nnnn, Ri  (data stored in memory location nnnn will be copied to Ri)
LD Ri, Rj (data stored in Ri will be copied to Rj)
LDi nnnn, Ri (integer value nnnn will be copied to Ri)
SD Ri, nnnn (data stored in Ri will be stored in memory location nnnn)
SD Ri, Rj (data stored in Ri will be stored in the memory location stored in Rj)
SDi mmmm, nnnn (integer value mmmm will be stored in memory location nnnn)
SDi mmmm, Ri (integer value mmmm will be stored in Ri)

ADD Ri, Rj, Rk (data stored in Ri and Rj will be added together and stored in Rk)
ADDi Ri, nnnn, Rk (data stored in Ri and integer value nnnn will be added together and stored in Rk)
SUB Ri, Rj, Rk (data stored in Ri minus data stored in Rj will be stored in Rk)
SUBi Ri, nnnn, Rk (data stored in Ri minus integer value nnnn will be stored in Rk)
MUL Ri, Rj, Rk (data stored in Ri and Rj will be multiplied together and stored in Rk)
MULi Ri, nnnn, Rk (data stored in Ri and integer value nnnn will be multiplied together and stored in Rk)
DIV Ri, Rj, Rk (data stored in Ri divided by data stored in Rj will be stored in Rk)
DIVi Ri, nnnn, Rk (data stored in Ri divided by integer value nnnn will be stored in Rk)

JMP nnnn (instructions will jump to location nnnn in the assembly language)
JZ Ri, nnnn (if Ri == 0, instructions will jump to location nnnn in the assembly language)
JNZ Ri, nnnn (if Ri != 0, instructions will jump to location nnnn in the assembly language)
JGZ Ri, nnnn (if Ri > 0, instructions will jump to location nnnn in the assembly language)
JGEZ Ri, nnnn (if Ri >= 0, instructions will jump to location nnnn in the assembly language)
JLZ Ri, nnnn (if Ri < 0, instructions will jump to location nnnn in the assembly language)
JLEZ Ri, nnnn (if Ri <= 0, instructions will jump to location nnnn in the assembly language)

Data: nnnn (Tells compiler where you want memory to start)

Use the symbol, #, to add any comments into the program

*Note, only integer values are accepted into the assembly language compiler
