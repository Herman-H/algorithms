#ifndef INVERSION_COUNT_H
#define INVERSION_COUNT_H

#include <vector>
#include <algorithm>

/* Two lists of rankings A and B of same size. Element at index i refers to the same object. */

namespace inversion_count
{
    typedef std::vector<unsigned int> rank_list;
    typedef std::pair<const unsigned int*,const unsigned int*> pair_t;
    typedef std::vector<pair_t> sorted_rank_list;
    
    unsigned int merge_sort_and_count(rank_list & A)
    {
        unsigned int N = A.size();
        if(N == 1)
            return 0;
        else if(N == 2)
        {
            unsigned int a_ = A[0];
            unsigned int b_ = A[1];
            if(a_ > b_)
            {
                A[0] = b_;
                A[1] = a_;
                return 1;
            }
            return 0;
        }
        unsigned int M = N/2;
        rank_list LOW{A.begin(),A.begin()+M};
        rank_list HIGH{A.begin()+M,A.end()};
        
        unsigned int low_inversions = merge_sort_and_count(LOW);
        unsigned int high_inversions = merge_sort_and_count(HIGH);
        
        unsigned int low_next = 0;
        unsigned int high_next = 0;
        
        unsigned int inversion_count = 0;
        A.clear();
        for(int i = 0; i < N; ++i)
        {
            if(low_next < M && LOW[low_next] <= HIGH[high_next])
            {
                A.push_back(LOW[low_next]);
                inversion_count += high_next;
                ++low_next;
            }
            else
            {
                A.push_back(HIGH[high_next]);
                ++high_next;
            }
        }
        
        return inversion_count+low_inversions+high_inversions;
    }
    
    unsigned int inversion_count(const rank_list & A, const rank_list & B)
    {
        sorted_rank_list sorted_by_B;
        for(unsigned int i = 0; i < A.size(); ++i)
        {
            sorted_by_B.push_back(pair_t{A.data()+i,B.data()+i});
        }
        std::sort(sorted_by_B.begin(), sorted_by_B.end(), [](pair_t L, pair_t R)
        {
            return *(L.second) < *(R.second);
        });
        
        rank_list A_unsorted;
        for(pair_t p : sorted_by_B)
        {
            A_unsorted.push_back(*(p.first));
        }
        
        return merge_sort_and_count(A_unsorted);
    }
}

void test_inversion_count()
{
    using namespace inversion_count;
    
    rank_list A1{1,2,3,4,5,6,7,8};
    rank_list B1{1,2,3,5,4,6,7,8};
    
    rank_list A2{3,4,2,5,1,8,7,6};
    rank_list B2{3,5,2,4,1,8,7,6};
    
    rank_list A3{1,2,3,4,5,6,7,8};
    rank_list B3{8,2,3,4,5,6,7,1};
    
    rank_list A4{2,1,3,4,5,6,7,8};
    rank_list B4{1,2,3,5,4,6,7,8};
    
    printf("\n\nTesting Inversion Count (nlogn).\n\n");
    printf("\t(1 2 3 4 5 6 7 8)\n");
    printf("\t(1 2 3 5 4 6 7 8)\n");
    printf("has 1 inversion and the algorithm found %d inversions.\n\n",inversion_count::inversion_count(A1,B1));
    printf("\n\t(3,4,2,5,1,8,7,6)\n");
    printf("\t(3,5,2,4,1,8,7,6)\n");
    printf("has 1 inversion and the algorithm found %d inversions.\n\n",inversion_count::inversion_count(A2,B2));
    printf("\n\t(1,2,3,4,5,6,7,8)\n");
    printf("\t(8,2,3,4,5,6,7,1)\n");
    printf("has 13 inversions and the algorithm found %d inversions.\n\n",inversion_count::inversion_count(A3,B3));
    printf("\n\t(2,1,3,4,5,6,7,8)\n");
    printf("\t(1,2,3,5,4,6,7,8)\n");
    printf("has 2 inversions and the algorithm found %d inversions.\n\n",inversion_count::inversion_count(A4,B4));
}


#endif /* INVERSION_COUNT_H */