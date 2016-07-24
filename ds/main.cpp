#include <cstdio>
#include "priority_queue.h"
#include "sorted_array.h"
#include "../tmp/tmp_test.h"

int main()
{
    /*alg::ds::priority_queue<int,100> pq;
    int a = 5;
    pq.insert(4);
    pq.insert(3);
    pq.insert(2);
    pq.insert(8);
    pq.insert(19);
    pq.insert(a);
    pq.insert(4);
    pq.insert(3);
    printf("%d\n",pq.element());
    pq.decrement();
    printf("%d\n",pq.element());
    pq.decrement();
    printf("%d\n",pq.element());
    pq.decrement();
    printf("%d\n",pq.element());
    pq.decrement();
    printf("%d\n",pq.element());
    pq.decrement();
    printf("%d\n",pq.element());
    pq.decrement();
    printf("%d\n",pq.element());
    pq.decrement();
    printf("%d\n",pq.element());
    pq.decrement();

    alg::ds::kv_priority_queue<int,const char *,100> pq1;
    pq1.insert({0,"noll"});
    pq1.insert({1,"ett"});
    pq1.insert({15,"femton"});
    pq1.insert({30,"trettio"});
    pq1.insert({20,"tjugo"});
    pq1.insert({5,"fem"});
    pq1.insert({4,"fyra"});
    printf("%s\n",pq1.element().value());
    pq1.decrement();
    printf("%s\n",pq1.element().value());
    pq1.decrement();
    printf("%s\n",pq1.element().value());
    pq1.decrement();
    printf("%s\n",pq1.element().value());
    pq1.decrement();
    printf("%s\n",pq1.element().value());
    pq1.decrement();
    printf("%s\n",pq1.element().value());
    pq1.decrement();
    printf("%s\n",pq1.element().value());
    pq1.decrement();

    alg::ds::sorted_array<int,100> sa;

    for(int i = 0; i < 50; ++i)
    {
        sa.insert(i);
    }

    sa.move_element(20, 30);
    sa.delete_element(25);
    sa.delete_element(0);
    for(int i = 0; i < sa.size(); ++i)
    {
        printf("%d\n",sa.get_element(i));
    }
    */
    test_all();

    return 0;
}
