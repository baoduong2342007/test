#include "game.h"
using namespace std;

int main(){
    freopen("main.inp" , "r" , stdin);
    freopen("main.out" , "w" , stdout);
    ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    int n , m , q;
    cin >> n >> m >> q;
    init(n , m);
    while (q--){
        int t;
        cin >> t;
        if (t == 1){
            int x , y; long long val;
            cin >> x >> y >> val;
            update(x , y , val);
        }
        else{
            int x , y , u , v;
            cin >> x >> y >> u >> v;
            cout << calculate(x , y , u , v) << "\n";
        }
    }
    return 0;
}
