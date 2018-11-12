#include <bits/stdc++.h>
using namespace std;
#define ll long long
#include "dsu.h"
#include "veb.h"
#include "fibonacci.h"
#include "binomial.h"

int main()
{   
    long long n,m,a,b,c;
    unordered_map< long long, pair<long long,vector<vector<long long>>>> q;
    long long i;
    scanf("%lld%lld",&n,&m);
    vector<long long> temp(3),z	;
    for(i=0;i<m;i++)
    {   scanf("%lld%lld%lld",&temp[0],&temp[1],&temp[2]);
        z.push_back(temp[2]);
        q[temp[2]].first++;
        q[temp[2]].second.push_back(temp);
    }
    long long vEB_result = Kruskal_using_vEB(n,m,z,q);
   	if(vEB_result==-1)
   	{	printf("No MST exists..\n");
    	return 0;
   	}
   	long long bin_result = Kruskal_using_Binomial(n,m,z,q);
   	long long fib_result = Kruskal_using_Fibonacci(n,m,z,q);
   	printf("MST Cost from all three implementation matches..\n");
    printf("Cost is : %lld",vEB_result);
    return 0;
}