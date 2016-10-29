#ifndef _EXPR_H_
#define _EXPR_H_

#include "string"
#include "stack"
#include "map"

class SyntaxError {
public:
	SyntaxError(int _pos) : pos(_pos) {}
	int pos;
};

class BreakException {
public:
	BreakException() {}
};


class RuntimeError {
public:
	RuntimeError(std::string _message) : message(_message) {}
	std::string message;
};

#define TOKEN_END 0
#define TOKEN_KEY 1
#define TOKEN_NUM 2
#define TOKEN_VAR 3
#define TOKEN_OPR 4
#define TOKEN_BLK 5

class Token {
public:
	int type;
	std::string token;
};

class Analyzer {
public:
	Analyzer(std::string _strExpr);
	Analyzer& operator >> (Token &token);
	void rewind();
private:
	std::string strExpr;
	int pos;
};

#define TYPE_NUM 1
#define TYPE_STR 2
#define TYPE_VAR 3
#define TYPE_BLK 4

class Value {
public:
	std::string str;
	double num;
	int type;
	Value() {}
	Value::Value(int _type, std::string str);
	Value::Value(int _type, double num);
};

class ValueStack {
public:
	void push(Value value);
	Value pop();
	double popNumber();
	std::string popVariableName();
	std::string popBlock();
	size_t size() { return s.size(); }
private:
	std::stack<Value> s;
};

class VariableSet {
public:
	void let(std::string varName, Value value);
	Value *get(std::string varName);
private:
	std::map<std::string, Value> varMap;
};

class Expr {
public:
	Expr(std::string _strExpr);
	double eval();
	bool hasError();
	std::string getErrorMessage();
private:
	void handleOperator(const std::string &opr, ValueStack &s);
	void handleKeyWord(const std::string &opr, ValueStack &s);
	Analyzer analyzer;
	std::string strError;
	bool boolHasError;
	std::string errorMessage;
};

#endif