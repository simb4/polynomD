#include <string>

using namespace std;

const int K = 10;

class composition;

class composition_helper {
	
};

class composition {
public:
	static vector<vector<int> > compositions;
	int id, deg;
	vector<int> pw;
};

class partition : composition {
	int partId;
};


static class composition_helper {
public:
	static map<composition> compositions;
	void generate() {
		if (!compositions.empty()) return;
		for (int n = 1; n <= 100)
	}
	composition get(vector<int> comp) {
		if (compositions.empty()) 
			generate();
		return compositions[composition];
	}

};

class Term {
	double coef;
	int deg; // number of variables
	int varPwId; // partition representing term 
	polyTerm(int varNum) {
		coef = deg = 0;		
		this->varNum = varNum;
		this->varPw = new int[varNum];
		memset(this->varPw, 0, sizeof(this->varPw));
	}
};


class Polynom {
	Term *terms[];
};

// class SymmetricPolynom : Polynom {
// 	Term *symTerms[];
// 	SymmetricPolynom(Polynom p) {
		
// 	}
// };

class PolynomHelper {
	string expr;
	
	Polynom readPolynom(string &s) {
		expr = s;
		Polynom poly = parse();
		return poly;
	}
};

  

int main() {


	return 0;
}


vi toPart(vi alpha) {
	sort(alpha.begin(), alpha.end());
	while (alpha.back() == 0) 
		alpha.pop_back();
	return alpha;
}

class comp { // composition
public:
	vi pow;
	number coef;
	comp() {
		pow.resize(K);
		coef = 0;
	}
	comp(vi pow, number coef) : pow(pow), coef(coef) {}
	bool operator < (const comp &c) {
		return this->pow < c.pow;
	}
	bool operator == (const comp &c) {
		return this->pow == c.pow;
	}
	comp operator + (const comp &c) {
		if (pow != c.pow) 
			throw runtime_error("Error: Compositions have different powers.");
		return comp(pow, this->coef + c.coef);
	}
	void operator += (const comp &c) {
		if (pow != c.pow) 
			throw runtime_error("Error: Compositions have different powers.");
		coef += c.coef;
	}
	comp operator * (const comp &c) {
		comp res;
		res.coef = this->coef * c.coef;
		for (int i = 0; i < K; i++)
			res.pow[i] = this->pow[i] + c.pow[i];
		return res;
	}
	void operator *= (const comp &c) {
		coef *= c.coef;
		for (int i = 0; i < K; i++)
			pow[i] += c.pow[i];
	}
};

class part : comp { // partition
public:
	part(vi pow, number coef) {
		this->pow = toPart(pow);
		this->coef = coef;
	}
};
