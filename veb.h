#include <math.h>
#include <iostream>
using namespace std;
#define ll long long
#include "dsu.h"

class vEB
{       ll u;
        ll *minimum, *maximum;
        vEB *summary;
        vEB **cluster;  
    public:
        vEB(ll);
        void Insert(ll);
        void Remove(ll);
        ll min();
        ll max();
        ll* predecessor(ll);
        ll* successor(ll);
        ll extractMin();
};

vEB::vEB(ll u)
{   this -> u = u;
    this -> minimum = NULL;
    this -> maximum = NULL;
    if (u == 2)
    {   this -> summary = NULL;
        this -> cluster = NULL;
    }
    else
    {   ll subSize = (ll)sqrt(u);
        this -> summary = new vEB(subSize);
        this -> cluster = new vEB*[subSize];
    }
}

void vEB::Insert(ll data)
{   if (u == 2)
    {   if (!data)
        {   if (!minimum)
            {   minimum = new ll;
                maximum = new ll;
                *minimum = *maximum = data;
            }
            else 
                *minimum = data;
        }
        else if (data)
        {   if (!maximum)
            {   minimum = new ll;
                maximum = new ll;
                *minimum = *maximum = data;
            }
            else 
                *maximum = data;
        }
    }
    else
    {   if (!minimum)
        {   minimum = new ll;
            maximum = new ll;
            *minimum = *maximum = data;
        }
        else
        {   if (data < *minimum)
            {   ll temp = *minimum;
                *minimum = data;
                this -> Insert(temp);
            }
            else
            {   if (data > *maximum) 
                    *maximum = data;
                ll subSize = (ll)sqrt(u);
                ll hi = data/subSize, lo = data%subSize;
                if (!cluster[hi])
                {   cluster[hi] = new vEB(subSize);
                    cluster[hi] -> Insert(lo);
                    summary -> Insert(hi);
                }
                else 
                    cluster[hi] -> Insert(lo);
            }
        }
    }
}

void vEB::Remove(ll data)
{   if (u == 2)
    {   if (!data)
        {   if (!*maximum)
                minimum = maximum = NULL;
            else 
                *minimum=1;
        }
        else if (data)
        {   if (*minimum)
                minimum = maximum = NULL;
            else 
                *maximum=0;
        }
    }
    else
    {   ll subSize = (ll)sqrt(u);
        ll hi=data/subSize, lo=data%subSize;
        if (data == *minimum)
        {   if (data == (*maximum))
            {   minimum = maximum = NULL;
            }
            else
            {   ll nextMinHi = summary -> min();
                ll nextMinLo = cluster[summary -> min()] -> min();
                ll nextMin = nextMinHi * subSize + nextMinLo;
                this -> Remove(nextMin);
                (*minimum) = nextMin;
            }
        }
        else
        {   cluster[hi] -> Remove(lo);
            if (cluster[hi] -> minimum == NULL)
            {   summary -> Remove(hi);
                delete cluster[hi];
                cluster[hi] = NULL;
            }
            if (data == *maximum)
            {   if (summary -> minimum == NULL) 
                    *maximum = *minimum;
                else
                {   ll nextMaxHi = summary -> max();
                    ll nextMaxLo = cluster[summary -> max()] -> max();
                    *maximum = nextMaxHi * subSize + nextMaxLo;
                }
            }
        }
    }
}

ll vEB::min()
{   return *minimum;
}

ll vEB::max()
{   return *maximum;
}

ll* vEB::predecessor(ll data)
{   if (u == 2)
    {   if (data)
        {   if (minimum == NULL || *minimum == 1) 
                return NULL;
            return minimum;
        }
        else 
            return NULL;
    }
    else
    {   if (minimum == NULL || data <= *minimum) 
            return NULL;
        if (data > *maximum) 
            return maximum;
        ll subSize = (ll)sqrt(u);
        ll hi = data/subSize;
        ll lo = data%subSize;
        if (!cluster[hi])
        {   ll* prev = summary -> predecessor(hi);
            ll* ret = new ll;
            (*ret) = (*prev) * subSize + cluster[(*prev)] -> max();
            return ret;
        }
        else
        {   ll *newLo, *newHi;
            newHi = new ll;
            newLo = new ll;
            *newHi = hi;
            ll minInCluster = cluster[hi] -> min();
            if (lo > minInCluster) 
                newLo = cluster[hi] -> predecessor(lo);
            else
            {   newHi = summary -> predecessor(hi);
                *newLo = cluster[*newHi] -> max();
            }
            ll *ret = new ll;
            *ret = *newHi * subSize + *newLo;
            return ret;
        }
    }
}

ll* vEB::successor(ll data)
{   if (u == 2)
    {   if (!data)
        {   if (!maximum || !*maximum) 
                return NULL;
            return maximum;
        }
        else 
            return NULL;
    }
    else
    {   if (!minimum || data >= *maximum) 
            return NULL;
        if (data < *minimum) 
            return minimum;
        ll subSize = (ll)sqrt(u);
        ll hi = data/subSize;
        ll lo = data%subSize;
        if (cluster[hi] == NULL)
        {   ll* next = summary -> successor(hi);
            ll* ret = new ll;
            *ret = *next * subSize + cluster[*next] -> min();
            return ret;
        }
        else
        {   ll *newLo, *newHi;
            newHi = new ll;
            newLo = new ll;
            *newHi = hi;
            ll maxInCluster = cluster[hi] -> max();
            if (lo < maxInCluster) 
                newLo = cluster[hi] -> successor(lo);
            else
            {   newHi = summary -> successor(hi);
                *newLo = cluster[*newHi] -> min();
            }
            ll *ret = new ll;
            *ret = *newHi * subSize + *newLo;
            return ret;
        }
    }
}

ll vEB::extractMin()
{   ll ret = this -> min();
    this -> Remove(ret);
    return ret;
}

vEB *vEB;

void kruskal(long long n, long long m, unordered_map< long long, pair<long long,vector<vector<long long>>>> q)
{   int V = n; 
    vector<vector<long long>> result(V);
    int e = 0; 
    struct subset *subsets = (struct subset*) malloc( V * sizeof(struct subset) ); 
    long long x =-1;
    vector<long long> next_edge(3);
    
    for (ll i=0;i<V;i++) 
    {   subsets[i].parent = i; 
        subsets[i].rank = 0; 
    }
        
    while (e < V - 1) 
    {   if(!q[x].first)
            x = *vEB->successor(x);
        q[x].first--; 
        next_edge = q[x].second[q[x].first];
        //cout<<next_edge[0]<<" "<<next_edge[1]<<" "<<next_edge[2]<<endl;
        int x = find(subsets, next_edge[0]); 
        int y = find(subsets, next_edge[1]); 
        if (x != y) 
        {   result[e++] = next_edge; 
            Union(subsets, x, y); 
        } 
    } 
    printf("Following are the edges included the constructed MST in sorted order..\n"); 
    for (ll i=0;i<e;i++) 
        printf("%lld -- %lld == %lld\n", result[i][0], result[i][1], result[i][2]); 
    return;
}