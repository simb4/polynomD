#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include "polynom.h"

const int MAXN = 20;
typedef vector<int> vect;
vect part;
vector<vect> parts[MAXN+1];
void generate(int n, int left, int mx) {
	if (left == 0) {
		parts[n].push_back(part);
		return;
	}
	for (int i = min(mx, left); i >= 1; i--) {
		part.push_back(i);
		generate(n, left - i, i);
		part.pop_back();
	}
}
void generate_partitions() {
	parts[0].push_back({0});
	for (int i = 1; i <= MAXN; i++) {
		generate(i, i, i);
	}
}

int get_id(vect p) {
	vector<vect> &_parts = parts[p.size()];
	return lower_bound(_parts.begin(), _parts.end(), p) - _parts.begin();
}
int get_p(int n, int mn=0) {
	if (n < 0 || n > MAXN) throw runtime_error("Error: this library doesn't support partitions of large numbers.");
	if (parts[n].size() == 0) generate_partitions();
	return parts[n].size();
}

class sym_monom {
public:
	int id, deg;
	number coef;
	sym_monom(int id, int deg, number coef) : id(id), deg(deg), coef(coef) {}

	bool const operator < (const sym_monom& m) const {
		return id < m.id;
	}
	bool operator == (const sym_monom& m) {
		return id == m.id;
	}
	number get_value(vect X) {
		assert(X.size() == K);

		vect comps = parts[deg][id];
		while (comps.size() < K) comps.push_back(0);

		number value = coef;
		do {
			value += monom(comps, ONE).get_value(X);
		} while (prev_permutation(comps.begin(), comps.end()));

		return value;
	}
	friend ostream &operator<<(ostream &output, const sym_monom& m) {
		vect &p = parts[m.deg][m.id];
		output << m.coef << "*m(";
		for (int i = 0; i < p.size(); i++)
			output << p[i] << (i+1==p.size()?')':',');
		return output;
	}
};

class sym_polynom {
public:
	vector<sym_monom> terms;
	sym_polynom() { }
	sym_polynom(sym_monom m) {
		terms.push_back(m);
	}
	sym_polynom(vector<sym_monom> sm) {
		terms = sm;
	}
	void add(sym_monom m) {
		terms.push_back(m);
	}

	number get_value(vect X) {
		assert(X.size() == K);

		number value = ZERO;
		for (sym_monom &sm : terms) 
			value += sm.get_value(X);
		
		return value;
	}
	string to_elementary() {
		string ret;
		for (int i = 0; i < terms.size(); i++) {

			for (int i = 0; i < terms.size(); i++) {

			}
			vect &p = parts[terms[i].deg][terms[i].id];
			// ret +=  << m.coef << "*e(";
			// for (int i = 0; i < p.size(); i++)
			// 	cout << p[i] << (i+1==p.size()?')':',');

		}
		return ret;
	}
	friend ostream &operator<<(ostream &output, const sym_polynom& p) {
		for (int i = 0; i < p.terms.size(); i++) {
			if (i != 0) output << " + ";
			output << p.terms[i];
		}
		return output;
	}
};


class sym_polynom_helper {
public:
	static const int debug=0;
	static bool is_homogenious(polynom &p) {
		int cur_deg = -1; 
		for (monom m : p.monoms) 
			if (cur_deg == -1) 
				cur_deg = m.deg; 
			else if (cur_deg != m.deg) 
				return false;
		return true; 
	}

	static bool is_symmetric(polynom& p) { 
		vector<bool> was(p.monoms.size()); 
		fill(was.begin(), was.end(), 0); 

		for (int i = 0; i < p.monoms.size(); i++) {
			if (was[i]) continue;

			vector<int> part = p.monoms[i].get_part();
			while (part.size() < K) part.push_back(0);

			do {
				int index = p.get_monom_index(part);

				if (index == -1)
					return false;
				
				if (p.monoms[i].coef != p.monoms[index].coef) 
					return false;

				was[index] = true;
			} while (prev_permutation(part.begin(), part.end()));
		}
		return true;
	}
	static sym_polynom to_symmetric(polynom& p) { 
		vector<bool> was(p.monoms.size());
		fill(was.begin(), was.end(), 0);
		sym_polynom ret_p;
		for (int i = 0; i < p.monoms.size(); i++) {
			if (was[i]) continue;

			vector<int> part = p.monoms[i].get_part();
			while (part.size() < K) part.push_back(0);

			do {
				int index = p.get_monom_index(part);

				if (index == -1 || p.monoms[i].coef != p.monoms[index].coef) 
					throw runtime_error("Polynom is not symmetric, use is_symmetric function first!");

				was[index] = true;
			} while (prev_permutation(part.begin(), part.end()));
			number coef = p.monoms[i].coef;
			int id = get_id(p.monoms[i].get_part());
			int deg = p.monoms[i].deg;
			ret_p.add(sym_monom(id, deg, coef));
		}
		return ret_p;
	}
};

