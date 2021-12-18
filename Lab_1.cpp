#include <iostream>
#include <cstdlib>
#include <vector>
#include <omp.h>
#include <chrono>


int main(int argc, char** argv)
{
    if (argc > 1) // if we pass arguments to function, argc will be more than 1
    {
        int vec_size = atoi(argv [1]); // size of the vector

        std::vector<double> randArray(vec_size);

        for (auto &vec_item: randArray) // Fill in random variables
        {
            vec_item = (double)rand() / (RAND_MAX/10);
        }
        // Print elements if pass 2nd arg and our array is not very big
        if (argc > 2 and vec_size <= 10)
        {
            std::cout << "Elements of the array: ";
            for (auto &vec_item: randArray)
            {
                std::cout << vec_item << "  ";
            }
            std::cout << std::endl;
        }
        // iterate over the number of threads
        for(auto thread_num = 1; thread_num < 11; thread_num++)
        {
            double max_val = -1.0; // as all the elements in array between 0 to 1

            auto time_start = std::chrono::high_resolution_clock::now(); // get the current time

#pragma omp parallel for num_threads(thread_num) reduction (max:max_val)
            for (auto i = 0; i < vec_size; i++)
            {
                if (randArray[i] > max_val)
                {
                    max_val = randArray[i];
                }
            }
            auto time_stop = std::chrono::high_resolution_clock::now(); // get the stop time

            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(time_stop - time_start); // calculate the duration

            std::cout << "Number of threads:" << thread_num << " Execution time (in microseconds): " <<  duration.count() << std::endl;

            // Print elements if pass 2nd arg and our array is not very big
            if (argc > 2 and vec_size <= 10)
            {
                std::cout << "Found maxval: " << max_val << std::endl;

            }
        }
    }
    else
    {
        std::cout << "Not arguments were given" << std::endl;
    }
    return 0;
}