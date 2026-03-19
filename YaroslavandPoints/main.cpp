#include <bits/stdc++.h>
using namespace std;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

struct Node{
    Node *left , *right;

    int pos , cnt;
    long long prio , sum , sum_dif;

    Node(int _pos){
        prio = rng();
        pos = _pos;
        cnt = 1;
        sum = _pos;
        sum_dif = 0;
        left = right = nullptr;
    }
};

int GetCnt(Node *root){
    if (root == nullptr) return 0; else return root->cnt;
}

long long GetSum(Node *root){
    if (root == nullptr) return 0; else return root->sum;
}

long long GetSumDif(Node *root){
    if (root == nullptr) return 0; else return root->sum_dif;
}

void Modify(Node *root , Node *left , Node *right){
    root->cnt = 1 + GetCnt(left) + GetCnt(right);
    root->sum = 1LL * (root->pos) + GetSum(left) + GetSum(right);
    root->sum_dif = GetSumDif(left) + GetSumDif(right);
    root->sum_dif += 1LL * GetSum(right) * GetCnt(left) - 1LL * GetSum(left) * GetCnt(right);
    root->sum_dif += GetSum(right) - 1LL * GetCnt(right) * (root->pos);
    root->sum_dif += 1LL * GetCnt(left) * (root->pos) - GetSum(left);
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
    Modify(root , root->left , root->right);
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
    Modify(root , root->left , root->right);
}

void Delete(Node *&root){
    if (root == nullptr) return;
    Delete(root->left);
    Delete(root->right);
    delete root;
    root = nullptr;
}

int x[100007];

void Delete(Node *&root , int pos){
    Node *A , *B , *C , *D;
    A = B = C = D = nullptr;
    Split(root , A , B , pos);
    Split(B , C , D , pos + 1);
    if (C != nullptr) delete C;
    Merge(root , A , D);
}

void Insert(Node *&root , int pos){
    Node *A , *B , *C , *D;
    A = B = C = D = nullptr;
    Split(root , A , B , pos);
    Split(B , C , D , pos + 1);
    if (C != nullptr) delete C;
    C = new Node(pos);
    Merge(root , A , C);
    Merge(root , root , D);
}

long long GetRange(Node *&root , int l , int r){
    Node *A , *B , *C , *D;
    A = B = C = D = nullptr;
    Split(root , A , B , l);
    Split(B , C , D , r + 1);
    long long ans = (C == nullptr) ? 0 : C->sum_dif;
    Merge(root , A , C);
    Merge(root , root , D);
    return ans;
}

int main(){
    if (fopen("main.inp" , "r")){
        freopen("main.inp" , "r" , stdin);
        freopen("main.out" , "w" , stdout);
    }
    ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    int n; cin >> n;
    Node *root = nullptr;
    for (int i = 1 ; i <= n ; i++){
        cin >> x[i];
        Insert(root , x[i]);
    }
    int q; cin >> q;
    while (q--){
        int t; cin >> t;
        if (t == 1){
            int p , d;
            cin >> p >> d;
            Delete(root , x[p]);
            x[p] += d;
            Insert(root , x[p]);
        }
        else{
            int l , r;
            cin >> l >> r;
            cout << GetRange(root , l , r) << "\n";
        }
    }
    Delete(root);
    return 0;
}
