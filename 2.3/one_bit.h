#pragma once
#include <iostream>
#include <cstdio>
#include <assert.h>
#include "zero_bit.h"
using namespace std;

// const int maxn = 1 << 16 +5;

int brute_force_count_one_bit_in_number(unsigned int x)
{
    int ret = 0;
    while(x)
    {
        if(x & 1)
        {
            ret += 1;
        }
        x >>= 1;
    }
    return ret;
}

int count_one_bit[maxn];
void init_count_one_bit_table()
{
    for(int i=0; i<maxn; i++)
    {
        count_one_bit[i] = brute_force_count_one_bit_in_number(i);
    }
}

int look_up_table_one_bit_in_number(unsigned int x)
{
    int high_bit = x/(1<<16);
    int low_bit = x%(1<<16);

    return count_one_bit[low_bit] + count_one_bit[high_bit];
}

void check_one_bit_is_the_same()
{
    for(int i=0; i<1e6; i++)
    {
        int x = rand();
        // cout << x << " " << brute_force_count_one_bit_in_number(x) << " " << look_up_table_one_bit_in_number(x) <<"\n";
        assert(brute_force_count_one_bit_in_number(x) == look_up_table_one_bit_in_number(x));
        assert(brute_force_count_one_bit_in_number(-x) == look_up_table_one_bit_in_number(-x));
    }
}

