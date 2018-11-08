#include <bits/stdc++.h>
using namespace std;
#include "veb.h"
#define ll long long

int main()
{	long long n,m;
	unordered_map< long long, pair<long long,vector<vector<long long>>>> q;
	long long i;
	scanf("%lld%lld",&n,&m);
	vector<vector<long long>> z;
	vector<long long> temp(3);
	long long max=-1;
	long long size[6]={2,4,16,256,65536,4294967296};
	for(i=0;i<m;i++)
	{	scanf("%lld%lld%lld",&temp[0],&temp[1],&temp[2]);
		z.push_back(temp);
		if(temp[2]>max)
			max=temp[2];
	}
	for(i=0;i<6;i++)
		if(size[i]>max)
		{	max=size[i];
			break;
		}
	vEB = new class vEB(max);
    for(int i=0;i<m;i++)
    {	vEB -> Insert(z[i][2]);
    	q[z[i][2]].first++;
    	q[z[i][2]].second.push_back(z[i]);
    }
    // long long x=-1;
    // for(int i=0;i<4;i++)
    // {	x=*vEB->successor(x);
    // 	cout<<x<<endl;
    // }
    kruskal(n,m,q);
    return 0;
}	