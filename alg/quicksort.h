#ifndef QUICKSORT_H
#define QUICKSORT_H

#include <cstdio>
#include <random>
#include <utility>

void quicksort(int * arr, unsigned int sz)
{
    typedef std::uniform_int_distribution<unsigned int> uniform_distribution;
    unsigned int l = 0;
    unsigned int r = sz-1;
    bool hasSwapped = false;

    std::random_device rd;
    std::mt19937 gen{rd()};
    uniform_distribution dist{0,sz-1};
    const unsigned int idx = dist(gen);
    const int pivot = arr[idx];

    printf("pivot=%d\n",pivot);
    while(l != r)
    {
        while(arr[l] < pivot && l != r)
        {
            ++l;
            printf("    pivot=%d, l=%d, arr[l]=%d\n",pivot,l,arr[l]);
        }
        while(arr[r] > pivot && l != r)
        {
            --r;
            printf("    pivot=%d, r=%d, arr[r]=%d\n",pivot,r,arr[r]);
        }

        if(r != l)
        {
            printf("pivot=%d: swapping %d,%d at positions %d,%d\n",pivot,arr[l],arr[r],l,r);
            std::swap(arr[l],arr[r]);
            hasSwapped = true;
            ++l;
        }
    }

    if(hasSwapped)
    {
        if(l > 1)
        {
            printf("sorting subset 0,%d\n",l);
            quicksort(arr,l);
        }
        if(sz-r-1 > 1)
        {
            printf("sorting subset %d,%d\n",r,sz-r);
            quicksort(arr+r,sz-r);
        }
    }
}

void test_quicksort()
{
    int numbers[] = {14,-23,133,4,12,0,-1,-44,4,1,1};

    quicksort(numbers, 11);

    for(unsigned int i = 0; i < 11; ++i)
    {
        printf("%d\n",numbers[i]);
    }
}


#endif // QUICKSORT_H
