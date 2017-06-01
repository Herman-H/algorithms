#ifndef CELEBRITY_H
#define CELEBRITY_H

#include <array>
#include <set>

namespace celebrity
{

    /* An algorithm to find a node that is connected to by everyone else and which don't connect to any other node. */

    template <unsigned int N>
    struct knows
    {
        typedef std::array<bool,N> knows_nodes_t;
        typedef std::array<knows_nodes_t,N> nodes_t;
        nodes_t m_nodes;
    private:
        bool get(unsigned int i, unsigned int j, const nodes_t &p) const { return (p[i])[j]; }
        
        void init_node(unsigned int node) { auto& n = m_nodes[node]; for(unsigned int i = 0; i < N; ++i) n[i] = false; }
    public:
        bool operator()(unsigned int i, unsigned int j) const { return get(i,j,m_nodes); }
        
        template <long long unsigned int A>
        void set_node_knows(unsigned int node, const std::array<unsigned int,A>& knows)
        {
            init_node(node);
            knows_nodes_t& n = m_nodes[node];
            for(unsigned int i = 0; i < A; ++i)
            {
                n[knows[i]] = true;
            }
        }
        void set_node_knows(unsigned int node, const std::array<unsigned int,0>& knows)
        {
            init_node(node);
        }
        void set_node_knows(unsigned int node)
        {
            init_node(node);
        }
        
        knows()
        {
            for(unsigned int i = 0; i < N; ++i) init_node(i);
        }
        ~knows(){}
    };

    template <unsigned int N>
    int celebrity(const knows<N>& k, std::set<unsigned int>& s)
    {
        if(s.size() == 1)
            return *(s.begin());
        else
        {
            auto it = s.begin();
            unsigned int i = *it;
            unsigned int j = *(++it);
            
            if(k(i,j)) // i is not a celebrity since it knows j
            {
                s.erase(s.begin());
                int p = celebrity(k,s); // We can go to the next iteration with a decreased set.
                if(p != -1 && (k(i,p) && !(k(p,i)))) // But we must still check if the removed element invalidates the finding.
                {
                    return p;
                }
                else
                {
                    return -1;
                }
            }
            else // j is not a celebrity since i does not know it 
            {
                s.erase(it);
                int p = celebrity(k,s); // We can go to the next iteration with a decreased set.
                if(p != -1 && (k(j,p) && !(k(p,j)))) // But we must still check if the removed element invalidates the finding.
                {
                    return p;
                }
                else
                {
                    return -1;
                }
            }
        }
    }
    
    template <unsigned int ... VS>
    struct init_array
    {
        constexpr std::array<unsigned int, sizeof...(VS)> operator()() { return std::array<unsigned int,sizeof...(VS)>{VS...}; }
    };
}
template <unsigned int ... VS>
using arr = celebrity::init_array<VS...>;

template <unsigned int V>
constexpr unsigned int ui = V;

void test_celebrity()
{
    typedef celebrity::knows<4> knows_t;
    typedef std::set<unsigned int> set_t;
    /* Test three different cases
     *
     *  (1) There is a celebrity in the set.
     *  (2) Everyone is knows someone.
     *  (3) No one knows anyone.
     */
     set_t nodes{0,1,2,3};
     printf("Testing the O(n) \"celebrity algorithm\".\n\n");
     // (1)
     knows_t k1;
     k1.set_node_knows(0, arr<1,2,3>()());
     k1.set_node_knows(1);
     k1.set_node_knows(2, arr<1>()());
     k1.set_node_knows(3, arr<0,1>()());
     
     auto n1 = nodes;
     printf("(1) Find a celebrity.\n");
     int c1 = celebrity::celebrity(k1,n1);
     printf("%s",c1 == -1 ? "No celebrity was found.\n" : (c1 == 1) ? "The correct celebrity was found.\n" : "The wrong celebrity was found." );
     ((c1 != 1 && c1 != -1) ? printf(". The algorithm selected %d as celebrity.\n",c1) : int());
     
     // (2)
     knows_t k2;
     k2.set_node_knows(0, arr<1,2,3>()());
     k2.set_node_knows(1, arr<2>()());
     k2.set_node_knows(2, arr<1>()());
     k2.set_node_knows(3, arr<0,1>()());
     
     auto n2 = nodes;
     printf("\n(2) Don't find a celebrity.\n");
     int c2 = celebrity::celebrity(k2,n2);
     printf("%s",c2 == -1 ? "No celebrity was found, as expected.\n" : "A celebrity was found, which is not correct." );
     (c2 != -1) ? printf(". The algorithm selected %d as celebrity.\n",c2) : int();
     
     // (3)
     knows_t k3;
     k3.set_node_knows(0);
     k3.set_node_knows(1);
     k3.set_node_knows(2);
     k3.set_node_knows(3);
     
     auto n3 = nodes;
     printf("\n(3) Don't find a celebrity.\n");
     int c3 = celebrity::celebrity(k3,n3);
     printf("%s",c3 == -1 ? "No celebrity was found, as expected.\n" : "A celebrity was found, which is not correct." );
     (c3 != -1) ? printf(". The algorithm selected %d as celebrity.\n",c3) : int();
     
}


#endif /* CELEBRITY_H */