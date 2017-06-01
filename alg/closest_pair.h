#ifndef CLOSEST_PAIR_H
#define CLOSEST_PAIR_H

#include <vector>
#include <algorithm>
#include <iostream>

namespace closest_pair
{
    typedef std::pair<double,double> point_t;
    typedef std::vector<point_t> point_vector;
    typedef std::vector<const point_t*> sorted_point_vector;
    typedef std::pair<point_t,point_t> pair_t;
    
    double dist(pair_t p)
    {
        const double x_dif = p.second.first  - p.first.first;
        const double y_dif = p.second.second  - p.first.second;
        return x_dif*x_dif + y_dif*y_dif;
    }
    
    pair_t closest_pair_(const sorted_point_vector & x_sorted, const sorted_point_vector & y_sorted, bool cut_by_x)
    {
        unsigned int N = x_sorted.size();
        if(N == 2)
        {
            return pair_t{*(x_sorted[0]), *(x_sorted[1])};
        }
        else if(N == 3)
        {
            pair_t a{*(x_sorted[0]), *(x_sorted[1])};
            pair_t b{*(x_sorted[0]), *(x_sorted[1])};
            pair_t c{*(x_sorted[0]), *(x_sorted[1])};
            
            return dist(a) <= dist(b) ? (dist(a) <= dist(c) ? a : c) : (dist(b) <= dist(c) ? b : c);
        }
        else if(N == 1)
        {
            return pair_t{point_t{0,0},point_t{0,0}};
        }
        
        sorted_point_vector first_x_sorted;
        sorted_point_vector first_y_sorted;
        sorted_point_vector second_x_sorted;
        sorted_point_vector second_y_sorted;
        
        unsigned int M = (N+1)/2;
        
        double mid_point = cut_by_x ? x_sorted[M]->first : y_sorted[M]->second;
        
        auto decide_list_x = [&](const point_t* p) -> sorted_point_vector&
        {
            if(cut_by_x && p->first < mid_point)
                return first_x_sorted;
            else if(cut_by_x)
                return second_x_sorted;
            else if(p->second < mid_point)
                return first_x_sorted;
            else
                return second_x_sorted;
        };
        
        auto decide_list_y = [&](const point_t* p) mutable -> sorted_point_vector&
        {
            if(cut_by_x && p->first < mid_point)
                return first_y_sorted;
            else if(cut_by_x)
                return second_y_sorted;
            else if(p->second < mid_point)
                return first_y_sorted;
            else
                return second_y_sorted;
        };
        
        for(auto i : x_sorted)
        {
            decide_list_x(i).push_back(i);
        }
        for(auto i : y_sorted)
        {
            decide_list_y(i).push_back(i);
        }

        pair_t pair_1 = closest_pair_(first_x_sorted,first_y_sorted,!cut_by_x); 
        pair_t pair_2 = closest_pair_(second_x_sorted,second_y_sorted,!cut_by_x);

        pair_t min_pair = dist(pair_1) <= dist(pair_2) ? pair_1 : pair_2;
        double d = dist(min_pair);
        
        sorted_point_vector S;
        const sorted_point_vector & from = cut_by_x ? x_sorted : y_sorted;

        for(auto i : from)
        {
            if(cut_by_x)
            {
                if(dist(pair_t{point_t{mid_point,i->second},*i}) < d)
                    S.push_back(i);
            }
            else
            {
                if(dist(pair_t{point_t{i->first,mid_point},*i}) < d)
                    S.push_back(i);
            }
        }
        
        pair_t s_min = min_pair;
        for(unsigned int i = 0; i < S.size(); ++i)
        {
            pair_t p_min = s_min;
            unsigned int j = i+1;
            while(j < i+16 && j < S.size())
            {
                pair_t p{*S[i],*S[j]};
                p_min = dist(p) < dist(p_min) ? p : p_min;
                ++j;
            }
            s_min = dist(p_min) < dist(s_min) ? p_min : s_min;
        }
        min_pair = dist(s_min) < dist(min_pair) ? s_min : min_pair;
        
        return min_pair;
    }
    
    pair_t closest_pair(const point_vector & points)
    {
        sorted_point_vector x_sorted;
        sorted_point_vector y_sorted;
        for(unsigned int i = 0; i < points.size(); ++i)
        {
            const point_t * p = &(points[i]);
            x_sorted.push_back(p);
            y_sorted.push_back(p);
        }
        
        std::sort(x_sorted.begin(),x_sorted.end(),[](const point_t* L, const point_t* R)
        {
            return (*L).first < (*R).first;
        });
        std::sort(y_sorted.begin(),y_sorted.end(),[](const point_t* L, const point_t* R)
        {
            return (*L).second < (*R).second;
        });
        return closest_pair_(x_sorted,y_sorted,true);
    }
}

void test_closest_pair()
{
    using namespace closest_pair;
    
    point_vector points;
    points.push_back(point_t{1,1});
    points.push_back(point_t{14,1});
    points.push_back(point_t{-14,1});
    points.push_back(point_t{13,5});
    points.push_back(point_t{-12,7});
    points.push_back(point_t{-21,-3});
    points.push_back(point_t{10,-2});
    points.push_back(point_t{7,6});
    points.push_back(point_t{13,8});
    
    printf("\n\nTesting Closest Pair algorithm (nlogn).\n");
    pair_t p = closest_pair::closest_pair(points);
    printf("The two closest points were found to be p1 = (%f,%f) and p2 = (%f,%f).\n",p.first.first,p.first.second,p.second.first,p.second.second);
}


#endif /* CLOSEST_PAIR_H */