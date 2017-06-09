#ifndef COMPOSITION_H
#define COMPOSITION_H
#include <vector>
using namespace std;
const int K = 10;
const int N = 10;

// class composition {
// public:
// 	int id, deg;
// 	composition(vector<int>);
// 	int getId();
// 	int getDeg();
// };


//12312312321
///123123123123213


class CompositionHelper {
public:
	// static vector<vector<int> > comps;
	// static vector<int> comp;
	// static void generate(int, int);
	// static void generateAll();
	// static int getComp(vector<int>);
	vector<vector<int> > comps;
	vector<int> comp;
	CompositionHelper();
	void generate(int, int);
	void generateAll();
	int getComp(vector<int>);


};
CompositionHelper::CompositionHelper() {
	comp.resize(K);
}
void CompositionHelper::generateAll() {
	if (!comps.empty()) return;
	comp.resize(K);
	for (int i = 1; i <= N; i++) {
		generate(i, 0);
	}
};

void CompositionHelper::generate(int n, int k) {
	if (k == K - 1) {
		comp[k] = n;
		comps.push_back(comp);
		return;
	}
	for (int i = 0; i <= n; i++)
	{	comp[k] = i;
		generate(n - i, k + 1);
	}
};

int CompositionHelper::getComp(vector<int> c) {
	generateAll();
	int id = lower_bound(comps.begin(), comps.end(), c) - comps.begin();
	if (comps[id] == c)
		return id;
	return -1;
}
#endif
