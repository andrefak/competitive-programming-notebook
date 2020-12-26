#include <bits/stdc++.h>
using namespace std;

#define pb push_back
#define eb emplace_back
#define mp make_pair
#define fi first
#define se second

typedef long long ll;
typedef pair<int, int> pii;

const int MAXN = 1e5 + 5;
const int BKSZ = 333;

vector<int> edges[MAXN];
int euler[MAXN], t_in[MAXN], t_out[MAXN], color[MAXN], curT;

class BIT {
	ll bt[MAXN+10];

	public:
	BIT() {
		memset(bt, 0, sizeof bt);
	}

	inline void update(int i, int val) {
		i++;

		while (i <= MAXN) {
			bt[i] += val;
			i += i&-i;
		}
	}

	inline ll query(int i) {
		i++;

		ll ret = 0;
		while (i > 0) {
			ret += bt[i];
			i -= i&-i;
		}
		return ret;
	}

	inline ll query(int i, int j) {
		if (i > j) return 0;		
		return query(j) - query(i-1);
	}
};

inline int64_t gilbertOrder(int x, int y, int pow, int rotate) {
	if (pow == 0) {
		return 0;
	}
	int hpow = 1 << (pow-1);
	int seg = (x < hpow) ? (
		(y < hpow) ? 0 : 3
	) : (
		(y < hpow) ? 1 : 2
	);
	seg = (seg + rotate) & 3;
	const int rotateDelta[4] = {3, 0, 0, 1};
	int nx = x & (x ^ hpow), ny = y & (y ^ hpow);
	int nrot = (rotate + rotateDelta[seg]) & 3;
	int64_t subSquareSize = int64_t(1) << (2*pow - 2);
	int64_t ans = seg * subSquareSize;
	int64_t add = gilbertOrder(nx, ny, pow-1, nrot);
	ans += (seg == 1 || seg == 2) ? add : (subSquareSize - add - 1);
	return ans;
}

class Query {
	public:
	int id, l, r, k;
	int64_t ord;
	Query() {}
	Query(int id, int l, int r, int k) {
		this->id = id;
		this->l = l;
		this->r = r;
		this->k = k;
		ord = gilbertOrder(l, r, 21, 0);
	}

	bool operator< (const Query& b) const {
		return ord < b.ord;
	}
};

class Mo {
	int n, q;
	ll curAns;
	vector<int> vec;
	vector<Query> qry;

	vector<int> f;
	BIT st;

	public:
	Mo() {}
	Mo(int* a, int n) {
		for (int i = 0; i < n; i++) vec.push_back(a[i]);
		this->q = 0;
		this->curAns = 0;
		f.resize(MAXN);
	}

	inline void addQuery(int l, int r, int k) {
		qry.push_back(Query(q++, l, r, k));
	}

	inline void add(int id) {
		st.update(f[vec[id]], -1);
		f[vec[id]]++;
		st.update(f[vec[id]], 1);
	}

	inline void remove(int id) {
		if (!f[vec[id]]) return;

		st.update(f[vec[id]], -1);
		f[vec[id]]--;
		st.update(f[vec[id]], 1);
	}

	inline ll getAnswer(int k) {
		return st.query(k, MAXN);
	}

	inline vector<ll> MoAlgorithm() {
		vector<ll> ans(q);
		sort(qry.begin(), qry.end());

		int l = 1, r = 0;
		for (int i = 0; i < (int)qry.size(); i++) {
			while (r < qry[i].r) add(++r);
			while (r > qry[i].r) remove(r--);
			while (l < qry[i].l) remove(l++);
			while (l > qry[i].l) add(--l);
			ans[qry[i].id] = getAnswer(qry[i].k);
		}

		return ans;
	}
};

void dfs(int u) {
	t_in[u] = ++curT;
	euler[curT] = color[u];

	for (int v : edges[u]) if (!t_in[v]) dfs(v);
	t_out[u] = curT;
}

int main() {
	int n, m; scanf("%d%d", &n, &m);
	for (int i = 1; i <= n; i++) scanf("%d", color+i);

	for (int i = 1; i < n; i++) {
		int u, v; scanf("%d%d", &u, &v);
		edges[u].push_back(v);
		edges[v].push_back(u);
	}

	dfs(1);

	Mo mo(euler+1, n);

	while(m--) {
		int v, k; scanf("%d%d", &v, &k);
		mo.addQuery(t_in[v]-1, t_out[v]-1, k);
	}

	vector<ll> ans = mo.MoAlgorithm();
	for (ll& x : ans) printf("%lld\n", x);
 
	return 0;
}