#ifndef INTERVAL_SCHEDULING_H
#define INTERVAL_SCHEDULING_H


#include <algorithm>
#include <vector>

namespace interval_scheduling
{
    struct interval
    {
        interval(int s, int f) : s(s), f(f) {}
        ~interval(){}
        int s;
        int f;
    };


    std::vector<interval*> interval_scheduling(const std::vector<interval*> & in)
    {
        struct interval_
        {
            unsigned int    id;
            interval*       i;
        };

        /* Choose the interval with the earliest finish, then remove all intersecting intervals. Then repeat. */

        std::vector<int> p;
        std::vector<unsigned int> opt;
        std::vector<interval_> start;
        std::vector<interval_> finish;
        std::vector<unsigned int> start_finish_map;

        const unsigned int N = in.size();
        finish.reserve(N);
        opt.reserve(N);

        for(unsigned int i = 0; i < N; ++i)
        {
            finish.push_back({i,in.at(i)});
        }
        std::sort(finish.begin(),finish.end(), [](const interval_& l, const interval_& r)
        {
            return l.i->f < r.i->f;
        });
        
        int biggest_f = finish[0].i->f;
        unsigned int f_idx = 0;
        opt.push_back(finish[0].id);
        
        while(f_idx < N-1)
        {
            // Increment f_idx by exactly one each iteration.
            ++f_idx;
            if(!(finish[f_idx].i->s < biggest_f))
            {
                opt.push_back(finish[f_idx].id);
                biggest_f = finish[f_idx].i->f;
            }
        }
        
        std::vector<interval*> r;
        for(unsigned int opt_id : opt)
        {
            r.push_back(in[opt_id]);
        }
        
        return r;
    }
}

void test_interval_scheduling()
{
    typedef interval_scheduling::interval interval_t;
    interval_t i1{24,37};
    interval_t i2{20,30};
    interval_t i3{10,45};
    interval_t i4{10,15};
    interval_t i5{9,42};
    interval_t i6{9,31};
    std::vector<interval_t*> vec{&i1,&i2,&i3,&i4,&i5,&i6};
    std::vector<interval_t*> r = interval_scheduling::interval_scheduling(vec);
    printf("\n\nTesting interval scheduling, greedy algorithm.\n\n");
    printf("The intervals in the optimal solution are\n");
    for(interval_t* i : r)
    {
        printf("\t[%d,%d]\n",i->s,i->f);
    }
    printf("\n");
}












#endif /* INTERVAL_SCHEDULING_H */