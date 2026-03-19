#include <bits/stdc++.h>

using namespace std;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

struct Node{
    Node *left, *right;
    int prio , val , sz , lazy;
    long long sum;

    Node(){};

    Node(int x){
        left = right = nullptr;
        prio = rng();
        val = x;
        sz = 1;
        lazy = 0;
        sum = 1LL * x;
    }
};

int GetSize(Node *root){
    if (root == nullptr) return 0; else return root->sz;
}

long long GetSum(Node *root){
    if (root == nullptr) return 0; else return root->sum;
}

void Down(Node *root){
    if (root == nullptr) return;
    if (root->lazy == 0) return;
    swap(root->left , root->right);
    if (root->left != nullptr) root->left->lazy ^= 1;
    if (root->right != nullptr) root->right->lazy ^= 1;
    root->lazy = 0;
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
    Down(left);
    Down(right);
    if (left->prio < right->prio){
        Merge(left->right , left->right , right);
        root = left;
    }
    else{
        Merge(right->left , left , right->left);
        root = right;
    }
    root->sz = 1 + GetSize(root->left) + GetSize(root->right);
    root->sum = 1LL * root->val + GetSum(root->left) + GetSum(root->right);
}

void Split(Node *root , Node *&left , Node *&right , int val){
    if (root == nullptr){
        left = right = nullptr;
        return;
    }
    Down(root);
    if (GetSize(root->left) < val){
        Split(root->right , root->right , right , val - GetSize(root->left) - 1);
        left = root;
    }
    else{
        Split(root->left , left , root->left , val);
        right = root;
    }
    root->sz = 1 + GetSize(root->left) + GetSize(root->right);
    root->sum = 1LL * root->val + GetSum(root->left) + GetSum(root->right);
}

void Clear(Node *&root){
    if (root == nullptr) return;
    Clear(root->left);
    Clear(root->right);
    delete root;
    root = nullptr;
}

int main(){
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
//    freopen("main.inp" , "r" , stdin);
//    freopen("main.out" , "w" , stdout);
    int n , q;
    cin >> n >> q;
    Node *root = nullptr;
    for (int i = 1 ; i <= n ; i++){
        int x; cin >> x;
        Merge(root , root , new Node(x));
    }
    while (q--){
        int t , l , r;
        cin >> t >> l >> r;
        Node *A , *B , *C , *D;
        A = B = C = D = nullptr;
        Split(root , A , B , l - 1);
        Split(B , C , D , r - l + 1);
        if (t == 1){
            C->lazy ^= 1;
        }
        else{
            cout << C->sum << "\n";
        }
        Merge(root , A , C);
        Merge(root , root , D);
    }
    Clear(root);
    return 0;
}
