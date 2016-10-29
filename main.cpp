#include "iostream"
#include "fstream"
#include "sstream"
#include "expr.h"
using namespace std;

int main(int argc, char **argv) {
	char buffer[256];
	while (true) {
		cout << ">>>> ";
		cin.getline(buffer, sizeof(buffer));
		string strExpr(buffer);
		// empty
		if (strExpr.empty()) {
			continue;
		}
		// quit command
		else if (strExpr == "quit") {
			break;
		}
		// load file
		else if (strExpr.substr(0, 4) == "load") {
			const char * fileName = strExpr.c_str() + 5;
			ifstream fin(fileName);
			if (fin.fail()) {
				cout << "IO Error : file '" << fileName << "' does not exist" << endl;
			}
			stringstream ss;
			while (!fin.eof()) {
				fin.getline(buffer, sizeof(buffer));
				ss << buffer << endl;
			}
			Expr exprAll(ss.str());
			exprAll.eval();
			fin.close();
		}
		// eval
		else {
			try {
				Expr ex(strExpr);
				double result = ex.eval();
				cout << "<<<< " << result << endl;
			}
			catch (SyntaxError e) {
				for (int i = 0; i < e.pos + 5; ++i) cout << " ";

				cout << "^ Syntax error : (" << e.pos << ")" << endl;
			}
			catch (RuntimeError e) {
				cout << "Runtime error : " << e.message << endl;
			}
		}

	}
	return 0;
}