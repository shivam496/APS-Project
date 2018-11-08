#include <bits/stdc++.h>
using namespace std;
#define ll long long

struct BinNode
{	int key;
    int degree;
    BinNode *f, *p, *c;
    BinNode()
    {	this -> key = 0;
        this -> degree = 0;
        this -> f = this -> p = this -> c = NULL;
    }
    BinNode(int key)
    {	this -> key = key;
        this -> degree = 0;
        this -> f = this -> p = this -> c = NULL;
    }
};

class BinHeap
{	BinNode *roots;
    BinNode *min;
    void linkTrees(BinNode*, BinNode*);
    BinNode* mergeRoots(BinHeap*, BinHeap*);
public:
    BinHeap();
    BinHeap(BinNode*);
    bool isEmpty();
    void insert(BinNode*);
    void merge(BinHeap*);
    BinNode* first();
    BinNode* extractMin();
    void decreaseKey(BinNode*, int);
    void Delete(BinNode*);
};

BinHeap::BinHeap()
{	this -> roots = NULL;
}

BinHeap::BinHeap(BinNode *x)
{
    this -> roots = x;
}

bool BinHeap::isEmpty()
{
    return (this -> roots == NULL);
}

void BinHeap::insert(BinNode *x)
{
    this -> merge(new BinHeap(x));
}

void BinHeap::linkTrees(BinNode *y, BinNode *z)
{
    // Precondition: y -> key >= z -> key
    y -> p = z;
    y -> f = z -> c;
    z -> c = y;
    z -> degree = z -> degree + 1;
}

BinNode* BinHeap::mergeRoots(BinHeap *x, BinHeap *y)
{
    BinNode *ret = new BinNode();
    BinNode *end = ret;
    
    BinNode *L = x -> roots;
    BinNode *R = y -> roots;
    if (L == NULL) return R;
    if (R == NULL) return L;
    while (L != NULL || R != NULL)
    {
        if (L == NULL)
        {
            end -> f = R;
            end = end -> f;
            R = R -> f;
        }
        else if (R == NULL)
        {
            end -> f = L;
            end = end -> f;
            L = L -> f;
        }
        else
        {
            if (L -> degree < R -> degree)
            {
                end -> f = L;
                end = end -> f;
                L = L -> f;
            }
            else
            {
                end -> f = R;
                end = end -> f;
                R = R -> f;
            }
        }
    }
    return (ret -> f);
}

void BinHeap::merge(BinHeap *bh)
{
    BinHeap *H = new BinHeap();
    H -> roots = mergeRoots(this, bh);
    
    if (H -> roots == NULL)
    {
        this -> roots = NULL;
        this -> min = NULL;
        return;
    }
    
    BinNode *prevX = NULL;
    BinNode *x = H -> roots;
    BinNode *nextX = x -> f;
    while (nextX != NULL)
    {
        if (x -> degree != nextX -> degree || (nextX -> f != NULL && nextX -> f -> degree == x -> degree))
        {
            prevX = x;
            x = nextX;
        }
        else if (x -> key <= nextX -> key)
        {
            x -> f = nextX -> f;
            linkTrees(nextX, x);
        }
        else
        {
            if (prevX == NULL) H -> roots = nextX;
            else prevX -> f = nextX;
            linkTrees(x, nextX);
            x = nextX;
        }
        nextX = x -> f;
    }
    
    this -> roots = H -> roots;
    this -> min = H -> roots;
    BinNode *cur = this -> roots;
    while (cur != NULL)
    {
        if (cur -> key < this -> min -> key) this -> min = cur;
        cur = cur -> f;
    }
}

BinNode* BinHeap::first()
{
    return this -> min;
}

BinNode* BinHeap::extractMin()
{
    BinNode *ret = this -> first();
    
    // delete ret from the list of roots
    BinNode *prevX = NULL;
    BinNode *x = this -> roots;
    while (x != ret)
    {
        prevX = x;
        x = x -> f;
    }
    if (prevX == NULL) this -> roots = x -> f;
    else prevX -> f = x -> f;
    
    // reverse the list of ret's children
    BinNode *revChd = NULL;
    BinNode *cur = ret -> c;
    while (cur != NULL)
    {
        BinNode *next = cur -> f;
        cur -> f = revChd;
        revChd = cur;
        cur = next;
    }
    
    // merge the two lists
    BinHeap *H = new BinHeap();
    H -> roots = revChd;
    this -> merge(H);
    
    return ret;
}

void BinHeap::decreaseKey(BinNode *x, int newKey)
{
    // Precondition: x -> key > newKey
    x -> key = newKey;
    BinNode *y = x;
    BinNode *z = y -> p;
    while (z != NULL && y -> key < z -> key)
    {
        // swap contents
        swap(y -> key, z -> key);
        
        y = z;
        z = y -> p;
    }
    
    if (y -> key < this -> min -> key) this -> min = y;
}

void BinHeap::Delete(BinNode *x)
{
    decreaseKey(x, -INT_MAX);
    extractMin();
}

long long Kruskal_using_Binomial(long long vertices,long long edges, unordered_map< long long, pair<long long,vector<vector<long long>>>> q)
{	

}

int main()
{
    BinHeap *bh = new BinHeap();
    
    BinNode *x = new BinNode(11);
    BinNode *y = new BinNode(5);
    
    bh -> insert(x);
    bh -> insert(y);
    bh -> insert(new BinNode(3));
    bh -> insert(new BinNode(8));
    bh -> insert(new BinNode(4));
    
    bh -> decreaseKey(x, 2);
    
    while (!bh -> isEmpty())
    {
        printf("%d ", bh -> extractMin() -> key);
    }
    printf("\n");
    return 0;
}
