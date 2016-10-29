#include "expr.h"
#include "iostream"
#include "sstream"
#include "cctype"
#include "cstdlib"
#include "cmath"
using namespace std;

#define checkoperand(s, n) if ((s).size() < (n)) throw(RuntimeError("Missing operand"))

const char * KEY_WORDS[] = {
	"let", "for", "print",nullptr
};

VariableSet variableSet;

void VariableSet::let(string varName, Value value) {
	varMap[varName] = value;
}

Value *VariableSet::get(string varName) {
	if (varMap.find(varName) != varMap.end()) {
		return &varMap[varName];
	}
	return nullptr;
}

Value::Value(int _type, std::string _str) : type(_type), str(_str) { }
Value::Value(int _type, double _num) : type(_type), num(_num) { }

void ValueStack::push(Value value) {
	s.push(value);
}

Value ValueStack::pop() {
	Value v = s.top();
	s.pop();
	return v;
}

double ValueStack::popNumber() {
	Value v = s.top();
	s.pop();
	if (v.type == TYPE_NUM) {
		return v.num;
	}
	else if (v.type == TYPE_VAR) {
		Value *value = variableSet.get(v.str);
		if (value == NULL) {
			stringstream ss; ss << "The variable '" << v.str << "' is undefined";
			throw(RuntimeError(ss.str()));
		}
		return value->num;
	}
	return 0;
}

string ValueStack::popVariableName() {
	Value v = s.top();
	s.pop();
	if (v.type != TYPE_VAR) {
		throw(RuntimeError("Right value required."));
	}
	return v.str;
}

string ValueStack::popBlock() {
	Value v = s.top();
	s.pop();
	if (v.type != TYPE_BLK) {
		throw(RuntimeError("Block value required."));
	}
	return v.str;
}

Analyzer::Analyzer(string _strExpr) : strExpr(_strExpr), pos(0) {

}

void Analyzer::rewind() {
	pos = 0;
}

Analyzer& Analyzer::operator >> (Token &token) {
	// skip white space
	while (isspace(strExpr[pos])) ++pos;
	// generic number
	if (isdigit(strExpr[pos])) {
		stringstream strBuffer;
		while (isdigit(strExpr[pos])) {
			strBuffer << strExpr[pos]; pos++;
		}
		if (strExpr[pos] == '.') {
			strBuffer << strExpr[pos]; pos++;
			while (isdigit(strExpr[pos])) {
				strBuffer << strExpr[pos]; pos++;
			}
		}
		token.type = TOKEN_NUM;
		token.token = strBuffer.str();
	}
	// variable
	else if (isalpha(strExpr[pos])) {
		stringstream strBuffer;
		while (isalnum(strExpr[pos])) {
			strBuffer << strExpr[pos]; pos++;
		}
		token.token = strBuffer.str();
		bool isKey = false;
		for (int i = 0; KEY_WORDS[i] != nullptr; ++i) {
			if (token.token == KEY_WORDS[i]) {
				isKey = true; break;
			}
		}
		token.type = isKey ? TOKEN_KEY : TOKEN_VAR;
	}
	// operator
	else if(strExpr[pos] == '+' || strExpr[pos] == '-' || strExpr[pos] == '+' ||
			strExpr[pos] == '*' || strExpr[pos] == '/' || strExpr[pos] == '^' ||
			strExpr[pos] == '<' || strExpr[pos] == '>' || strExpr[pos] == '=' ||
			strExpr[pos] == ';') {
		token.type = TOKEN_OPR;
		token.token = strExpr[pos]; pos++;
	}
	// block
	else if (strExpr[pos] == '{') {
		stringstream strBuffer;
		int balance = 1;
		pos++;
		while (balance > 0) {
			if (strExpr[pos] == '{') balance++;
			else if (strExpr[pos] == '}') balance--;
			else if (strExpr[pos] == '\0') throw SyntaxError(pos);
			if (balance != 0)
				strBuffer << strExpr[pos];
			pos++;
		}
		token.type = TOKEN_BLK;
		token.token = strBuffer.str();
	}
	// terminal
	else if (strExpr[pos] == '\0') {
		token.type = TOKEN_END;
		token.token = "";
	}
	// error
	else {
		throw SyntaxError(pos);
	}
	return *this;
}

Expr::Expr(string _strExpr) : analyzer(_strExpr) {}

void Expr::handleOperator(const string &opr, ValueStack &s) {
	double a, b;
	if (opr[0] == '+') {
		checkoperand(s, 2);
		b = s.popNumber(); a = s.popNumber();
		s.push(Value(TYPE_NUM, a + b));
	}
	else if (opr[0] == '-') {
		checkoperand(s, 2);
		b = s.popNumber(); a = s.popNumber();
		s.push(Value(TYPE_NUM, a - b));
	}
	else if (opr[0] == '*') {
		checkoperand(s, 2);
		b = s.popNumber(); a = s.popNumber();
		s.push(Value(TYPE_NUM, a * b));
	}
	else if (opr[0] == '/') {
		checkoperand(s, 2);
		b = s.popNumber(); a = s.popNumber();
		s.push(Value(TYPE_NUM, a / b));
	}
	else if (opr[0] == '^') {
		checkoperand(s, 2);
		b = s.popNumber(); a = s.popNumber();
		s.push(Value(TYPE_NUM, pow(a, b)));
	}
	else if (opr[0] == '>') {
		checkoperand(s, 2);
		b = s.popNumber(); a = s.popNumber();
		s.push(Value(TYPE_NUM, a > b));
	}
	else if (opr[0] == '<') {
		checkoperand(s, 2);
		b = s.popNumber(); a = s.popNumber();
		s.push(Value(TYPE_NUM, a < b));
	}
	else if (opr[0] == '=') {
		checkoperand(s, 2);
		b = s.popNumber(); a = s.popNumber();
		s.push(Value(TYPE_NUM, a == b));
	}
	else if (opr[0] == ';') {
		checkoperand(s, 1);
		s.pop();
	}
}

void Expr::handleKeyWord(const string &key, ValueStack &s) {
	if (key == "let") {
		checkoperand(s, 2);
		string a = s.popVariableName();
		double b = s.popNumber();
		variableSet.let(a, Value(TYPE_NUM, b));
		s.push(Value(TYPE_NUM, b));
	}
	else if (key == "for") {
		checkoperand(s, 4);
		Expr exe(s.popBlock());
		Expr adder(s.popBlock());
		Expr check(s.popBlock());
		Expr start(s.popBlock());

		for (start.eval(); check.eval() != 0; adder.eval()) {
			exe.eval();
		}

		s.push(Value(TYPE_NUM, 0));
	}
	else if (key == "print") {
		checkoperand(s, 1);
		cout << s.popNumber() << endl;
		s.push(Value(TYPE_NUM, 0));
	}
}

double Expr::eval() {
	Token token;
	ValueStack stack;
	// fetch token form expression
	while (analyzer >> token, token.type != TOKEN_END) {
		if (token.type == TOKEN_NUM) {
			stack.push(Value(TYPE_NUM, atof(token.token.c_str())));
		}
		else if (token.type == TOKEN_VAR) {
			stack.push(Value(TYPE_VAR, token.token));
		}
		else if (token.type == TOKEN_BLK) {
			stack.push(Value(TYPE_BLK, token.token));
		}
		else if(token.type == TOKEN_OPR) {
			handleOperator(token.token, stack);
		}
		else if (token.type == TOKEN_KEY) {
			handleKeyWord(token.token, stack);
		}
	}
	analyzer.rewind();
	if (stack.size() > 1) {
		throw RuntimeError("Missing operator");
	} 
	else if (stack.size() == 0) {
		return 0;
	}
	return stack.popNumber();
}

bool Expr::hasError() {
	return this->boolHasError;
}

string Expr::getErrorMessage() {
	return errorMessage;
}