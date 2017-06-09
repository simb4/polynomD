#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include "sym_polynom.h"
using namespace std;

int main() {

	printf("Commit B\n");



	freopen("z.in", "r", stdin);
	generate_partitions();
	polynom p = polynom_reader::parse_polynom("(1 + a * b) * (1 + a * c) * (1 + b * c)");
	polynom q = polynom_reader::parse_polynom("(2 + a) * (2 + b) * (2 + c)");
	cout << p << "\n";
	cout << p.get_value({2, 1, 0}) << "\n";
	cout << sym_polynom_helper::is_symmetric(p) << "\n";
	cout << sym_polynom_helper::is_symmetric(q) << "\n";
	sym_polynom p_sym = sym_polynom_helper::to_symmetric(p);
	cout << p_sym;

	return 0;
}
