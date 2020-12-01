#pragma once

#include<iostream>
#include<string>
#include<ctime>
#include<cstdlib>
#include<unordered_set>
using namespace std;

#include "config.h"


class Date{
public:
    int year;
    int month;
    int day;
    Date(int y, int m, int d):year(y),month(m),day(d){}
};


class DateUtil{
public:
    const static int year_min = 1950;
    const static int year_max = 2020;
    const static int days_in_month[12];
    const static int days_before_month[12];

    static inline bool is_leap_year(int year)
    {
        if(year % 100 == 0)
        {
            return year/100 % 4 == 0;
        }
        return year%4 == 0;
    }

    static Date get_a_random_date()
    {
        int year = rand()%(year_max-year_min) + year_min;
        int month = rand()%12 + 1;

        int max_days =  days_in_month[month-1];
        if(month == 2 && is_leap_year(year))
        {
            max_days = days_in_month[month-1] + 1;
        }
        int day = rand()%max_days + 1;
        return Date(year, month, day);
    }


    static int calc_birthday_number(int month, int day)
    {
        int ret = 0;
        ret += days_before_month[month-1];
        ret += day;
        return ret;
    }
};

const int DateUtil::days_before_month[12] = {0, 31,59,90,120,151,181,212,243,273,304,334};
const int DateUtil::days_in_month[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

class IDGenerator{
public:
    const static int area_code_min = 000000;
    const static int area_code_max = 1000000;

    const static int id_suffix_min = 0000;
    const static int id_suffix_max = 10000;

    
    unordered_set<string> uo_st;
    std::unordered_set<std::string>::iterator it;

    IDGenerator()
    {
        while(uo_st.size() < N)
        {
            uo_st.insert(init_generate_a_random_id());
        }
        it=uo_st.begin();
    }

    string get_next_random_id() {
        if(it != uo_st.end())
        {
            auto id_string = *it;
            it++;
            return id_string;
        }
        else {
            printf("Overflow Error\n");
            return string();
        }
    }

    void show_first_ten_ids()
    {
        int cnt = 0;
        for(auto x : uo_st) {
            cout << x << "\n";
            cnt ++;
            if(cnt > 10) break;
        }
    }
    
private:
    string init_generate_a_random_id() {
        int area_code = rand()%(area_code_max-area_code_min) + area_code_min;
        Date date = DateUtil::get_a_random_date();

        int id_suffix = rand()%(id_suffix_max) + id_suffix_min;
        char tmp[19];
        sprintf(tmp, "%06d%04d%02d%02d%04d", area_code, date.year, date.month, date.day, id_suffix);
        return string(tmp);
    }
};