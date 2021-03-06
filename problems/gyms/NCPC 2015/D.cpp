#include <bits/stdc++.h>
using namespace std;

#ifdef LOCAL_PC
	#define debug(args...) cout.flush(); fflush(stdout); fprintf(stderr, args)
#else
	#define debug(args...) ; /*  */
#endif
#define pb push_back
#define eb emplace_back
#define mk make_pair
#define fi first
#define se second
typedef long long ll;
typedef pair<int, int> pii;
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

template<class num> inline void rd(num &x) {
	char c; while(isspace(c = getchar()));
	bool neg = false;
	if(!isdigit(c)) neg = (c == '-'), x = 0;
	else x = c - '0';
	while(isdigit(c = getchar()))
	x = (x << 3) + (x << 1) + c - '0';
	if (neg) x = -x;
}
template <class T, class... Args> inline void rd(T& x, Args&... args) { rd(x); rd(args...); }

const int MAXN = 2e5 + 5, INF = 0x3f3f3f3f;

int main() {
	int n, k; rd(n, k);

	int ans = 0;
	priority_queue<pii, vector<pii>, greater<pii>> pq;
	set<int> av;
	map<int, int> qtt;
	for (int i = 0; i < n; i++) {
		int t; rd(t);
		while(pq.size() && pq.top().fi <= t) {
			auto p = pq.top();
			pq.pop();
			qtt[p.se]--;
			if (qtt[p.se] < k) av.emplace(p.se);
		}

		if (av.empty()) {
			ans++;
			av.emplace(ans);
		}


		int u = *av.begin();
		qtt[u]++;
		if (qtt[u] == k) av.erase(u);
		pq.emplace(t + 1000, u);
	}

	printf("%d\n", ans);
	
}