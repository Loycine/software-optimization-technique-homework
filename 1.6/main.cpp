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

#include "config.h"
#include "utils.h"

#include <assert.h>


inline long long atoll_fix_length(char* id)
{
    long long ret = 0;
    for(int i=0; i<18; i++)
    {
        ret *= 10;
        ret += (id[i]-'0');
    }
    return ret;
}

class Method{
public:
    string method_name;

    virtual void person_insert(struct person*p, int N) = 0;
    virtual struct person *person_search(char *id) = 0;
};


class Baseline : public Method {
public:
    map<string, person> mp;

    Baseline()
    {
        method_name = "Solution1";
    }

    virtual void person_insert(struct person*p, int N)
    {
        for(int i=0; i<N; i++)
        {
            mp[string(p[i].id, 18)] = p[i];
        }
    }

    virtual struct person *person_search(char *id)
    {
        auto it = mp.find(string(id, 18)); 
        if(it != mp.end())
        {
            return &(it->second);
        }
        else
        {
            return nullptr;
        }
    }
};

class UsingArrayIndexAsValue : public Method {
public:
    map<string, int> mp;
    person* people;

    UsingArrayIndexAsValue()
    {
        method_name = "Solution2";
    }

    virtual void person_insert(struct person*p, int N)
    {
        for(int i=0; i<N; i++)
        {
            mp[string(p[i].id)] = i;
        }
        people = p;
    }

    virtual struct person *person_search(char *id)
    {
        auto it = mp.find(string(id));
        if(it != mp.end())
        {
            return &(people[(it->second)]);
        }
        else
        {
            return nullptr;
        }
    }
};


class UsingInt64AsKey : public Method {
public:
    map<long long, int> mp;
    person* people;

    UsingInt64AsKey()
    {
        method_name = "Solution3";
    }

    virtual void person_insert(struct person*p, int N)
    {
        for(int i=0; i<N; i++)
        {
            long long id_number = atoll_fix_length(p[i].id);
            mp[id_number] = i;
        }

        people = p;
    }

    virtual struct person *person_search(char *id)
    {
        long long id_number = atoll_fix_length(id);
        auto it = mp.find(id_number);
        if(it != mp.end())
        {
            return &(people[(it->second)]);
        }
        else
        {
            return nullptr;
        }
    }
};

class PreHashedByDate : public Method {
public:
    person* people;
    map<long long, int> hash_table[366];


    PreHashedByDate()
    {
        method_name = "Solution4";
    }

    inline int get_month(char* id)
    {
        int ret = 0;
        for(int i=10; i<12; i++)
        {
            ret *= 10;
            ret += (id[i]-'0');
        }
        return ret;
    }

    inline int get_day(char* id)
    {
        int ret = 0;
        for(int i=12; i<14; i++)
        {
            ret *= 10;
            ret += (id[i]-'0');
        }
        return ret;
    }

    inline int transform_id_to_date(char* id)
    {
        int month = get_month(id);
        int day = get_day(id);

        int date = DateUtil::calc_birthday_number(month, day);
        return date;
    }

    virtual void person_insert(struct person*p, int N)
    {
        for(int i=0; i<N; i++)
        {
            long long id_number = atoll_fix_length(p[i].id);
            int date = transform_id_to_date(p[i].id);
            hash_table[date-1][id_number] = i;
        }

        people = p;
    }

    virtual struct person *person_search(char *id)
    {
        long long id_number = atoll_fix_length(id);
        int date = transform_id_to_date(id);

        auto& mp = hash_table[date-1];
        auto it = mp.find(id_number);
        if(it != mp.end())
        {
            return &(people[(it->second)]);
        }
        else
        {
            return nullptr;
        }
    }
};


class MethodFactory
{
public:
    static Method* build()
    {
        if(method_name=="Solution1")
        {
            return new Baseline();
        }
        else if(method_name == "Solution2")
        {
            return new UsingArrayIndexAsValue();
        }
        else if(method_name == "Solution3")
        {
            return new UsingInt64AsKey();
        }
        else if(method_name == "Solution4")
        {
            return new PreHashedByDate();
        }
        else
        {
            return nullptr;
        }
    }
};

class Environment {
public:
    IDGenerator g;
    person* people;
    Method* method;

    double insert_time_result;
    double search_time_result;

    Environment()
    {
        method = MethodFactory::build();
        people = new person[N];
    }

    ~Environment()
    {
        delete method;
        delete people;
    }

    void init()
    {
        for(int i=0; i<N; i++)
        {
            auto x = g.get_next_random_id();

            strncpy(people[i].id, x.c_str(), 18);
        }
    }

    void run()
    {
        run_experiment_insert();
        run_experiment_search();
    }

    void run_experiment_insert()
    {
        high_resolution_clock::time_point t1 = high_resolution_clock::now();

        method->person_insert(people, N);

        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
        std::cout << "Insert took me " << time_span.count() << " seconds.";
        std::cout << std::endl;

        insert_time_result = time_span.count();
    }

    void run_experiment_search()
    {
        high_resolution_clock::time_point t1 = high_resolution_clock::now();

        for(int i=0; i<M; i++)
        {
            person* p = method->person_search(people[i].id);
            
            assert(check_if_it_is_the_same(p->id, people[i].id));
        }

        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
        std::cout << "Search took me " << time_span.count() << " seconds.";
        std::cout << std::endl;

        search_time_result = time_span.count();
    }

    void write_result()
    {
        ofstream output;
        output.open("result.txt", ios::app);
        output << N << "\t    " << M << "\t    " << method->method_name << "\t    " << insert_time_result << "\t    " << search_time_result << "\n";
        output.close();
    }

    bool check_if_it_is_the_same(char* a, char* b)
    {
        // cout << string(a,18) << "\n";
        // cout << string(b,18) << "\n";
        for(int i=0; i<18; i++)
        {
            if(a[i] != b[i])
            {
                return false;
            }
        }
        return true;
    }
};


void read_config(int argc, char **argv)
{
    if(argc > 1 && argc == 4)
    {
        N = atoi(argv[1]);
        M = atoi(argv[2]);
        method_name = argv[3];
    }
}

int main(int argc, char **argv)
{
    read_config(argc, argv);
    Environment env;
    env.init();
    env.run();
    env.write_result();
}