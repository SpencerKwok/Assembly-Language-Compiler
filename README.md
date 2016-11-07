# Assembly-Language-Compiler
Welcome to my Assembly Language Compiler!<br>

How to use the compiler:<br>
1. Download the AssemblyLanguage.cpp file<br>
2. Compile the c++ file<br>
3. Run AssemblyLanguage.cpp<br>
4. Enter your assembly language document into the program by entering the file name (including the .txt) before or after starting the program<br>
5. Hit enter<br><br>

List of assembly language instructions are below:<br><br>

LD nnnn, Ri  (data stored in memory location nnnn will be copied to Ri)<br>
LD Ri, Rj (data stored in Ri will be copied to Rj)<br>
LDi nnnn, Ri (integer value nnnn will be copied to Ri)<br>
SD Ri, nnnn (data stored in Ri will be stored in memory location nnnn)<br>
SD Ri, Rj (data stored in Ri will be stored in the memory location stored in Rj)<br>
SDi mmmm, nnnn (integer value mmmm will be stored in memory location nnnn)<br>
SDi mmmm, Ri (integer value mmmm will be stored in Ri)<br><br>

ADD Ri, Rj, Rk (data stored in Ri and Rj will be added together and stored in Rk)<br>
ADDi Ri, nnnn, Rk (data stored in Ri and integer value nnnn will be added together and stored in Rk)<br>
SUB Ri, Rj, Rk (data stored in Ri minus data stored in Rj will be stored in Rk)<br>
SUBi Ri, nnnn, Rk (data stored in Ri minus integer value nnnn will be stored in Rk)<br>
MUL Ri, Rj, Rk (data stored in Ri and Rj will be multiplied together and stored in Rk)<br>
MULi Ri, nnnn, Rk (data stored in Ri and integer value nnnn will be multiplied together and stored in Rk)<br>
DIV Ri, Rj, Rk (data stored in Ri divided by data stored in Rj will be stored in Rk)<br>
DIVi Ri, nnnn, Rk (data stored in Ri divided by integer value nnnn will be stored in Rk)<br><br>

JMP nnnn (instructions will jump to location nnnn in the assembly language)<br>
JZ Ri, nnnn (if Ri == 0, instructions will jump to location nnnn in the assembly language)<br>
JNZ Ri, nnnn (if Ri != 0, instructions will jump to location nnnn in the assembly language)<br>
JGZ Ri, nnnn (if Ri > 0, instructions will jump to location nnnn in the assembly language)<br>
JGEZ Ri, nnnn (if Ri >= 0, instructions will jump to location nnnn in the assembly language)<br>
JLZ Ri, nnnn (if Ri < 0, instructions will jump to location nnnn in the assembly language)<br>
JLEZ Ri, nnnn (if Ri <= 0, instructions will jump to location nnnn in the assembly language)<br><br>

Data: nnnn (Tells compiler where you want memory to start)<br>
Use the symbol, #, to add any comments into the program<br><br>

*Note, only integer values are accepted into the assembly language compiler
