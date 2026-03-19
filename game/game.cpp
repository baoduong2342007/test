#include "game.h"
#include <bits/stdc++.h>
using namespace std;

int n , m;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

void init(int _n , int _m){
    n = _n;
    m = _m;
    srand(time(NULL));
}

struct Node{
    Node *left , *right;
    int pos;
    long long val , gcd_sum , prio;

    Node(){};

    Node(int _pos , long long _val){
        left = right = nullptr;
        pos = _pos;
        val = gcd_sum = _val;
        prio = rng();
    }
};

struct Treap{
    Node *root;

    Treap(){
        root = nullptr;
    }

    void Delete(Node *&root){
        if (root == nullptr) return;
        Delete(root->left);
        Delete(root->right);
        delete root;
        root = nullptr;
    }

    void ClearAll(){
        Delete(root);
    }

    ~Treap(){
        ClearAll();
    }

    long long GetGcd(Node *root){
        if (root == nullptr) return 0; else return root->gcd_sum;
    }

    void Merge(Node *&root , Node *left , Node *right){
        if (left == nullptr){
            root = right;
            return;
        }
        if (right == nullptr){
            root = left;
            return;
        }
        if (left->prio < right->prio){
            Merge(left->right , left->right , right);
            root = left;
        }
        else{
            Merge(right->left , left , right->left);
            root = right;
        }
        root->gcd_sum = __gcd(root->val , __gcd(GetGcd(root->left) , GetGcd(root->right)));
    }

    void Split(Node *root , Node *&left , Node *&right , int pos){
        if (root == nullptr){
            left = right = nullptr;
            return;
        }
        if (root->pos < pos){
            Split(root->right , root->right , right , pos);
            left = root;
        }
        else{
            Split(root->left , left , root->left , pos);
            right = root;
        }
        root->gcd_sum = __gcd(root->val , __gcd(GetGcd(root->left) , GetGcd(root->right)));
    }

    void Update(int pos , long long val){
        Node *A , *B , *C , *D;
        A = B = C = D = nullptr;
        Split(root , A , B , pos);
        Split(B , C , D , pos + 1);
        delete C;
        C = new Node(pos , val);
        Merge(root , A , C);
        Merge(root , root , D);
    }

    long long GetRange(int l , int r){
        Node *A , *B , *C , *D;
        A = B = C = D = nullptr;
        Split(root , A , B , l);
        Split(B , C , D , r + 1);
        long long ans = GetGcd(C);
        Merge(root , A , C);
        Merge(root , root , D);
        return ans;
    }
};

struct Node2{
    Treap T;
    Node2 *left , *right;

    Node2(){
        T = Treap();
        left = right = nullptr;
    };
};

struct SegmentTree{
    Node2 *root;

    SegmentTree(){
        root = new Node2();
    }

    void Delete(Node2 *&root){
        if (root == nullptr) return;
        Delete(root->left);
        Delete(root->right);
        (root->T).ClearAll();
    }

    void ClearAll(){
        Delete(root);
    }

    ~SegmentTree(){
        ClearAll();
    }

    void Update(Node2 *root , int l , int r , int x , int y , long long val){
        if (l == r){
            root->T.Update(y , val);
            return;
        }
        int mid = (l + r) / 2;
        if (x <= mid){
            if (root->left == nullptr) root->left = new Node2();
            Update(root->left , l , mid , x , y , val);
        }
        else{
            if (root->right == nullptr) root->right = new Node2();
            Update(root->right , mid + 1 , r , x , y , val);
        }
        long long left_gcd = (root->left == nullptr) ? 0 : (root->left->T).GetRange(y , y);
        long long right_gcd = (root->right == nullptr) ? 0 : (root->right->T).GetRange(y , y);
        long long combine_gcd = __gcd(left_gcd , right_gcd);
        root->T.Update(y , combine_gcd);
    }

    long long Get(Node2 *root , int l , int r , int x , int y , int u , int v){
        if (u < l || r < x) return 0;
        if (x <= l && r <= u) return (root->T).GetRange(y , v);
        int mid = (l + r) / 2;
        long long left_gcd = (root->left == nullptr) ? 0 : Get(root->left , l , mid , x , y , u , v);
        long long right_gcd = (root->right == nullptr) ? 0 : Get(root->right , mid + 1 , r , x , y , u , v);
        return __gcd(left_gcd , right_gcd);
    }
} S;

void update(int x , int y , long long val){
    S.Update(S.root , 0 , n - 1 , x , y , val);
}

long long calculate(int x , int y , int u , int v){
    return S.Get(S.root , 0 , n - 1 , x , y , u , v);
}
