#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <omp.h>
#include <chrono>

#define MAX_THREADS_NUM         10

using namespace std;
using namespace std::chrono;

// function to generate random matrices
double** generate_random_matrix(int rows, int columns)
{
    double** matrix = new double*[rows];

    for (int i = 0; i < rows; i++)
    {
        matrix[i] = new double[columns];
    }
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            matrix[i][j] = (double)rand() / RAND_MAX;
        }
    }
    return matrix;
}

// function to make the values of matrix zero
double** generate_empty_matrix(int rows, int columns)
{
    double** matrix = new double*[rows];

    for (int i = 0; i < rows; i++)
    {
        matrix[i] = new double[columns];
    }
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            matrix[i][j] = 0;
        }
    }
    return matrix;
}

// Free the allocated memory
void release_matrix(double** matrix, int rows, int columns)
{
    if (matrix != NULL)
    {
        for (int i = 0; i < rows; i++)
        {
            if (matrix[i] != NULL)
            {
                delete matrix[i];
            }
        }
    }

    delete matrix;

}

//Print matrix
void print_matrix(double** matrix, int rows, int columns)
{
    cout << endl;

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            cout << setw(10) << matrix[i][j] << "  ";
        }
        cout << endl;
    }

    cout << endl;
}


int main(int argc, char** argv)
{
    if (argc > 2) // if we pass arguments to function, argc will be more than 1 (depends on number of arguments)
    {
        unsigned rows_num =  atoi(argv[1]); // number of rows in matrix
        unsigned cols_num = atoi (argv[2]); // number of columns in matrix

        double **matrix_a, **matrix_b, **matrix_c; // initialize varibles for matrices
        // generate matrix A
        matrix_a = generate_random_matrix(rows_num, cols_num);

        if (rows_num * cols_num <= 25)
        {
            cout << "Matrix output:" << endl;
            print_matrix(matrix_a, rows_num, cols_num);
        }

        // generate matrix B
        matrix_b = generate_random_matrix(cols_num, rows_num);

        if (rows_num * cols_num <= 25)
        {
            print_matrix(matrix_b, cols_num, rows_num);
        }

        // generate matrix C with zero values (it size becomes rows*rows)
        matrix_c = generate_empty_matrix(rows_num, rows_num);

        int* time_array = new int[MAX_THREADS_NUM]; // initialize array that will store the execution time of matrix multiplication
        int sum_time = 0;

        //iterate over the number of threads
        for(int thread_num = 1; thread_num <= MAX_THREADS_NUM; thread_num++)
        {
            matrix_c = generate_empty_matrix(rows_num, rows_num);
            auto time_start = high_resolution_clock::now(); // get the current time
            #pragma omp parallel for num_threads(thread_num)
            for (int i = 0; i < rows_num; i++)
            {
                for (int k = 0; k < rows_num; k++)
                {
                    for (int j = 0; j < cols_num; j++)
                    {
                        matrix_c[i][j] += matrix_a[i][k] * matrix_b[k][j];
                    }
                }
            }
            auto time_stop = high_resolution_clock::now(); // get the stop time
            auto duration = duration_cast<microseconds>(time_stop - time_start); // calculate the duration
            time_array[thread_num - 1] = duration.count();
            sum_time += duration.count();

            if (thread_num == MAX_THREADS_NUM)
            {
                print_matrix(matrix_c, rows_num, rows_num);
            }

            release_matrix(matrix_c, rows_num, rows_num);
        }
        printf("i-j-k order");
        cout << endl;

        for (int i = 0; i < MAX_THREADS_NUM; i++)
        {
            printf("Number of threads: %d, Time (in microseconds): %lld, Efficiency: %f\n", i + 1, time_array[i], float(time_array[i])/float(sum_time));
        }
        cout << endl;



        sum_time = 0;
        //iterate over the number of threads
        for(int thread_num = 1; thread_num <= MAX_THREADS_NUM; thread_num++)
        {
            matrix_c = generate_empty_matrix(rows_num, rows_num);
            auto time_start = high_resolution_clock::now(); // get the current time
            #pragma omp parallel for num_threads(thread_num)
            for (int i = 0; i < rows_num; i++)
            {
                for (int k = 0; k < rows_num; k++)
                {
                    for (int j = 0; j < cols_num; j++)
                    {
                        matrix_c[i][j] += matrix_a[i][k] * matrix_b[k][j];
                    }
                }
            }
            auto time_stop = high_resolution_clock::now(); // get the stop time
            auto duration = duration_cast<microseconds>(time_stop - time_start); // calculate the duration
            time_array[thread_num - 1] = duration.count();
            sum_time += duration.count();

            release_matrix(matrix_c, rows_num, rows_num);
        }
        printf("i-k-j order");
        cout << endl;

        for (int i = 0; i < MAX_THREADS_NUM; i++)
        {
            printf("Number of threads: %d, Time (in microseconds): %lld, Efficiency: %f\n", i + 1, time_array[i], float(time_array[i])/float(sum_time));
        }
        cout << endl;



        sum_time = 0;
        //iterate over the number of threads
        for(int thread_num =1; thread_num <= MAX_THREADS_NUM; thread_num++)
        {
            matrix_c = generate_empty_matrix(rows_num, rows_num);
            auto time_start = high_resolution_clock::now(); // get the current time
            #pragma omp parallel for num_threads(thread_num)
            for (int j = 0; j < rows_num; j++)
            {
                for (int k = 0; k < rows_num; k++)
                {
                    for (int i = 0; i < cols_num; i++)
                    {
                        matrix_c[i][j] += matrix_a[i][k] * matrix_b[k][j];
                    }
                }
            }
            auto time_stop = high_resolution_clock::now(); // get the stop time
            auto duration = duration_cast<microseconds>(time_stop - time_start); // calculate the duration
            time_array[thread_num - 1] = duration.count();
            sum_time += duration.count();

            release_matrix(matrix_c, rows_num, rows_num);

        }
        printf("j-k-i order");
        cout << endl;
        for (int i = 0; i < MAX_THREADS_NUM; i++)
        {
            printf("Number of threads: %d, Time (in microseconds): %lld, Efficiency: %f\n", i + 1, time_array[i], float(time_array[i])/float(sum_time));
        }
        cout << endl;


        sum_time = 0;
        //iterate over the number of threads
        for(int thread_num = 1; thread_num <= MAX_THREADS_NUM; thread_num++)
        {
            matrix_c = generate_empty_matrix(rows_num, rows_num);
            auto time_start = high_resolution_clock::now(); // get the current time
            #pragma omp parallel for num_threads(thread_num)
            for (int j = 0; j < rows_num; j++)
            {
                for (int i = 0; i < rows_num; i++)
                {
                    for (int k = 0; k < cols_num; k++)
                    {
                        matrix_c[i][j] += matrix_a[i][k] * matrix_b[k][j];
                    }
                }
            }
            auto time_stop = high_resolution_clock::now(); // get the stop time
            auto duration = duration_cast<microseconds>(time_stop - time_start); // calculate the duration
            time_array[thread_num - 1] = duration.count();
            sum_time += duration.count();

            release_matrix(matrix_c, rows_num, rows_num);
        }
        printf("j-i-k order");
        cout << endl;
        for (int i = 0; i < MAX_THREADS_NUM; i++)
        {
            printf("Number of threads: %d, Time (in microseconds): %lld, Efficiency: %f\n", i + 1, time_array[i], float(time_array[i])/float(sum_time));
        }
        cout << endl;



        sum_time = 0;
        //iterate over the number of threads
        for(int thread_num = 1; thread_num <= MAX_THREADS_NUM; thread_num++)
        {
            matrix_c = generate_empty_matrix(rows_num, rows_num);
            auto time_start = high_resolution_clock::now(); // get the current time
            #pragma omp parallel for num_threads(thread_num)
            for (int k = 0; k < rows_num; k++)
            {
                for (int i = 0; i < rows_num; i++)
                {
                    for (int j = 0; j < cols_num; j++)
                    {
                        matrix_c[i][j] += matrix_a[i][k] * matrix_b[k][j];
                    }
                }
            }
            auto time_stop = high_resolution_clock::now(); // get the stop time
            auto duration = duration_cast<microseconds>(time_stop - time_start); // calculate the duration
            time_array[thread_num - 1] = duration.count();
            sum_time += duration.count();

            release_matrix(matrix_c, rows_num, rows_num);
        }
        printf("k-i-j order");
        cout << endl;
        for (int i = 0; i < MAX_THREADS_NUM; i++)
        {
            printf("Number of threads: %d, Time (in microseconds): %lld, Efficiency: %f\n", i + 1, time_array[i], float(time_array[i])/float(sum_time));
        }
        cout << endl;



        sum_time = 0;
        //iterate over the number of threads
        for(int thread_num = 1; thread_num <= MAX_THREADS_NUM; thread_num++)
        {
            matrix_c = generate_empty_matrix(rows_num, rows_num);
            auto time_start = high_resolution_clock::now(); // get the current time
            #pragma omp parallel for num_threads(thread_num)
            for (int k = 0; k < rows_num; k++)
            {
                for (int j = 0; j < rows_num; j++)
                {
                    for (int i = 0; i < cols_num; i++)
                    {
                        matrix_c[i][j] += matrix_a[i][k] * matrix_b[k][j];
                    }
                }
            }
            auto time_stop = high_resolution_clock::now(); // get the stop time
            auto duration = duration_cast<microseconds>(time_stop - time_start); // calculate the duration
            time_array[thread_num - 1] = duration.count();
            sum_time += duration.count();

            release_matrix(matrix_c, rows_num, rows_num);
        }
        printf("k-j-i order");
        cout << endl;
        for (int i = 0; i < MAX_THREADS_NUM; i++)
        {
            printf("Number of threads: %d, Time (in microseconds): %lld, Efficiency: %f\n", i + 1, time_array[i], float(time_array[i])/float(sum_time));
        }

        release_matrix(matrix_a, rows_num, cols_num);
        release_matrix(matrix_b, cols_num, rows_num);

    }
    else if (argc == 2)
    {
        cout << "Only one argument was given Two required!" << endl;
        return 1;
    }
    else
    {
        cout << "Not arguments were given" << endl;
        return 1;
    }

    return 0;
}