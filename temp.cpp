#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <set>
#include <utility>
#include <climits>
#include <math.h>
#include <map>
#include <unordered_set>

using namespace std;
typedef long long ll;
typedef vector<vector<long long> > vvll;
typedef vector<vector<bool> > vvb;
typedef vector<long long> vll;
typedef pair<ll, ll> pll;

#define F first
#define S second
#define PB push_back

const int M = 1e9 + 7;
ll mod_sub(ll a, ll b, ll m) {a = a % m; b = b % m; return (((a - b) % m) + m) % m;}
ll mod_add(ll a, ll b, ll m) {a = a % m; b = b % m; return (((a + b) % m) + m) % m;}

bool cmp(pair<ll, ll> a, pair<ll, ll> b){
	return a.second < b.second;
}

void fun(int &&x) {
	cout<<x<<endl;
	x=7;
}

void solve(){
	int a=5;
	fun(a+5);
	cout<<a<<endl;
}


int main(){
	ios::sync_with_stdio(false);
	cin.tie(0);
	int T=1; 
	// cin>>T;
	while(T--){
		solve();
	}
	return 0;
}