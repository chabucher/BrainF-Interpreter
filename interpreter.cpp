#include <iostream>
#include <fstream>

#define CIRCULAR_TAPE true
#define TAPE_SIZE 2056

void RunProgram(std::string);
bool CompileProgram(std::string);

/// <summary>
/// Usage: ./executable BFProgramFile
/// </summmry>
int main(int argc, char** argv) {

    if (argc == 2){
        std::ifstream inFile(argv[1]);

        std::string program = std::string(std::istreambuf_iterator<char>(inFile),
                                          std::istreambuf_iterator<char>()) + '\0';

        RunProgram(program);
    }
    else {
        std::cout << "Usage: ./executable BFProgramFile";
    }
    
    std::cout << std::endl;
    
    return 0;
}

/// <summary>
/// Runs the bf program
/// </summary>
void RunProgram(std::string program) {
    
    if (!CompileProgram(program)) { return; }
    
    unsigned int tape[TAPE_SIZE] = { 0 };
    unsigned int *ptr = tape;

    unsigned int instructionIdx = 0;
    char instruction = program[instructionIdx];

    while(instruction) {

        switch (instruction) {
        case '+':
            ++*ptr;
            break;
        case '-':
            --*ptr;
            break;
        case '<':
            // Jump to back of circular tape
            if (ptr == tape && CIRCULAR_TAPE) {
                ptr += (TAPE_SIZE - 1);
            }
            else {
                --ptr;
            }
            break;
        case '>':
            if (ptr == (tape + TAPE_SIZE - 1) && CIRCULAR_TAPE) {
                ptr = tape;
            }
            else {
                ++ptr;
            }
            break;
        case '[':
            // Jump past the matching ']' if the cell at the pointer is 0
            if (!*ptr) {
                int inbalanced = 1;
                while (inbalanced) {
                    ++instructionIdx;

                    if (program[instructionIdx] == '[') { ++inbalanced; }
                    if (program[instructionIdx] == ']') { --inbalanced; }
                }
            }
            break;
        case ']':
            // Jump back to the matching '[' if the cell at the pointer is NOT 0
            if (*ptr) {
                int inbalanced = 1;
                while (inbalanced) {
                    --instructionIdx;

                    if (program[instructionIdx] == '[') { --inbalanced; }
                    if (program[instructionIdx] == ']') { ++inbalanced; }
                }
            }
            break;
        case '.':
            std::putchar(*ptr);
            break;
        case ',':
            *ptr = std::getchar();
            break;
        }

        instruction = program[++instructionIdx];
    }
}

/// /// <summary>
/// Compiles a bf program. 
/// </summary>
bool CompileProgram(std::string program) {
    int open = 0;
    int close = 0;

    for (std::string::iterator it = program.begin(); it != program.end(); ++it) {
        if (*it == '[') {
            ++open;
        }
        else if (*it == ']') {
            ++close;
        }
    }

    if (open == close) {
        std::cout << "Compilation success! Depth = " << open << "\n\n";
        return true;
    }
    else if (open > close) {
        std::cout << "Compilation failure! " << open - close << " too many opening '[' braces.\n\n";
        return false;
    }
    else {
        std::cout << "Compilation failure! " << close - open << " too many closing ']' braces.\n\n";
        return false;
    }
}
