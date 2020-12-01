#include <iostream>
#include <map>
#include <string>
#include <cstring>
#include <ratio>
#include <chrono>
#include <fstream>
#include <vector>
using namespace std;
using namespace std::chrono;

#include <assert.h>


const int N = 2048;
const int m0 = 1024;

float A[N][N],B[N][N],C[N][N], D[N][N];

float rand_float(float s)
{
    return 4*s*(1-s);
}


void matrix_gen(float* a, float* b, int N, float seed)
{
    float s = seed;
    for(int i=0; i<N*N; i++)
    {
        s = rand_float(s);
        a[i] = s;
        s = rand_float(s);
        b[i] = s;
    }
}

void mul_stupid(float* C, float* A, float* B, int N)
{
    memset(C, 0, sizeof(float) * N * N);
    int i, j, k;

    for(i = 0; i < N; i++)
        for(j = 0; j < N; j++)
            for(k = 0; k < N; k++)
                C[j + i * N] += A[k + i * N] * B[j + k * N];
}


void mul(float* C, float* A, float* B, int N, int m0)
{
    memset(C, 0, sizeof(float) * N * N);
    int i, j, k, i1, j1, k1;

    for(i = 0; i < N; i += m0)
        for(j = 0; j < N; j += m0)
            for(k = 0; k < N; k += m0)
                for(i1 = i; i1 < i + m0 && i1 < N; i1++)
                    for(j1 = j; j1 < j + m0 && j1 < N; j1++)
                        for(k1 = k; k1 < k + m0 && k1 < N; k1++)
                            C[j1 + i1 * N] += A[k1 + i1 * N] * B[j1 + k1 * N];
}



void check_is_the_same(float* A, float* B)
{
    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
        {
            //cout << A[j + i*N] << " " << B[j + i*N] << "\n";
            assert(A[j + i*N] == B[j + i*N]);
        }
    }
}

void print_matrix(float* A)
{
    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
        {
            cout << A[j + i*N] << " ";
        }
        cout << "\n";
    }
}




class Environment {
public:
    double original_time_result;
    double block_time_result;

    void init()
    {
        matrix_gen((float*)A, (float*)B, N, 0.6);
    }

    void run()
    {
        init();
        //run_experiment_original();
        init();
        run_experiment_block();
        write_result();
        //check_is_the_same((float*)C,(float*)D);
    }


    void run_experiment_original()
    {
        high_resolution_clock::time_point t1 = high_resolution_clock::now();

        mul_stupid((float*)D,(float*)A,(float*)B,N);

        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
        std::cout << "original took me " << time_span.count() << " seconds.";
        std::cout << std::endl;

        original_time_result = time_span.count();
    }

    void run_experiment_block()
    {
        high_resolution_clock::time_point t1 = high_resolution_clock::now();

        mul((float*) C, (float*) A, (float*) B, N,  m0);

        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
        std::cout << "block took me " << time_span.count() << " seconds.";
        std::cout << std::endl;

        block_time_result = time_span.count();
    }

    void write_result()
    {
        ofstream output;
        output.open("result.txt", ios::app);
        output << N  <<  "\t    " <<  m0  << "\t    " <<  original_time_result << "\t    " << block_time_result <<  "\n";
        output.close();
    }
};


int main()
{
    Environment env;
    env.run();
}