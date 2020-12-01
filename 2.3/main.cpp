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

#include "one_bit.h"
#include "zero_bit.h"


const int N = 1e7;

class Environment {
public:
    double original_time_result;
    double algorithm_time_result;
    vector<int> x_rand;

    void init()
    {
        init_count_one_bit_table();
        init_count_front_zero_bit_table();

        for(int i=0; i<N; i++)
        {
            x_rand.push_back(rand());
        }
    }

    void run()
    {
        init();

        run_experiment_one_bit();
        write_result("one_bit");
        run_experiment_front_zero();
        write_result("front_zero");
    }


    void run_experiment_one_bit()
    {
        run_experiment_original(brute_force_count_one_bit_in_number);
        run_experiment_algorithm(look_up_table_one_bit_in_number);
        write_result("one_bit");
    }


    void run_experiment_front_zero()
    {
        run_experiment_original(brute_force_count_front_zero_bit_in_number_release);
        run_experiment_algorithm(look_up_table_front_zero_bit_in_number);
        write_result("front_zero_bit");
    }

    void run_experiment_original(int f(unsigned int))
    {
        high_resolution_clock::time_point t1 = high_resolution_clock::now();

        for(int i=0; i<N; i++)
            f(x_rand[i]);

        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
        std::cout << "original took me " << time_span.count() << " seconds.";
        std::cout << std::endl;

        original_time_result = time_span.count();
    }

    void run_experiment_algorithm(int f(unsigned int))
    {
        high_resolution_clock::time_point t1 = high_resolution_clock::now();

       for(int i=0; i<N; i++)
            f(x_rand[i]);

        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
        std::cout << "look up table took me " << time_span.count() << " seconds.";
        std::cout << std::endl;

        algorithm_time_result = time_span.count();
    }

    void write_result(string s)
    {
        ofstream output;
        output.open("result.txt", ios::app);
        output << N  <<  "\t    " <<  s  << "\t    " <<  original_time_result << "\t    " << algorithm_time_result <<  "\n";
        output.close();
    }
};


int main()
{
    Environment env;
    env.run();
}