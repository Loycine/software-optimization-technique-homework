#pragma once
#include <iostream>
#include <cstdio>
#include <assert.h>

using namespace std;

const int maxn = 1 << 16 +5;


int brute_force_count_front_zero_bit_in_number_release(unsigned int x)
{
    int ret = 0;
    while(x)
    {
        ret += 1;
        x >>= 1;
    }
    return 32 - ret;
}


int brute_force_count_front_zero_bit_in_number(unsigned int x, int total_bit=32)
{
    int ret = 0;
    while(x)
    {
        ret += 1;
        x >>= 1;
    }
    return total_bit - ret;
}

int count_front_zero_bit[maxn];
void init_count_front_zero_bit_table()
{
    for(int i=0; i<maxn; i++)
    {
        count_front_zero_bit[i] = brute_force_count_front_zero_bit_in_number(i, 16);
    }
}

int look_up_table_front_zero_bit_in_number(unsigned int x)
{
    int high_bit = x/(1<<16);
    int low_bit = x%(1<<16);

    if(count_front_zero_bit[high_bit] != 16) {
        return count_front_zero_bit[high_bit];
    }
    return count_front_zero_bit[high_bit] + count_front_zero_bit[low_bit];
}

void check_front_zero_bit_is_the_same()
{
    for(int i=0; i<1e1; i++)
    {
        int x = rand();
        // cout << x << " " << brute_force_count_front_zero_bit_in_number(x) << " " << look_up_table_front_zero_bit_in_number(x) <<"\n";
        assert(brute_force_count_front_zero_bit_in_number(x) == look_up_table_front_zero_bit_in_number(x));
        assert(brute_force_count_front_zero_bit_in_number(-x) == look_up_table_front_zero_bit_in_number(-x));
    }
}