struct FibNode
{
    int key;
    bool marked;
    int degree;
    FibNode *b, *f, *p, *c;
    
    FibNode()
    {
        this -> key = 0;
        this -> marked = false;
        this -> degree = 0;
        this -> b = this -> f = this -> p = this -> c = NULL;
    }
    
    FibNode(int key)
    {
        this -> key = key;
        this -> marked = false;
        this -> degree = 0;
        this -> b = this -> f = this -> p = this -> c = NULL;
    }
};

class FibHeap
{
    FibNode *min;
    int N;
    
public:
    FibHeap();
    FibHeap(FibNode*);
    bool isEmpty();
    void insert(FibNode*);
    void merge(FibHeap*);
    FibNode* first();
    FibNode* extractMin();
    void decreaseKey(FibNode*, int);
    void Delete(FibNode*);
};

FibHeap::FibHeap()
{
    this -> min = NULL;
    this -> N = 0;
}

FibHeap::FibHeap(FibNode *n)
{
    this -> min = n;
    n -> b = n -> f = n;
    n -> p = n -> c = NULL;
    
    this -> N = 1;
}

bool FibHeap::isEmpty()
{
    return (this -> min == NULL);
}

void FibHeap::insert(FibNode *n)
{
    this -> merge(new FibHeap(n));
}

void FibHeap::merge(FibHeap *h)
{
    this -> N += h -> N;
    if (h -> isEmpty()) return;
    if (this -> isEmpty())
    {
        this -> min = h -> min;
        return;
    }
    FibNode *first1 = this -> min;
    FibNode *last1 = this -> min -> b;
    FibNode *first2 = h -> min;
    FibNode *last2 = h -> min -> b;
    first1 -> b = last2;
    last1 -> f = first2;
    first2 -> b = last1;
    last2 -> f = first1;
    if (h -> min -> key < this -> min -> key) this -> min = h -> min;
}

FibNode* FibHeap::first()
{
    return this -> min;
}

FibNode* FibHeap::extractMin()
{
    FibNode *ret = this -> min;
    this -> N = this -> N - 1;
    
    if (ret -> f == ret)
    {
        this -> min = NULL;
    }
    else
    {
        FibNode *prev = ret -> b;
        FibNode *next = ret -> f;
        prev -> f = next;
        next -> b = prev;
        this -> min = next; // Not necessarily a minimum. This is for assisting with the merge w/ min's children.
    }
    
    if (ret -> c != NULL)
    {
        FibNode *firstChd = ret -> c;
        FibNode *currChd = firstChd;
        
        do
        {
            currChd -> p = NULL;
            currChd = currChd -> f;
        } while (currChd != firstChd);
        
        if (this -> isEmpty())
        {
            this -> min = firstChd;
        }
        else
        {
            FibNode *first1 = this -> min;
            FibNode *last1 = this -> min -> b;
            FibNode *first2 = firstChd;
            FibNode *last2 = firstChd -> b;
            first1 -> b = last2;
            last1 -> f = first2;
            first2 -> b = last1;
            last2 -> f = first1;
        }
    }
    
    if (this -> min != NULL)
    {
        int maxAuxSize = 5 * (((int)log2(this -> N + 1)) + 1);
        FibNode *aux[maxAuxSize + 1];
        for (int i=0;i<=maxAuxSize;i++) aux[i] = NULL;
        int maxDegree = 0;
        
        FibNode *curr = this -> min;
        
        do
        {
            FibNode *next = curr -> f;
            int deg = curr -> degree;
            FibNode *P = curr;
            while (aux[deg] != NULL)
            {
                FibNode *Q = aux[deg];
                aux[deg] = NULL;
                
                if (P -> key > Q -> key)
                {
                    FibNode *tmp = P;
                    P = Q;
                    Q = tmp;
                }
                
                Q -> p = P;
                if (P -> c == NULL)
                {
                    P -> c = Q;
                    Q -> b = Q -> f = Q;
                }
                else
                {
                    FibNode *last = P -> c -> b;
                    last -> f = Q;
                    Q -> b = last;
                    P -> c -> b = Q;
                    Q -> f = P -> c;
                }
                
                deg++;
                P -> degree = deg;
            }
            aux[deg] = P;
            if (deg > maxDegree) maxDegree = deg;
            curr = next;
        } while (curr != this -> min);
        
        
        FibNode *previous = aux[maxDegree];
        this -> min = previous;
        for (int i=0;i<=maxDegree;i++)
        {
            if (aux[i] != NULL)
            {
                previous -> f = aux[i];
                aux[i] -> b = previous;
                if (aux[i] -> key < this -> min -> key) this -> min = aux[i];
                previous = aux[i];
            }
        }
        
    }
    
    return ret;
}

void FibHeap::decreaseKey(FibNode *n, int newKey)
{
    // Precondition: newKey < n -> key
    n -> key = newKey;
    
    FibNode *curr = n;
    if (curr -> p != NULL)
    {
        if (curr -> key < curr -> p -> key)
        {
            FibNode *parent = curr -> p;
            curr -> marked = false;
            
            curr -> p = NULL;
            if (curr -> f == curr) parent -> c = NULL;
            else
            {
                FibNode *prev = curr -> b;
                FibNode *next = curr -> f;
                prev -> f = next;
                next -> b = prev;
                if (parent -> c == curr) parent -> c = prev;
            }
            parent -> degree = parent -> degree - 1;
            
            FibNode *last = this -> min -> b;
            last -> f = curr;
            curr -> b = last;
            this -> min -> b = curr;
            curr -> f = this -> min;
            
            if (curr -> key < this -> min -> key) this -> min = curr;
            
            while (parent -> p != NULL && parent -> marked)
            {
                curr = parent;
                parent = curr -> p;
                curr -> marked = false;
                
                curr -> p = NULL;
                if (curr -> f == curr) parent -> c = NULL;
                else
                {
                    FibNode *prev = curr -> b;
                    FibNode *next = curr -> f;
                    prev -> f = next;
                    next -> b = prev;
                    if (parent -> c == curr) parent -> c = prev;
                }
                parent -> degree = parent -> degree - 1;
                
                FibNode *last = this -> min -> b;
                last -> f = curr;
                curr -> b = last;
                this -> min -> b = curr;
                curr -> f = this -> min;
                
            }
            if (parent -> p != NULL) parent -> marked = true;
        }
    }
    else if (n -> key < this -> min -> key) this -> min = n;
}

void FibHeap::Delete(FibNode *n)
{
    this -> decreaseKey(n, -INT_MAX);
    this -> extractMin();
}

FibHeap *fh;

long long kruskal_F(ll vertices,ll edges, unordered_map< ll, pair<ll,vector<vector<ll>>>> q)
{   
    vector<vector<ll>> result(vertices);
    ll e = 0; 
    struct subset *subsets = (struct subset*) malloc( vertices * sizeof(struct subset) ); 
    ll x =-1;
    vector<ll> next_edge(3);
    for (ll i=0;i<vertices;i++) 
    {   subsets[i].parent = i; 
        subsets[i].rank = 0; 
    }
    while (e < vertices - 1) 
    {   x = fh -> extractMin() -> key;
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
    if(e!=vertices-1)
    {   printf("No MST exists..");
	return -1; 
    }
    printf("Following are the edges included the constructed MST in sorted order..\n"); 
    long long s=0;
    for (ll i=0;i<e;i++) 
    {	printf("%lld -- %lld == %lld\n", result[i][0], result[i][1], result[i][2]);
	s+=result[i][2];
    } 
    return s;
}

ll Kruskal_using_Fibonacci(ll n,ll m,vector<ll> data,unordered_map< ll, pair<ll,vector<vector<ll>>>> q)
{	fh = new FibHeap();
    for(ll i=0;i<m;i++)
    	fh -> insert(new FibNode(data[i]));
    kruskal_F(n,m,q);
    delete(fh);
}

// int main()
// {   
//     long long n,m,a,b,c;
//     unordered_map< long long, pair<long long,vector<vector<long long>>>> q;
//     long long i;
//     scanf("%lld%lld",&n,&m);
//     vector<long long> temp(3),z;
//     for(i=0;i<m;i++)
//     {   scanf("%lld%lld%lld",&temp[0],&temp[1],&temp[2]);
//         z.push_back(temp[2]);
//         q[temp[2]].first++;
//         q[temp[2]].second.push_back(temp);
//     }
//    	Kruskal_using_Fibonacci(n,m,z,q);
//     return 0;
// }
