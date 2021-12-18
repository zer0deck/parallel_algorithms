#include <iostream>
#include <omp.h>

int main(int argc, char** argv)
{
    unsigned w_num = 0;
    char*       string = argv[1];
    char        new_symb, prev_symb;
    unsigned    i = 1;

    if (argc < 2)
    {
        std::cout << "No argument passed" << std::endl;
        return 1;
    }

    prev_symb = new_symb = string[0];

    while (new_symb != '\0')
    {
        new_symb = string[i];

        if (!((prev_symb == ' ') or (prev_symb == '\t') or (prev_symb == '\n')) and
            ((new_symb == ' ') or (new_symb == '\t') or (new_symb == '\n') or (new_symb == '\0')))
        {
            w_num++;
        }
        prev_symb = new_symb;
        i++;
    }

    std::cout << "Number of words:" << w_num << std::endl;
    return 0;
}