#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <cassert>

using namespace std;
const unsigned int K = 3; // variable number

// #define CUSTOM_NUMBER
#ifdef CUSTOM_NUMBER

class number {
public:
	unsigned int val;
	number() : val(0) {}
	number(unsigned int val) : val(val) {}

	bool operator <(const number& x) const {
		return val < x.val;
	}

	bool operator >(const number& x) {
		return val > x.val;
	}

	bool operator ==(const number& x) {
		return val == x.val;
	}

	bool operator !=(const number& x) {
		return val != x.val;
	}

	number operator + (const number& x) {
		return number(val ^ x.val);
	}

	number operator * (const number& x) {
		return number(val & x.val);
	}

	void operator += (const number& x) {
		val ^= x.val;
	}

	void operator *= (const number& x) {
		val *= x.val;
	}

	friend ostream &operator<<(ostream &output, const number& x) {
		output << x.val;
		return output;
	}
};
const number ONE = number((1ll<<32)-1);
const number ZERO = number(0);
#else
typedef int number;
const int ONE = 1;
const int ZERO = 0;
#endif


class monom {
public:
	number coef;
	int deg;
	int pow[K];
	monom() : coef(ZERO), deg(0) { memset(pow, 0, sizeof(pow)); }
	monom(number num) : coef(num), deg(0) { memset(pow, 0, sizeof(pow)); }

	monom(char var, int pw) : coef(ONE), deg(0) {
		memset(pow, 0, sizeof(pow));
		pow[var-'a'] += pw, deg += pw;
	}

	monom(int *pow, number coef) {
		this->coef = coef;
		this->deg = 0;
		for (int i = 0; i < K; i++)
			this->pow[i] = pow[i], deg += pow[i];
	}

	monom(vector<int> pow, number coef) {
		assert(pow.size() == K);
		this->coef = coef;
		this->deg = 0;
		for (int i = 0; i < K; i++)
			this->pow[i] = pow[i], deg += pow[i];
	}

	void add(char ch, int pw) {
		if ('a' <= ch && ch <= 'z') ch -= 'a';
		pow[ch] += pw;
		deg += pw;
	}

	vector<int> get_part() {
		vector<int> part;
		for (int i = 0; i < K; i++) {
			if (pow[i] != 0) {
				part.push_back(pow[i]);
			}
		}
		if (part.size() == 0) {
			part.push_back(0);
			return part;
		}
		sort(part.begin(), part.end());
		reverse(part.begin(), part.end());
		return part;
	}

	bool const operator < (const monom& m) const {
		for (int i = 0; i < K; i++) {
			if (pow[i] == m.pow[i]) 
				continue;

			return pow[i] > m.pow[i];
		}
		return false;
	}

	bool operator == (const monom& m) {
		if (deg != m.deg) return false;
		for (int i = 0; i < K; i++)
			if (pow[i] != m.pow[i])
				return false;
		return true;
	}
	bool operator != (const monom& m) {
		if (deg != m.deg) return true;
		for (int i = 0; i < K; i++)
			if (pow[i] != m.pow[i])
				return true;
		return false;
	}

	monom operator * (const monom& m) {
		monom ret;
		ret.coef = coef * m.coef;
		ret.deg = deg + m.deg;
		for (int i = 0; i < K; i++)
			ret.pow[i] = pow[i] + m.pow[i];
		return ret;
	}

	void operator *= (const monom& m) {
		coef *= m.coef;
		deg += m.deg;
		for (int i = 0; i < K; i++) 
			pow[i] += m.pow[i];
	}

	friend ostream &operator<<(ostream &output, const monom& m) {
		output << m.coef;
		for (int i = 0; i < K; i++) {
			if (m.pow[i] == 1) 
				output << '*' << (char)('a' + i);
			else if (m.pow[i])
				output << '*' << (char)('a' + i) << '^' << m.pow[i];
		}
		return output;
	}

	number get_value(vector<number> X) {
		assert(X.size() == K);
		number value = coef;
		for (int i = 0; i < X.size(); i++) {
			number x_pw = ONE;
			int pw = pow[i];
			number x = X[i];
			while (pw) {
				if (pw & 1) x_pw *= x;
				x = x * x;
				pw >>= 1;
			}
			value *= x_pw;
		}
		return value;
	}
};

class polynom {
public:
	vector<monom> monoms;
	polynom() { this->monoms.clear(); }
	polynom(vector<monom> monoms) { this->monoms = monoms; }
	polynom(monom m) { 
		this->monoms.clear(); 
		this->monoms.push_back(m);
	}
	
	int get_monom_index(vector<int> m) {
		monom M(m, ZERO);
		int ind = lower_bound(monoms.begin(), monoms.end(), M) - monoms.begin();

		if (ind == monoms.size() || monoms[ind] != M) 
			return -1;
		return ind;
	}
	number get_value(vector<number> X) {
		assert(X.size() == K);
		number value = ZERO;
		for (monom m : monoms) 
			value += m.get_value(X);
		return value;
	}

	void negate() {
		for (monom &m : monoms) 
			m.coef *= -1;
	}

	void remove_duplicates(vector<monom> &m) {
		sort(m.begin(), m.end());
		int i = 0, j = 0, k = 0;
		while (i < m.size()) {
			while (++j < m.size() && m[i] == m[j]) 
				m[i].coef += m[j].coef;
			m[k++] = m[i];
			i = j;
		}
		m.resize(k);
		k = 0;
		for (i = 0; i < m.size(); i++) 
			if (m[i].coef != ZERO) m[k++] = m[i];
		m.resize(k);
	}

	polynom operator +(const polynom& p) {
		vector<monom> m;
		m.insert(m.end(), monoms.begin(), monoms.end());
		m.insert(monoms.end(), p.monoms.begin(), p.monoms.end());
		remove_duplicates(monoms);
		return polynom(m);
	}

	void operator +=(const polynom& p) {
		monoms.insert(monoms.end(), p.monoms.begin(), p.monoms.end());
		remove_duplicates(monoms);
	}

	polynom operator *(const polynom& p) {
		vector<monom> m;
		for (monom m1 : monoms) for (monom m2 : p.monoms) 
			m.push_back(m1 * m2);
		remove_duplicates(m);
		return polynom(m);
	}

	void operator *=(const polynom& p) {
		vector<monom> monoms1 = monoms;
		monoms.clear();

		for (monom m1 : monoms1) for (monom m2 : p.monoms) 
			monoms.push_back(m1 * m2);
		
		remove_duplicates(monoms);
	}

	friend ostream &operator<<(ostream &output, const polynom &p) {
		if (p.monoms.size() == 0) {
			output << 0;
			return output;
		}
		for (int i = 0; i < p.monoms.size(); i++) {
			if (i != 0)
				output << " + ";
			output << p.monoms[i];
		}
		return output;
	}
};

class polynom_reader {
public:
	static const int debug=0;
	
	static int parse_number(string &s, int &it) {
		int num = 0;
		while (it < s.length() && '0' <= s[it] && s[it] <= '9') 
			num = num * 10 + (s[it++]-'0');
		return num;
	}	
	
	static polynom parse_factor(string &s, int &it) {
		polynom p;
		if (s[it] == '(') {
			it++; // (
			p = _parse_polynom(s, it);
			it++; // )
		}
		else if ('a' <= s[it] && s[it] <= 'z') {
			char var = s[it++];
			int pw = s[it] == '^' ? parse_number(s, ++it) : 1;
			p = monom(var, pw);
		}
		else
			p = monom(parse_number(s, it));
	
		if (debug) cerr << "parse_factor " << p << "\n";
		return p;
	}
	
	static polynom parse_var(string &s, int &it) {
		polynom t = parse_var(s, it);

		while (it < s.length() && s[it] == '*') 
			t = parse_var(s, ++it);

		if (debug) cerr << "parse_factor " << t << "\n";
		return t;
	}
	static polynom parse_term(string &s, int &it) {
		polynom pol = parse_factor(s, it);
		
		while (it < s.length() && s[it] == '*') 
			pol *= parse_factor(s, ++it);
		
		if (debug) cerr << "parse_term " << pol << "\n";
		return pol;
	}
	static polynom _parse_polynom(string &s, int &it) {
		polynom res = parse_term(s, it);
		
		while (it < s.length() && (s[it] == '+' || s[it] == '-')) {
			char sign = s[it++];
			
			polynom res1 = parse_term(s, it);

			if (sign == '-') 
				res1.negate();
			
			res += res1;
		}

		if (debug) cerr << "parse_polynom " << res << "\n";
		return res;
	}
	static polynom parse_polynom(string s) {
		int it = 0;
		for (int i = 0; i < s.length(); i++)
			if (s[i] != ' ') 
				s[it++] = s[i];

		s.resize(it);
		it = 0;
		return _parse_polynom(s, it);
	}
};
