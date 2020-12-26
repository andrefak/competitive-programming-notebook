#include <bits/stdc++.h>
using namespace std;

vector<int> build_z(string& s) {
	int n = s.size();
	vector<int> z(n, 0);

	for (int i = 1, l = 0, r = 0; i < n; i++) {
		if (i <= r)
			z[i] = min(r - i + 1, z[i - l]);

		while(i + z[i] < n && s[z[i]+i] == s[z[i]])
			z[i]++;

		if (z[i] + i - 1 > r)
			l = i, r = i + z[i] - 1;
	}

	return z;
}

int main() {
	ios::sync_with_stdio(false); cin.tie(NULL);
	int t; cin >> t;
	while(t--) {
		string s; cin >> s;
		int n = s.size();

		vector<int> z = build_z(s);

		int ans = n;
		for (int i = 1; i < ans; i++) {
			if (n%i == 0 && z[i] + i == n) ans = i;
		}

		cout << ans;
		if (t) cout << "\n\n";
		else cout << "\n";
	}

}