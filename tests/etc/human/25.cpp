 #pragma GCC optimize("unroll-loops", "unswitch-loops")
#pragma GCC optimize("Ofast", "no-stack-protector")
#pragma GCC optimize("fast-math")

#include<bits/stdc++.h>


#define ll long long

using namespace std;


struct vstrech {
    ll mn, mx, pl, id;
};
vector<vstrech> a;
map<set<ll>, vector<ll>> was;

vector<ll> answer(vector<ll> v_rn, set<ll> s_rn, ll n_rn) {
    bool ans = false;
    vector<ll> out;
    if (was.count(s_rn) == 1) {
        return was[s_rn];
    }
    for (auto [minim, maxim, plus, num] : a) {
        if (s_rn.count(num) == 0) {
            if (n_rn >= minim && n_rn <= maxim) {
                ans = true;
                v_rn.push_back(num);
                s_rn.insert(num);
                n_rn += plus;
                vector<ll> rn = answer(v_rn, s_rn, n_rn);
                if (rn.size() > out.size()) {
                    out = rn;
                }
                v_rn.pop_back();
                s_rn.erase(num);
                n_rn -= plus;
            }
        }
    }
    if (!ans) {
        out = v_rn;
    }
    return was[s_rn] = out;
}


int32_t main() {
    ios_base::sync_with_stdio(false);
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    ll kk = 0, p, n;
    cin >> n >> p;
    a.resize(n);
    for (auto &el : a) {
        cin >> el.mn >> el.mx >> el.pl;
        kk++;
        el.id = kk;
    }
    vector<ll> bruh_uwu = answer({}, {}, p);
    cout << bruh_uwu.size() << "\n";
    for (auto el : bruh_uwu) {
        cout << el << ' ';
    }
}
