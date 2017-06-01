#ifndef WEIGHTED_INTERVAL_SCHEDULING_H
#define WEIGHTED_INTERVAL_SCHEDULING_H

#include <algorithm>
#include <vector>

namespace weighted_interval_scheduling
{
    struct interval
    {
        interval(int s, int f, int w) : s(s), f(f), w(w) {}
        ~interval(){}
        int s;
        int f;
        unsigned int w;
    };


    std::vector<interval*> weighted_interval_scheduling(const std::vector<interval*> & in)
    {
        struct interval_
        {
            unsigned int    id;
            interval*       i;
        };


        std::vector<int> p;
        std::vector<unsigned int> opt;
        std::vector<interval_> start;
        std::vector<interval_> finish;
        std::vector<unsigned int> start_finish_map;

        const unsigned int N = in.size();
        start.reserve(N);
        finish.reserve(N);
        start_finish_map.reserve(N);
        p.reserve(N);
        opt.reserve(N);

        for(unsigned int i = 0; i < N; ++i)
        {
            finish.push_back({i,in.at(i)});
            start.push_back({i,in.at(i)});
        }
        std::sort(start.begin(),start.end(), [](const interval_& l, const interval_& r)
        {
            return (l.i->f - l.i->s) < (r.i->f - r.i->s);
        });
        std::sort(finish.begin(),finish.end(), [](const interval_& l, const interval_& r)
        {
            return l.i->f < r.i->f;
        });

        for(unsigned int i = 0; i < N; ++i)
        {
            start_finish_map[finish[i].id] = i;
        }

        int biggest_f = -1;
        for(unsigned int j = 0; j < N; ++j)
        {
            interval_ cs = start[j];
            while(cs.i->f - cs.i->s > finish[biggest_f+1].i->f)
                ++biggest_f;

            p[start_finish_map[cs.id]] = biggest_f;
        }
        opt[0] = finish[0].i->w;
        for(unsigned int i = 1; i < N; ++i)
        {
            if( p[i] < 0)
                opt[i] = finish[i].i->w >= opt[i-1] ? finish[i].i->w : opt[i-1];
            else
                opt[i] = finish[i].i->w + opt[p[i]] >= opt[i-1] ? finish[i].i->w + opt[p[i]] : opt[i-1];
        }

        std::vector<interval*> r;
        unsigned int W = opt[N-1];
        for(unsigned int i = N-1; i > 0; --i)
        {
            if(p[i] < 0)
            {
                if(W - finish[i].i->w == 0)
                {
                    r.push_back(finish[i].i);
                    W = W-finish[i].i->w;
                }
            }
            else
            {
                if(W - finish[i].i->w == opt[p[i]])
                {
                    r.push_back(finish[i].i);
                    W = W-finish[i].i->w;
                }
            }
        }
        if(W-finish[0].i->w == 0)
            r.push_back(finish[0].i);

        return r;
    }
}

void test_weighted_interval_scheduling()
{
    typedef weighted_interval_scheduling::interval interval_t;
    interval_t i1{24,37,7};
    interval_t i2{20,30,4};
    interval_t i3{10,45,1};
    interval_t i4{10,15,2};
    interval_t i5{9,42,2};
    interval_t i6{9,31,4};
    std::vector<interval_t*> vec{&i1,&i2,&i3,&i4,&i5,&i6};
    std::vector<interval_t*> r = weighted_interval_scheduling::weighted_interval_scheduling(vec);
    unsigned int W = 0;
    printf("The intervals in the optimal solution are\n");
    for(interval_t* i : r)
    {
        W+=i->w;
        printf("\t{[%d,%d],%d}\n",i->f-i->s,i->f,i->w);
    }
    printf("with the total weight of %d.\n",W);
}


#endif // WEIGHTED_INTERVAL_SCHEDULING_H
