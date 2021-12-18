#include <iostream>
#include <omp.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

int main(int argc, char** argv){
    if (argc > 1) // if we pass arguments to function, argc will be more than 1 (depends on number of arguments)
    {
        int vec_size = atoi (argv [1]); // size of the vector
        double* randArray = new double[vec_size];
        for (long int i = 0; i < vec_size; i++) { //Generate number between 0 to 1
            randArray[i] = (double)rand() / RAND_MAX;
        } 
        //cout<<"\nElements of the array::"<<endl;
        // printing elements of an array
        //for(int i = 0; i < vec_size; i++)
        //    cout<<"Elements no "<< i + 1<< "::" << randArray[i] <<endl;
        //  }
        
        // iterate over the number of threads
        for(int thread_num = 1; thread_num < 11; thread_num++){
            double max_val = -1.0; // as all the elements in array between 0 to 1
            auto start = high_resolution_clock::now(); // get the current time
            #pragma omp parallel for num_threads(thread_num) reduction (max:max_val)
            for (int i = 0; i < vec_size; i++){
                if (randArray[i] > max_val){max_val = randArray[i];}
            }
            auto stop = high_resolution_clock::now(); // get the stop time
            auto duration = duration_cast<microseconds>(stop - start); // calculate the duration
            printf("Number of threads is %d, Execution time (in microseconds): %lld\n", thread_num, duration.count());
        }
    }
    else
    {
        cout << "Not arguments were given" << endl;
    }
    return 0;
}