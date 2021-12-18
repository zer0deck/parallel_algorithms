#include <iostream>
#include <cstdlib>
#include <vector>
#include <omp.h>
#include <chrono>


int main(int argc, char** argv)
{
if (argc > 1) // blocks the programm if there are no entered arguments
{
    int vec_size = atoi(argv [1]); // size of the vector
    std::vector<double> randArray(vec_size);

    for (auto &vec_item: randArray) // Fill in random variables
    {
        vec_item = (double)rand() / (RAND_MAX/10);
    }

    if (argc > 2 and vec_size <= 10) // we test the programm for 10 arguments (as written in an exercise)
    {
        std::cout << "Elements of the array: ";
        for (auto &vec_item: randArray)
        {
            std::cout << vec_item << "  ";
        }
        std::cout << std::endl;
    }

    for(auto thread_num = 1; thread_num < 11; thread_num++)
    {
        double maximum_value = -1.0; 
        auto time_start = std::chrono::high_resolution_clock::now(); // c.time

#pragma omp parallel for num_threads(thread_num) reduction (max:maximum_value)

        for (auto i = 0; i < vec_size; i++)
        {
            if (randArray[i] > maximum_value)
            {
                maximum_value = randArray[i];
            }
        }

        auto time_stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(time_stop - time_start);
        std::cout << "Thread: " << thread_num << " ET(ms): " <<  duration.count() << " ";

        //Enter any thind argument to see elements (if array is less than eleven)
        if (argc > 2 and vec_size <= 10)
        {
            std::cout << "Maximum value: " << maximum_value << std::endl;
        }
    }
}
else
{
std::cout << "You hadn't entered any arguments" << std::endl;
}
return 0;
}