#include <iostream>
#include <cstdio>
#include <algorithm>
#include <vector>

using namespace std;

typedef long long ll;
const int N = (int)2e5;
int n, m, a[N], b[N];

struct node {
	int sum, leaf;
	node *l, *r;
	// node() : sum(0), leaf(0), l(NULL), r(NULL) {}
	node(int sum) : sum(sum), leaf(1), l(NULL), r(NULL) {}
	node(node *l, node *r) : l(l), r(r) {
		this->sum = 0;
		this->sum += (l->leaf ? !!l->sum : l->sum);
		this->sum = l->sum + r->sum;
	}
} *root[N];

#define mid ((l+r)>>1)

node *build(int l, int r) {
	if (l == r) return new node(0);
	return new node(build(l, mid), build(mid+1, r));
}
node* update(node *v, int l, int r, int pos, int val) {
	if (l == r) return new node(v->sum + val);
	return pos <= mid ?
		new node(update(v->l, l, mid, pos, val), v->r):
		new node(v->l, update(v->r, mid+1, r, pos, val));
}
int get(node *a, node *b, int l, int r, int k) {
	if (l == r) return l;
	if (k <= a->l->sum - b->l->sum)
		return get(a->l, b->l, l, mid, k);
	return get(a->r, b->r, mid+1, r, k - (a->l->sum - b->l->sum));
}


int main() {
	freopen("z.in", "r", stdin);
	scanf("%d%d", &n, &m);
	
	for (int i = 1; i <= n; i++) {
		scanf("%d", a + i);
		b[i] = a[i];
		
	}
	sort(b + 1, b + n + 1);
	int M = unique(b + 1, b + n + 1) - b;

	root[0] = build(1, M);
	for (int i = 1; i <= n; i++) {
		a[i] = lower_bound(b + 1, b + M + 1, a[i]) - b;
		root[i] = update(root[i-1], 1, M, a[i], +1);
		root[i] = update(root[i], 1, M, a[i], -1);
	}


	int ans = 0;
	for (int i = 1; i <= m; i++) {
		int l, r, k, a, b, c, d;
		cerr << i <<'\n';
		scanf("%d %d %d %d %d", &a, &b, &c, &d, &k);
		l = (a * max(ans, 0) + b) % (n + 1);
		r = (c * max(ans, 0) + d) % (n + 1);
		if (l > r) swap(l, r);
		if (l == 0) l = 1;
		if (root[r]->sum - root[l-1]->sum < k)
			ans = -1;
		else
			ans = get(root[r], root[l-1], 1, n, k);
		printf("%d-th[%d %d] = %d\n", k,l,r,ans);
	}


	return 0;
}
