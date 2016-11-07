#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdio>
int getAndRunFile(const char*);
int openFile(char[]);
int validateDocument(int, char[]);
char* getLine(int, char[], char[]);
bool validate(char*, int, int*, int*, int*, int*);
bool validRegister(char*, int);
bool validNumber(char*, int, bool);
bool validWord(char*, int);
int currentValue(char*, int);
int currentRegister(char*, int);
int runCode(char [], int, char[], int);
bool load(int(&)[9], char*, int(&)[10000], int);
bool store(int(&)[9], char*, int(&)[10000], int);
bool sub(int(&)[9], char*);
bool add(int(&)[9], char*);
bool mul(int(&)[9], char*);
bool div(int(&)[9], char*);
bool jmpWord(char*, char [], int*, int);
bool jmpNumber(char [], int*, int);
bool jmpZero(int [], char*, char [], int*, int, bool);
bool jmpNotZero(int [], char*, char [], int*, int, bool);
bool jmpGreaterZero(int [], char*, char [], int*, int, bool);
bool jmpGreaterEqualZero(int [], char*, char [], int*, int, bool);
bool jmpLessZero(int [], char*, char [], int*, int, bool);
bool jmpLessEqualZero(int [], char*, char [], int*, int, bool);
int main(int argc, char* argv[])
{
    std::cout << "Welcome to the assembly language compiler!" << std::endl << std::endl;
    if (argc > 2)
    {
        std::cerr << "Error. Invalid Files input";
        return -1;
    }
    if (argc < 2)
    {
        char line[100];
        std::cout << "It appears that you have not entered an input file. Please enter it here (include .txt): ";
        std::cin >> line;
        std::cout << std::endl;
        argv[1] = &line[0];
    }
    const char *inputFile = argv[1];
    return getAndRunFile(inputFile);
}
int validateDocument(char fileName[], int last)
{
    char line[10000];
    bool totallyValid = true;
    int data = 0;
    int loadStore = 0;
    int ALU = 0;
    int compareJump = 0;
    for (int i = 0; i <= last; i++)
    {
        char *currentLine = getLine(i, fileName, line);
        bool valid = validate(currentLine, i, &loadStore, &ALU, &compareJump, &data);
        if (!valid)
        {
            totallyValid = false;
        }
    }
    std::cout << "Number of Load/Store: " << loadStore << std::endl;
    std::cout << "Number of ALU: " << ALU << std::endl;
    std::cout << "Number of Compare/Jump: " << compareJump << std::endl << std::endl;
    if (!totallyValid) return -1;
    return runCode(fileName, last, line, data);
}
int runCode(char fileName[], int last, char line[], int data)
{
    int registers[9];
    int memory[10000];
    for (int i = 0; i < 10000; i++)
    {
        memory[i] = INT_MIN;
    }
    bool success = true;
    for (int i = 0; i <= last; i++)
    {
        char *currentLine = getLine(i, fileName, line);
        bool colon = false;
        int k = 0;
        while (*(currentLine + k) != '\0')
        {
            if (*(currentLine + k) == ':') colon = true;
            k++;
        }
        bool b = false;
        while ((*currentLine == '\0' || b) && i <= last)
        {
            k = 1;
            b = false;
            colon = false;
            i++;
            currentLine = getLine(i, fileName, line);
            while (*(currentLine+k) != '\0')
            {
                if (*(currentLine+k) == ':') b = true;
                k++;
            }
        }
        if (!colon)
        {
            switch(*line)
            {
                case 'L':
                    //run load function
                    success = load(registers, currentLine, memory, data);
                    break;
                case 'S':
                    switch (*(line+1))
                    {
                        case 'D':
                        {
                            //run store function
                            success = store(registers, currentLine, memory, data);
                            break;
                        }
                        default:
                            success = sub(registers, currentLine);
                            //run sub function
                            break;
                    }
                    break;
                case 'A':
                    //run add function
                    success = add(registers, currentLine);
                    break;
                case 'M':
                    //run mul function
                    success = mul(registers, currentLine);
                    break;
                case 'D':
                    //run div function
                    success = div(registers, currentLine);
                    break;
                case 'J':
                    {
                    int k = 0;
                    bool word = false;
                    while (*(currentLine + k) != '\0')
                    {
                        if(*(currentLine + k) == '[') word = true;
                        k++;
                    }
                    switch(*(line+1))
                    {
                        case 'M':
                            //run jmp function
                            if (word) success = jmpWord(currentLine, fileName, &i, last);
                            else success = jmpNumber(currentLine, &i, last);
                            break;
                        case 'Z':
                            //run jmpZero function
                            if (registers[currentRegister(currentLine,1)] == 0)
                            {
                                if (word) success = jmpWord(currentLine, fileName, &i, last);
                                else success = jmpNumber(currentLine, &i, last);
                            }
                            break;
                        case 'N':
                            //run jmpNotZero function
                            if (registers[currentRegister(currentLine,1)] != 0)
                            {
                                if (!word) success = jmpNumber(currentLine, &i, last);
                                else success = jmpWord(currentLine, fileName, &i, last);
                            }
                            break;
                        case 'G':
                            switch(*(line+2))
                            {
                                case 'Z':
                                    //run jmpGreaterZero function
                                    if (registers[currentRegister(currentLine,1)] > 0)
                                    {
                                        if (word) success = jmpWord(currentLine, fileName, &i, last);
                                        else success = jmpNumber(currentLine, &i, last);
                                    }
                                    break;
                                default:
                                    //run jmpGreaterEqualZero function
                                    if (registers[currentRegister(currentLine,1)] >= 0)
                                    {
                                        if (!word) success = jmpNumber(currentLine, &i, last);
                                        else success = jmpWord(currentLine, fileName, &i, last);
                                    }
                                    break;
                            }
                            break;
                        case 'L':
                            switch(*(line+2))
                            {
                                case 'Z':
                                    if (registers[currentRegister(currentLine,1)] < 0)
                                    {
                                        if (!word) success = jmpNumber(currentLine, &i, last);
                                        else success = jmpWord(currentLine, fileName, &i, last);
                                    }
                                    break;
                                default:
                                    //run jmpLessEqualZero function
                                    if (registers[currentRegister(currentLine,1)] <= 0)
                                    {
                                        if (!word) success = jmpNumber(currentLine, &i, last);
                                        else success = jmpWord(currentLine, fileName, &i, last);
                                    }
                                    break;
                            }
                            break;
                    }
                    }
                }
            }
        if (!success)
        {
            return success;
        }
    }
    std::cout << "Your assembly language has been processed succesfully!" << std::endl;
    std::cout << "Here are the final values for your registers:" << std::endl;
    for (int i = 0; i < 9; i++)
    {
        std::cout << "Register [" << i+1 << "]: " << registers[i] << std::endl;
    }
    std::cout << std::endl;
    std::cout << "Here are the final values for memory:" << std::endl;
    for (int i = 0; i < 10000; i++)
    {
        if (memory[i] != INT_MIN) std::cout << "Memory [" << i+data << "]: " << memory[i] << std::endl;
    }
    std::cout << std::endl << "Thanks for using this program! Have a great day!" << std::endl;
    return success;
}
bool load(int (&registers)[9], char* currentLine, int (&memory)[10000], int data)
{
    if (*(currentLine+2) == 'i') registers[currentRegister(currentLine,2)] = currentValue(currentLine, 1);
    else if (*(currentLine+3) == 'R') registers[currentRegister(currentLine,2)] = registers[currentRegister(currentLine,1)];
    else
    {
        int i = currentValue(currentLine, 1) - data;
        if (i < 0) return false;
        else registers[currentRegister(currentLine,2)] = memory[i];
    }
    return true;
}
bool store(int (&registers)[9], char* currentLine, int (&memory)[10000], int data)
{
    if (*(currentLine+2) == 'i')
    {
        int k = currentValue(currentLine, 2) - data;
        if (k < 0) return false;
        int i = 0;
        while (*(currentLine+4+i) != ' ' && *(currentLine+4+i) != '\n')
        {
            i++;
        }
        if (*(currentLine+5+i) == 'R') registers[currentRegister(currentLine,3)] = memory[k];
        else memory[k] = currentValue(currentLine,1);
    }
    else
    {
        int i = 0;
        while (*(currentLine+4+i) != ' ') i++;
        if (*(currentLine+5+i) == 'R') registers[currentRegister(currentLine,1)] = registers[currentRegister(currentLine,2)];
        else
        {
            int k = currentValue(currentLine, 2) - data;
            if (k < 0) return false;
            memory[k] = registers[currentRegister(currentLine,1)];
        }
    }
    return true;
}
bool sub(int (&registers)[9], char *currentLine)
{
    if (*(currentLine+3) == 'i') registers[currentRegister(currentLine,3)] = registers[currentRegister(currentLine,1)] - currentValue(currentLine,2);
    else registers[currentRegister(currentLine,3)] = registers[currentRegister(currentLine,1)] - registers[currentRegister(currentLine,2)];
    return true;
}
bool add(int (&registers)[9], char *currentLine)
{
    if (*(currentLine+3) == 'i') registers[currentRegister(currentLine,3)] = registers[currentRegister(currentLine,1)] + currentValue(currentLine,2);
    else registers[currentRegister(currentLine,3)] = registers[currentRegister(currentLine,1)] + registers[currentRegister(currentLine,2)];
    return true;
}
bool mul(int (&registers)[9], char *currentLine)
{
    if (*(currentLine+3) == 'i') registers[currentRegister(currentLine,3)] = registers[currentRegister(currentLine,1)] * currentValue(currentLine,2);
    else registers[currentRegister(currentLine,3)] = registers[currentRegister(currentLine,1)] * registers[currentRegister(currentLine,2)];
    return true;
}
bool div(int (&registers)[9], char *currentLine)
{
    if (*(currentLine+3) == 'i')
    {
        int i = currentValue(currentLine,2);
        if (i == 0) return false;
        else registers[currentRegister(currentLine,3)] = registers[currentRegister(currentLine,1)] / i;
    }
    else
    {
        int i = registers[currentRegister(currentLine,2)];
        if (i == 0) return false;
        else registers[currentRegister(currentLine,3)] = registers[currentRegister(currentLine,1)] / i;
    }
    return true;
}
bool jmpNumber(char *currentLine, int *current, int last)
{
    int i = currentValue(currentLine,1);
    if (i > last) return false;
    *current = i+1;
    return true;
}
bool jmpWord(char *currentLine, char fileName[], int *current, int last)
{
    char line[10000];
    int labelLocation = 0;
    while (*(currentLine + labelLocation) != '[') labelLocation++;
    for(int i = 0; i <= last; i++)
    {
        char *c = getLine(i, fileName, line);
        int k = 0;
        while (*(c+k) == *(currentLine+labelLocation+1+k))
        {
            k++;
            if (*(currentLine+labelLocation+1+k) == ']')
            {
                *current = i;
                return true;
            }
        }
    }
    return false;
}
int getAndRunFile(const char* fileName)
{
    int i = 0;
    while (*(fileName + i) != '\0')
    {
        i++;
    }
    char inputFile[i-1];
    for (int k = 0; k < i+1; k++)
    {
        inputFile[k] = *(fileName + k);
    }
    return openFile(inputFile);
}
int openFile(char fileName[])
{
    std::ifstream inputFile;
    inputFile.open(fileName);
    if (inputFile.is_open())
    {
        inputFile >> std::noskipws;
        char c;
        int totalInstructions = 0;
        while (inputFile >> c)
        {
            if(c == '\n') totalInstructions++;
        }
        inputFile.close();
        std::cout << "Total number of assembly instructions: " << totalInstructions + 1<< std::endl;
        return validateDocument(fileName, totalInstructions);
    }
    std::cerr << "Error, Unable to open file: " << fileName;
    return -1;
}
char* getLine(int current, char fileName[], char line[])
{
    std::ifstream inputFile(fileName);
    inputFile >> std::noskipws;
    int i = 0;
    int currentLine = 1;
    char c;
    while (inputFile >> c)
    {
        if (currentLine == current)
        {
            while(c == ' ' && inputFile.peek() == '\t') inputFile >> c;
            bool tabCommaFound = false;
            while (c == ',' || c == '\t')
            {
                while (c == ',') inputFile >> c;
                while (c == '\t') inputFile >> c;
                while (c == ' ' && inputFile.peek() == ' ') inputFile >> c;
                tabCommaFound = true;
            }
            if (tabCommaFound && c != ' ' && c != '\n' && c != '\0' && i != 0)
            {
                line[i] = ' ';
                i++;
            }
            if (c == '#')
            {
                while (inputFile >> c) if (c == '\n') break;
            }
            if (c == '\n') break;
            line[i] = c;
            i++;
        }
        if (c == '\n')
        {
            currentLine++;
        }
    }
    if (currentLine < current)
    {
        line[0] = 'd';
        return line;
    }
    line[i] = '\0';
    if (line[i-1] != ':' && line[i-1] != ']' && line[0] != 'C')
    {
        line[i-1] = '\0';
    }
    return line;
}
bool validate(char* line, int current, int* loadStore, int* ALU, int* compareJump, int* data)
{
    if (*line == '\0') return true;
    int length = 0;
    while (*(line+length) != '\0') length++;
    if (*(line+length-1) == ':' || *(line) == '#' || (*(line) == 'C' && *(line+1) == 'o' && *(line+2) == 'd' && *(line+3) == 'e' && *(line+4) == ':')) return true;
    if (*(line) == 'D' && *(line+1) == 'a' && *(line+2) == 't' && *(line+3) == 'a' && *(line+4) == ':')
    {
        if (*data != 0) return false;
        int i = 0;
        int decimal = 0;
        while (*(line+7+decimal) != '\0')
        {
            decimal++;
        }
        while (decimal >= 0)
        {
            switch(*(line+6+i))
            {
            case '0':
                break;
            case '1':
                *data += 1 * (pow(10,decimal) + 0.1);
                break;
            case '2':
                *data += 2 * (pow(10,decimal) + 0.1);
                break;
            case '3':
                *data += 3 * (pow(10,decimal) + 0.1);
                break;
            case '4':
                *data += 4 * (pow(10,decimal) + 0.1);
                break;
            case '5':
                *data += 5 * (pow(10,decimal) + 0.1);
                break;
            case '6':
                *data += 6 * (pow(10,decimal) + 0.1);
                break;
            case '7':
                *data += 7 * (pow(10,decimal) + 0.1);
                break;
            case '8':
                *data += 8 * (pow(10,decimal) + 0.1);
                break;
            case '9':
                *data += 9 * (pow(10,decimal) + 0.1);
                break;
            default:
                return false;
            }
            decimal--;
            i++;
        }
        return true;
    }
    switch(*line)
        {
            case 'L':
                switch (*(line+1))
                {
                    case 'D':
                    {
                        //run load function
                        *loadStore += 1;
                        if (*(line+2) == 'i')
                        {
                            return (validNumber(line,1, true) && validRegister(line,2));
                        }
                        if (*(line+3) == 'R') return (validRegister(line,1) && validRegister(line,2));
                        else
                        {
                            return (validNumber(line,1, false) && validRegister(line,2));
                        }
                    }
                    default:
                        std::cerr << "Error on line " << current+1 << ": No such OPCODE" << std::endl;
                        return false;
                }
                break;
            case 'S':
                switch (*(line+1))
                {
                    case 'D':
                    {
                        *loadStore += 1;
                        int position = 0;
                        bool firstCaseEncountered = false;
                        if (*(line+2) == 'i')
                        {
                            while(*(line+position) != ' ' || !firstCaseEncountered)
                            {
                                if (*(line+position) == ' ') firstCaseEncountered = true;
                                position++;
                            }
                            if (*(line+position+1) == 'R') return(validNumber(line,1, false) && validRegister(line,2));
                            else return(validNumber(line,1, false) && validNumber(line,2, true));
                        }
                        else
                        {
                            while(*(line+position) != ' ' || !firstCaseEncountered)
                            {
                                if (*(line+position) == ' ') firstCaseEncountered = true;
                                position++;
                            }
                            if (*(line+position+1) == 'R') return(validRegister(line,1) && validRegister(line,2));
                            else return (validRegister(line,1) && validNumber(line,2, true));
                        }

                    }
                    case 'U':
                        switch (*(line+2))
                        {
                            case 'B':
                            {
                                *ALU += 1;
                                if(*(line+3) == 'i') return (validRegister(line,1) && validNumber(line,2, false) && validRegister(line,3));
                                else return (validRegister(line,1) && validRegister(line,2) && validRegister(line,3));
                            }
                            default:
                                std::cerr << "Error on line " << current+1 << ": No such OPCODE" << std::endl;
                                return false;
                        }
                        break;
                    default:
                        std::cerr << "Error on line " << current+1 << ": No such OPCODE" << std::endl;
                        return false;
                }
                break;
            case 'A':
                switch(*(line+1))
                {
                    case 'D':
                        switch(*(line+2))
                        {
                            case 'D':
                            {
                                *ALU += 1;
                                if(*(line+3) == 'i') return (validRegister(line,1) && validNumber(line,2, false) && validRegister(line,3));
                                else return (validRegister(line,1) && validRegister(line,2) && validRegister(line,3));
                            }
                            default:
                                std::cerr << "Error on line " << current+1 << ": No such OPCODE" << std::endl;
                                return false;
                        }
                    default:
                        std::cerr << "Error on line " << current+1 << ": No such OPCODE" << std::endl;
                        return false;
                }
                break;
            case 'M':
                switch(*(line+1))
                {
                    case 'U':
                        switch(*(line+2))
                        {
                            case 'L':
                            {
                                *ALU += 1;
                                if(*(line+3) == 'i') return (validRegister(line,1) && validNumber(line,2, false) && validRegister(line,3));
                                else return (validRegister(line,1) && validRegister(line,2) && validRegister(line,3));
                            }
                            default:
                                std::cerr << "Error on line " << current+1 << ": No such OPCODE" << std::endl;
                                return false;
                        }
                    default:
                        std::cerr << "Error on line " << current+1 << ": No such OPCODE" << std::endl;
                        return false;
                }
                break;
            case 'D':
                switch(*(line+1))
                {
                    case 'I':
                        switch (*(line+2))
                        {
                            case 'V':
                            {
                                *ALU += 1;
                                if(*(line+3) == 'i') return (validRegister(line,1) && validNumber(line,2, false) && validRegister(line,3));
                                else return (validRegister(line,1) && validRegister(line,2) && validRegister(line,3));
                            }
                            default:
                                std::cerr << "Error on line " << current+1 << ": No such OPCODE" << std::endl;
                                return false;
                        }
                    default:
                        std::cerr << "Error on line " << current+1 << ": No such OPCODE" << std::endl;
                        return false;
                }
                break;
            case 'J':
                switch(*(line+1))
                {
                    case 'M':
                        switch(*(line+2))
                        {
                            case 'P':
                            {
                                *compareJump += 1;
                                int i = 0;
                                bool squareBracket = false;
                                while (*(line+3+i) != '\n')
                                {
                                    if (*(line+3+i) == '[') squareBracket = true;
                                    i++;
                                }
                                if (squareBracket) return validWord(line,1);
                                else return validNumber(line,1, true);
                            }
                            default:
                                std::cerr << "Error on line " << current+1 << ": No such OPCODE" << std::endl;
                                return false;
                        }
                        break;
                    case 'Z':
                    {
                        return (validRegister(line,1) && validNumber(line,2, true));
                    }
                    case 'N':
                        switch(*(line+2))
                        {
                            case 'Z':
                            {
                                *compareJump += 1;
                                int i = 0;
                                bool squareBracket = false;
                                while (*(line+3+i) != '\n')
                                {
                                    if (*(line+3+i) == '[') squareBracket = true;
                                    i++;
                                }
                                if (squareBracket) return (validRegister(line,1) && validWord(line,2));
                                else return validNumber(line,1, true);
                            }
                            default:
                                std::cerr << "Error on line " << current+1 << ": No such OPCODE" << std::endl;
                                return false;
                        }
                    case 'G':
                        switch(*(line+2))
                        {
                            case 'Z':
                            {
                                *compareJump += 1;
                                int i = 0;
                                bool squareBracket = false;
                                while (*(line+3+i) != '\n')
                                {
                                    if (*(line+3+i) == '[') squareBracket = true;
                                    i++;
                                }
                                if (squareBracket) return (validRegister(line,1) && validWord(line,2));
                                else return validNumber(line,1, true);
                            }
                            case 'E':
                                switch(*(line+3))
                                {
                                    case 'Z':
                                    {
                                        *compareJump += 1;
                                        int i = 0;
                                        bool squareBracket = false;
                                        while (*(line+4+i) != '\n')
                                        {
                                            if (*(line+4+i) == '[') squareBracket = true;
                                            i++;
                                        }
                                        if (squareBracket) return (validRegister(line,1) && validWord(line,2));
                                        else return validNumber(line,1, true);
                                    }
                                    default:
                                        std::cerr << "Error on line " << current+1 << ": No such OPCODE" << std::endl;
                                        return false;
                                }
                            default:
                                std::cerr << "Error on line " << current+1 << ": No such OPCODE" << std::endl;
                                return false;
                        }
                        break;
                    case 'L':
                        switch(*(line+2))
                        {
                            case 'Z':
                            {
                                *compareJump += 1;
                                int i = 0;
                                bool squareBracket = false;
                                while (*(line+3+i) != '\n')
                                {
                                    if (*(line+3+i) == '[') squareBracket = true;
                                    i++;
                                }
                                if (squareBracket) return (validRegister(line,1) && validWord(line,2));
                                else return validNumber(line,1, true);
                            }
                            case 'E':
                                switch(*(line+3))
                                {
                                    case 'Z':
                                    {
                                        *compareJump += 1;
                                        int i = 0;
                                        bool squareBracket = false;
                                        while (*(line+3+i) != '\n')
                                        {
                                            if (*(line+3+i) == '[') squareBracket = true;
                                            i++;
                                        }
                                        if (squareBracket) return (validRegister(line,1) && validWord(line,2));
                                        else return validNumber(line,1, true);
                                    }
                                    default:
                                        std::cerr << "Error on line " << current+1 << ": No such OPCODE" << std::endl;
                                        return false;
                                }
                            default:
                                std::cerr << "Error on line " << current+1 << ": No such OPCODE" << std::endl;
                                return false;
                        }
                    default:
                        std::cerr << "Error on line " << current+1 << ": No such OPCODE" << std::endl;
                        return false;
                }
                break;
            default:
                std::cerr << "Error on line " << current+1 << ": No such OPCODE" << std::endl;
                return false;
        }
    return false;
}
bool validNumber(char* line, int spaces, bool positive)
{
    int previousNumber = 0;
    int nextNumber = 0;
    int decimal = 0;
    int spacesFound = 0;
    int position = 0;
    while (spacesFound < spaces)
    {
        if (*(line+position+1) == ' ' || *(line+position+1) == '\n' || *(line+position+1) == '\n') spacesFound++;
        position++;
    }
    position++;
    while (*(line+position+decimal) != ' ' && *(line+position+decimal) != '\n' && *(line+position+decimal) != '\0') decimal++; decimal--;
    while (decimal >= 0)
    {
        switch(*(line+position))
        {
        case '-':
            if (positive || *(line+position-1) != ' ')
                return false;
        case '0':
            break;
        case '1':
            nextNumber = 1 * (pow(10,decimal)+0.1);
            break;
        case '2':
            nextNumber = 2 * (pow(10,decimal)+0.1);
            break;
        case '3':
            nextNumber = 3 * (pow(10,decimal)+0.1);
            break;
        case '4':
            nextNumber = 4 * (pow(10,decimal)+0.1);
            break;
        case '5':
            nextNumber = 5 * (pow(10,decimal)+0.1);
            break;
        case '6':
            nextNumber = 6 * (pow(10,decimal)+0.1);
            break;
        case '7':
            nextNumber = 7 * (pow(10,decimal)+0.1);
            break;
        case '8':
            nextNumber = 8 * (pow(10,decimal)+0.1);
            break;
        case '9':
            nextNumber = 9 * (pow(10,decimal)+0.1);
            break;
        default:
            return false;
        }
        if (nextNumber + previousNumber < nextNumber) return false;
        else previousNumber += nextNumber;
        position++;
        decimal--;
    }
    return true;
}
bool validRegister(char* line, int spaces)
{
    int spacesFound = 0;
    int position = 0;
    while (spacesFound < spaces)
    {
        if (*(line+position+1) == ' ' || *(line+position+1) == '\n' || *(line+position+1) == '\0') spacesFound++;
        position++;
    }
    position++;
    if (*(line+position) != 'R')
    {
        return false;
    }
    else
    {
        switch(*(line+position+1))
        {
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                if (*(line+position+2) != ' ' && *(line+position+2) != '\n' && *(line+position+2) != '\0')
                {
                    return false;
                }
                return true;
                break;
        default:
            return false;
        }
    }
}
bool validWord(char *line, int spaces)
{
    int spacesFound = 0;
    int position = 0;
    while (spacesFound < spaces)
    {
        if (*(line+position+1) == ' ' || *(line+position+1) == '\n' || *(line+position+1) == '\n') spacesFound++;
        position++;
    }
    position++;
    bool endFound = false;
    int i = 0;
    while (*(line+i+position) != '\0')
    {
        if (*(line+i+position) == ']' && (*(line+i+position+1) == '\0' || *(line+i+position+1) == ' ')) endFound = true;
        i++;
    }
    return endFound;
}
int currentRegister(char* currentLine, int spaces)
{
    int spacesFound = 0;
    int position = 0;
    while (spacesFound < spaces)
    {
        if (*(currentLine+position+1) == ' ' || *(currentLine+position+1) == '\n' || *(currentLine+position+1) == '\n') spacesFound++;
        position++;
    }
    position++;
    switch (*(currentLine+position+1))
    {
        case '1':
            return 0;
        case '2':
            return 1;
        case '3':
            return 2;
        case '4':
            return 3;
        case '5':
            return 4;
        case '6':
            return 5;
        case '7':
            return 6;
        case '8':
            return 7;
        case '9':
            return 8;
    }
    return position;
}
int currentValue(char* currentLine, int spaces)
{
    int value = 0;
    int decimal = 0;
    int negative = 1;
    int spacesFound = 0;
    int position = 0;
    while (spacesFound < spaces)
    {
        if (*(currentLine+position+1) == ' ' || *(currentLine+position+1) == '\n' || *(currentLine+position+1) == '\n') spacesFound++;
        position++;
    }
    position++;
    while (*(currentLine+position+decimal) != ' ' && *(currentLine+position+decimal) != '\n' && *(currentLine+position+decimal) != '\0') decimal++; decimal--;
    while (decimal >= 0)
    {
        switch(*(currentLine+position))
        {
            case '-':
                negative = -1;
                break;
            case '0':
                break;
            case '1':
                value += 1 * (pow(10,decimal)+0.1);
                break;
            case '2':
                value += 2 * (pow(10,decimal)+0.1);
                break;
            case '3':
                value += 3 * (pow(10,decimal)+0.1);
                break;
            case '4':
                value += 4 * (pow(10,decimal)+0.1);
                break;
            case '5':
                value += 5 * (pow(10,decimal)+0.1);
                break;
            case '6':
                value += 6 * (pow(10,decimal)+0.1);
                break;
            case '7':
                value += 7 * (pow(10,decimal)+0.1);
                break;
            case '8':
                value += 8 * (pow(10,decimal)+0.1);
                break;
            case '9':
                value += 9 * (pow(10,decimal)+0.1);
                break;
        }
        position++;
        decimal--;
    }
    return value * negative;
}
