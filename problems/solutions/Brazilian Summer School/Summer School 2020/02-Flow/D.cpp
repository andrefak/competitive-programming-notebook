#include <bits/stdc++.h>
using namespace std;
#define For(i,a,b) for(int (i)=(a);(i) < (b); ++(i))
#define INF 0x3f3f3f3f
#define pb push_back
#define mp make_pair
#define fi first
#define se second
#define fastcin ios_base::sync_with_stdio(false);cin.tie(NULL);
#define ll long long
#define endl "\n"
#define debug false
#define MAX 

const int N = 1e3 + 5;
const int K = 15;

bool dom[N][N];
int f[N], vec[N][K];
int n, k;

int S = N-2, T = N-1;
    
struct edge {
    int a, b, cap, flow, idx;
    bool rev;
};

int d[N], ptr[N], q[N];
vector<edge> e;
vector<int> g[N];
vector<int> ind;
void add_edge (int a, int b, int cap) {
    int idx = 1;
    edge e1 = { a, b, cap, 0, idx, false };
    edge e2 = { b, a, 0, 0, idx, true };
    g[a].push_back ((int) e.size());
    e.push_back (e1);
    g[b].push_back ((int) e.size());
    e.push_back (e2);
}
    
bool bfs(int s, int t) {
    int qh=0, qt=0;
    q[qt++] = s;
    memset (d, -1, sizeof d);
    d[s] = 0;
    while (qh < qt && d[t] == -1) {
        int v = q[qh++];
        for (size_t i=0; i<g[v].size(); ++i) {
            int id = g[v][i],
                to = e[id].b;
            if (d[to] == -1 && e[id].flow < e[id].cap) {
                q[qt++] = to;
                d[to] = d[v] + 1;
            }
        }
    }
    return d[t] != -1;
}
    
int dfs (int v, int flow, int t) {
    if (!flow)  return 0;
    if (v == t)  return flow;
    for (; ptr[v]<(int)g[v].size(); ++ptr[v]) {
        int id = g[v][ptr[v]],
            to = e[id].b;
        if (d[to] != d[v] + 1)  continue;
        int pushed = dfs (to, min (flow, e[id].cap - e[id].flow), t);
        if (pushed) {
            e[id].flow += pushed;
            e[id^1].flow -= pushed;
            return pushed;
        }
    }
    return 0;
}

int vis[N];

void min_cut(int u) {
    vis[u] = 1;
    for(int i = 0; i < g[u].size(); ++i) {
        int id = g[u][i];
        int to = e[id].b, f = e[id].flow, cap = e[id].cap;
        if(vis[to] == 0 and f < cap) min_cut(to);
    }
}
    
int dinic(int s, int t) {
    int flow = 0;
    for (;;) {
        if (!bfs(s, t))  break;
        memset (ptr, 0, sizeof ptr);
        while (int pushed = dfs (s, INF, t))
            flow += pushed;
    }
    return flow;
}

inline bool domina(int i, int j) {
	if (i == j) return false;
	bool dif = false;
	for (int t = 0; t < k; t++) {
		if (vec[i][t] != vec[j][t]) dif = true;
		if (vec[i][t] < vec[j][t]) return false;
	}
	return dif;
}

int main() {	
	fastcin;

	cin >> n >> k;

	for(int i = 0; i < n; ++i) {
		for(int j = 0; j < k; ++j) {
			cin >> vec[i][j];
		}
		cin >> f[i];
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			dom[i][j] = domina(i, j);
		}
	}

	S = n, T = n+1;
	for(int i = 0; i < n; ++i) {
		if(f[i] == 0) {
			add_edge(i, T, 1);
		} else {
			add_edge(S, i, 1);
		}
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (dom[i][j] && f[i] < f[j]) {
				add_edge(j, i, INF);
			}
		}
	}
 
	cout << dinic(S, T) << '\n';

	min_cut(S);

	vector<int> ans;
	for(int u = 0; u < n + 2; ++u) {
        for(int i = 0; i < g[u].size(); ++i) {
            int id = g[u][i];
            int to = e[id].b, idx = e[id].idx, cap = e[id].cap;
            if(vis[u] and !vis[to] and e[id].rev == false) {
				if (u == S) ans.pb(to+1);
				else if (to == T) ans.pb(u+1);
            }
        }
    }
	sort(ans.begin(), ans.end());
	for (int xx : ans) cout << xx << ' ';
	cout << '\n';
}