// Uses struct Point declared in Point.cpp

/** returns 2 * area(polygon) */
template<typename T> T shoelace2(vector<Point<T>> const& p) {
	T ans = 0; int n = p.size();
	for (int i = 0, j = n-1; i < n; j = i, i++) ans += p[i].cross(p[j]);
	return abs(ans);
}

/** return the minimum distance**2 between 2 points in the plane */
template<typename T> T closest_pair_distance2(vector<Point<T>> p) {
	sort(p.begin(), p.end());

	vector<Point<T>> strip(p.size());
	auto find_closest = [&](auto&& find_closest, vector<Point<T>>& p, int l, int r)->T {
		if (r - l <= 0) return numeric_limits<T>::max();

		int m = (l + r) / 2;
		T distL = find_closest(find_closest, p, l, m);
		T distR = find_closest(find_closest, p, m + 1, r);
		T dist = min(distL, distR);

		int strip_index = 0;
		for (int i = l, j = m + 1; i <= m || j <= r; ) {
			if (j > r || (i <= m && p[i].y < p[j].y)) strip[strip_index++] = p[i++];
			else strip[strip_index++] = p[j++];
		}
		for (int i = l; i <= r; i++) p[i] = strip[i - l];

		strip_index = 0;
		for (int i = l; i <= r; i++) if ((p[i].x - p[m].x) * (p[i].x - p[m].x) < dist)
			strip[strip_index++] = p[i];

		for (int i = 0; i < strip_index; i++) {
			for (int j = i + 1; j < strip_index && (strip[j].y - strip[i].y) * (strip[j].y - strip[i].y) < dist; j++) {
				dist = min(dist, strip[i].dist2(strip[j]));
			}
		}

		return dist;
	};

	return find_closest(find_closest, p, 0, (int) p.size() - 1);
}

/** Checks whether 1-dimensional segments [a, b] and [c, d] intersect */
template<typename T> bool segments_intersect_1d(T a, T b, T c, T d) {
	if (a > b) swap(a, b);
	if (c > d) swap(c, d);
	return max(a, c) <= min(b, d);
}

/** Checks whether segments AB and CD intersect */
template <typename T> bool segments_intersect(Point<T> const& a, Point<T> const& b, Point<T> const& c, Point<T> const& d) {
	if (c.cross(a, d) == 0 && c.cross(b, d) == 0)
		return segments_intersect_1d(a.x, b.x, c.x, d.x) && segments_intersect_1d(a.y, b.y, c.y, d.y);
	return sign(a.cross(b, c)) != sign(a.cross(b, d)) &&
		   sign(c.cross(d, a)) != sign(c.cross(d, b));
}

/** Check if point p is inside polygon. Return: 0: outside, 1: inside, 2: boundary.
 * BE CAREFUL: inf has to be greater than any other point, to make sure it isn't collinear with any edge */
template<typename T> int in_polygon(Point<T> p, vector<Point<T>> const& v) {
	int n = v.size(), count = 0;

	const T inf = 2e9;
	Point<T> p2{p.x + 1, inf};
	for (int i = 0, j = n-1; i < n; j = i, i++) {
		if (p.in_segment(v[i], v[j])) return 2;
		count += segments_intersect(p, p2, v[i], v[j]);
	}

	return count % 2;
}

/** Returns whether polygon points are in clockwise or counter-clockwise order */
template<typename T> bool is_clockwise(vector<Point<T>> const& v) {
	T area = 0; int n = v.size();
	for (int i = 0, j = n - 1; i < n; j = i, i++) area += v[i].cross(v[j]);
	return area >= 0;
}

/** Count integer (lattice) points inside and on boundary of polygon (can be concave)
 * BE CAREFUL: T must be INTEGER!!! */
template<typename T> pair<T, T> count_lattice_points(vector<Point<T>> const& v) {
	/** Pick's theorem: A = i + b/2 - 1 (area = inner_lattice + boundary_lattice/2 - 1) */
	pair<T, T> ans = {0, 0}; // <inside, boundary>
	T area2 = 0;
	int n = v.size();
	for (int i = 0, j = n-1; i < n; j = i, i++) {
		Point<T> dif = v[i] - v[j];
		dif.x = abs(dif.x), dif.y = abs(dif.y);
		ans.se += gcd(dif.x, dif.y);
		area2 += v[i].cross(v[j]);
	}
	area2 = abs(area2);
	ans.fi = (area2 - ans.se + 2) / 2;
	return ans;
}

/** Convex hull excluding collinear points. O(n)
 * To include collinear points, change the <= operators of orientation to < */
template<typename T> vector<Point<T>> convex_hull(vector<Point<T>> v) {
	int n = v.size();
	if (n <= 2) return v;

	sort(v.begin(), v.end());

	vector<Point<T>> ch(2 * n);
	int sz = 0;

	for (int i = 0; i < n; i++) {
		while(sz > 1 && v[i].orientation(ch[sz - 2], ch[sz - 1]) < 0) sz--;
		ch[sz++] = v[i];
	}
	for (int i = n-2, up_sz = sz; i >= 0; i--) {
		while(sz > up_sz && v[i].orientation(ch[sz - 2], ch[sz - 1]) < 0) sz--;
		ch[sz++] = v[i];
	}

	ch.resize(sz - 1);
	return ch;
}
