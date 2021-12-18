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
        std::cout << "You hadn't entered any text" << std::endl;
        return 1;
    }

    prev_symb = new_symb = string[0];

    while (new_symb != '\0')
    {
        new_symb = string[i];

        if (!((prev_symb == ' ') || (prev_symb == '\t') || (prev_symb == '\n')) &&
            ((new_symb == ' ') || (new_symb == '\t') || (new_symb == '\n') || (new_symb == '\0')))
        {
            w_num++;
        }
        prev_symb = new_symb;
        i++;
    }

    std::cout << "The sentence has: " << w_num << " words" << std::endl;
    return 0;
}