#include <iostream>
#include <fstream>
#include <string>
#include <vector>

std::string out;

std::vector<std::string> lexer;
std::vector<int> indents;
std::vector<std::string> tokens = {
	"input","output","int","float","bool","string","endl","if","else","elif","while",";",":","==","!=",">","<",">=","<=","}","{","=","$","+=","-="
	,"/=","*=","%="
};

std::vector<int> intReg;
std::vector<float> floatReg;
std::vector<bool> boolReg;
std::vector<std::string> stringReg;
std::vector<bool> ifVal;
std::vector<bool> whileVal;

struct ref {
	std::vector<std::string> ref;
} if_, while_, int_, float_, bool_, string_;

enum type {
	_input, _output, _int, _float, _bool, _string, _endl, _if, _else, _else_if, _while, _semi_colon, _colon, _equal_to, _not_equal_to, _greater_than,
	_less_than, _greater_than_equal_to, _less_than_equal_to, _right_curly, _left_curly, _equals, _dollar_sign, _plus_equals,
	_minus_equals, _divide_equals, _multiply_equals, _modulus_equals
};

bool isNum(std::string const& a) {
	int checkDigit = 0;
	bool allowedPeriod = true;
	for (int i = 0; i < a.length(); ++i) {
		if (isdigit(a[i])) {
			++checkDigit;
		}
		if (a[i] == char(46) && allowedPeriod) {
			++checkDigit;
			allowedPeriod = false;
		}
	}
	if (checkDigit == a.length()) {
		return true;
	}
	else {
		return false;
	}
}

bool to_bool(std::string const& a) {
	if (a == "true") {
		return true;
	}
	else {
		return false;
	}
}

int getToken(std::string const& a) {
	for (int i = 0; i < tokens.size(); ++i) {
		if (tokens.at(i) == a) {
			return i;
		}
	}
}

int getStartInstruction(int a) {
	bool searching = true;
	while (searching) {
		if (a == 0) {
			return getToken(lexer.at(a));
			searching = false;
		}
		if (lexer.at(a - 1) == ";" or lexer.at(a - 1) == ":") {
			return getToken(lexer.at(a));
			searching = false;
		}
		else {
			--a;
		}
	}
}

std::string getStartInstructionName(int a) {
	bool searching = true;
	while (searching) {
		if (a == 0) {
			return lexer.at(a);
			searching = false;
		}
		if (lexer.at(a - 1) == ";" or lexer.at(a - 1) == "{" or lexer.at(a - 1) == "}") {
			return lexer.at(a);
			searching = false;
		}
		else {
			--a;
		}
	}
}

bool isInIf(int a) {
	bool searching = true;
	while (searching) {
		if (a == 0) {
			return false;
			searching = false;
		}
		if (getToken(lexer.at(a)) == _right_curly) {
			return false;
			searching = false;
		}
		if (getToken(lexer.at(a)) == _if) {
			return true;
			searching = false;
		}
		--a;
	}
	/*
	int ifs = 0;
	int curlys = 2;
	while (searching) {
		if (a == 0) {
			return false;
			searching = false;
		}
		if (getToken(lexer.at(a)) == _right_curly) {
			++curlys;
		}
		if (getToken(lexer.at(a)) == _if) {
			++ifs;
		}
		if (curlys == ifs) {
			return true;
		}
		--a;
	}
	*/
}

bool isInWhile(int a) {
	bool searching = true;
	while (searching) {
		if (a == 0) {
			return false;
			searching = false;
		}
		if (getToken(lexer.at(a)) == _right_curly) {
			return false;
			searching = false;
		}
		if (getToken(lexer.at(a)) == _while) {
			return true;
			searching = false;
		}
		--a;
	}
}

int getWhileEnd(int a) {
	bool searching = true;
	while (searching) {
		if (a == lexer.size() - 1) {
			searching = false;
		}
		if (getToken(lexer.at(a)) == _right_curly) {
			return a;
			searching = false;
		}
		++a;
	}
}

int getNearConditional(int a) {
	int conds = 0;
	int curlys = 2;
	bool searching = true;
	int condsSearched = 0;
	while (searching) {
		if (a == 0) {
			return false;
			searching = false;
		}
		if (getToken(lexer.at(a)) == _right_curly) {
			++curlys;
		}
		if (getToken(lexer.at(a)) == _if or getToken(lexer.at(a)) == _else_if or getToken(lexer.at(a)) == _while) {
			++conds;
		}
		if (curlys == conds) {
			for (int i = a; i >= 0; --i) {
				if (getToken(lexer.at(a)) == _if or getToken(lexer.at(a)) == _else_if or getToken(lexer.at(a)) == _while) {
					++condsSearched;
				}
			}
			return condsSearched;
			searching = false;
		}
		--a;
	}
}

bool isInElse(int a) {
	bool searching = true;
	while (searching) {
		if (a == 0) {
			return false;
			searching = false;
		}
		if (getToken(lexer.at(a)) == _right_curly) {
			return false;
			searching = false;
		}
		if (getToken(lexer.at(a)) == _else) {
			return true;
			searching = false;
		}
		--a;
	}
}

bool isInElif(int a) {
	bool searching = true;
	while (searching) {
		if (a == 0) {
			return false;
			searching = false;
		}
		if (getToken(lexer.at(a)) == _right_curly) {
			return false;
			searching = false;
		}
		if (getToken(lexer.at(a)) == _else_if) {
			return true;
			searching = false;
		}
		--a;
	}
}

int getStartInstructionFromLineNum(int a) {
	bool searching = true;
	int linesSearched = 0;
	int i = 0;
	while (searching) {
		if (linesSearched == a) {
			return getStartInstruction(i + 1);
			searching = false;
		}
		if (i == lexer.size() - 1) {
			return getStartInstruction(i + 1);
			searching = false;
		}
		if (getToken(lexer.at(i)) == _semi_colon or getToken(lexer.at(i)) == _colon) {
			++linesSearched;
		}
		++i;
	}
}

int getConditionals(int a) {
	int ifsSearched = 0;
	bool searching = true;
	while (searching) {
		if (a == 0) {
			return ifsSearched;
			searching = false;
		}
		if (getToken(lexer.at(a)) == _if or getToken(lexer.at(a)) == _else_if or getToken(lexer.at(a)) == _while) {
			++ifsSearched;
		}
		--a;
	}
}

int getLineNum(int a) {
	int linesSearched = 0;
	bool searching = true;
	if (getToken(lexer.at(a)) == _semi_colon or getToken(lexer.at(a)) == _colon) {
		--a;
	}
	while (searching) {
		if (a <= 0) {
			return linesSearched;
			searching = false;
		}
		if (getToken(lexer.at(a)) == _semi_colon or getToken(lexer.at(a)) == _colon) {
			++linesSearched;
		}
		--a;
	}
}

bool isInConditional(int a) {
	int lineNum = getLineNum(a);
	int curLineNum = lineNum;
	bool searching = true;
	if (indents.at(lineNum) == 0) {
		return false;
		searching = false;
	}
	while (searching) {
		if ((indents.at(curLineNum) == indents.at(lineNum) - 1) && getStartInstructionFromLineNum(curLineNum) == _if) {
			return true;
			searching = false;
		}
		--curLineNum;
	}
}

int getParentConditional(int a) {
	int lineNum = getLineNum(a);
	int curLineNum = lineNum;
	int condsSearched = 0;
	bool searching = true;
	while (searching) {
		if ((indents.at(curLineNum) == indents.at(lineNum) - 1) && getStartInstructionFromLineNum(curLineNum) == _if) {
			searching = false;
		}
		--curLineNum;
	}
	for (int i = 0; getLineNum(i) <= curLineNum; ++i) {
		if (getToken(lexer.at(i)) == _if) {
			++condsSearched;
		}
	}
	return condsSearched;
}

void checkConditionals() {
	for (int i = 0; i < if_.ref.size(); i += 3) {
		double firstVal = 0;
		double secondVal = 0;
		if (if_.ref.at(i).at(0) == char(36)) {
			for (int c = 0; c < int_.ref.size(); ++c) {
				if (int_.ref.at(c) == if_.ref.at(i)) {
					firstVal = intReg.at(c);
				}
			}
			for (int c = 0; c < float_.ref.size(); ++c) {
				if (float_.ref.at(c) == if_.ref.at(i)) {
					firstVal = floatReg.at(c);
				}
			}
		}
		else {
			firstVal = std::stod(if_.ref.at(i));
		}
		if (if_.ref.at(i + 2).at(0) == char(36)) {
			for (int c = 0; c < int_.ref.size(); ++c) {
				if (int_.ref.at(c) == if_.ref.at(i + 2)) {
					secondVal = intReg.at(c);
				}
			}
			for (int c = 0; c < float_.ref.size(); ++c) {
				if (float_.ref.at(c) == if_.ref.at(i + 2)) {
					secondVal = floatReg.at(c);
				}
			}
		}
		else {
			secondVal = std::stod(if_.ref.at(i + 2));
		}
		switch (getToken(if_.ref.at(i + 1))) {
		case _equal_to:
			if (firstVal == secondVal) {
				ifVal.at(int(i / 3)) = true;
			}
			else {
				ifVal.at(int(i / 3)) = false;
			}
			break;
		case _not_equal_to:
			if (firstVal != secondVal) {
				ifVal.at(int(i / 3)) = true;
			}
			else {
				ifVal.at(int(i / 3)) = false;
			}
			break;
		case _greater_than:
			if (firstVal > secondVal) {
				ifVal.at(int(i / 3)) = true;
			}
			else {
				ifVal.at(int(i / 3)) = false;
			}
			break;
		case _less_than:
			if (firstVal < secondVal) {
				ifVal.at(int(i / 3)) = true;
			}
			else {
				ifVal.at(int(i / 3)) = false;
			}
			break;
		case _greater_than_equal_to:
			if (firstVal >= secondVal) {
				ifVal.at(int(i / 3)) = true;
			}
			else {
				ifVal.at(int(i / 3)) = false;
			}
			break;
		case _less_than_equal_to:
			if (firstVal <= secondVal) {
				ifVal.at(int(i / 3)) = true;
			}
			else {
				ifVal.at(int(i / 3)) = false;
			}
			break;
		}
	}
	for (int i = 0; i < lexer.size(); ++i) {
		if (getToken(lexer.at(i)) == _if && isInConditional(i) && !ifVal.at(getParentConditional(i))) {
			ifVal.at(getConditionals(i)) = false;
		}
	}
}

double getVarVal(int const& a) {
	for (int c = 0; c < int_.ref.size(); ++c) {
		if (int_.ref.at(c) == lexer.at(a)) {
			return intReg.at(c);
		}
	}
	for (int c = 0; c < float_.ref.size(); ++c) {
		if (float_.ref.at(c) == lexer.at(a)) {
			return floatReg.at(c);
		}
	}
}

int getNearIf(int a) {
	int ifsSearched = 0;
	bool ignoreOtherConditions = true;
	bool searching = true;
	while (searching) {
		if (a == 0) {
			return ifsSearched;
			searching = false;
		}
		if ((getToken(lexer.at(a)) == _else_if or getToken(lexer.at(a)) == _while) && !ignoreOtherConditions) {
			++ifsSearched;
		}
		if (getToken(lexer.at(a)) == _if) {
			++ifsSearched;
			ignoreOtherConditions = false;
		}
		--a;
	}
}

bool canExecute(int const& i) {
	/*if ((isInIf(i) && ifVal.at(getConditionals(i) - 1))
		or (!isInIf(i) && !isInElse(i) && !isInElif(i))
		or (isInElse(i) && !ifVal.at(getNearIf(i) - 1) && !ifVal.at(getConditionals(i) - 1))
		or (isInElif(i) && ifVal.at(getConditionals(i) - 1) && !ifVal.at(getConditionals(i) - 2))
		or (isInWhile(i) && ifVal.at(getConditionals(i) - 1))) {
		return true;
	}
	else {
		return false;
	}*/

	if ((isInConditional(i) && ifVal.at(getParentConditional(i)))
		or (!isInConditional(i))) {
		return true;
	}
	else {
		return false;
	}
}

void execute(int const& start, int const& end) {
	for (int i = start; i < end; ++i) {
		checkConditionals();
		switch (getStartInstruction(i)) {
		case _input:
			if (canExecute(i)) {
				if (lexer.at(i).at(0) == char(36)) {
					for (int c = 0; c < int_.ref.size(); ++c) {
						if (int_.ref.at(c) == lexer.at(i)) {
							std::cin >> intReg.at(c);
						}
					}
					for (int c = 0; c < float_.ref.size(); ++c) {
						if (float_.ref.at(c) == lexer.at(i)) {
							std::cin >> floatReg.at(c);
						}
					}
				}
			}
			break;
		case _output:
			if (canExecute(i)) {
				if (isNum(lexer.at(i))) {
					std::cout << lexer.at(i);
				}
				if (getToken(lexer.at(i)) == _endl) {
					std::cout << "\n";
				}
				if (lexer.at(i).at(0) == char(36) && getStartInstruction(i) == _output) {
					std::cout << getVarVal(i);
				}
			}
			break;
		}
		if (canExecute(i)) {
			if (getStartInstructionName(i).at(0) == char(36)) {
				if (getToken(lexer.at(i + 1)) == _equals) {
					if (isNum(lexer.at(i + 2))) {
						for (int c = 0; c < int_.ref.size(); ++c) {
							if (int_.ref.at(c) == lexer.at(i)) {
								intReg.at(c) = std::stoi(lexer.at(i + 2));
							}
						}
						for (int c = 0; c < float_.ref.size(); ++c) {
							if (float_.ref.at(c) == lexer.at(i)) {
								floatReg.at(c) = std::stof(lexer.at(i + 2));
							}
						}
					}
					if (lexer.at(i + 2).at(0) == char(36)) {
						int selectVar = 0;
						int changeVar = 0;
						for (int c = 0; c < int_.ref.size(); ++c) {
							if (int_.ref.at(c) == lexer.at(i)) {
								changeVar = c;
							}
						}
						for (int c = 0; c < float_.ref.size(); ++c) {
							if (float_.ref.at(c) == lexer.at(i)) {
								changeVar = c;
							}
						}
						for (int c = 0; c < int_.ref.size(); ++c) {
							if (int_.ref.at(c) == lexer.at(i + 2)) {
								selectVar = c;
								intReg.at(changeVar) = intReg.at(selectVar);
							}
						}
						for (int c = 0; c < float_.ref.size(); ++c) {
							if (float_.ref.at(c) == lexer.at(i + 2)) {
								selectVar = c;
								floatReg.at(changeVar) = floatReg.at(selectVar);
							}
						}
					}
				}
				if (getToken(lexer.at(i + 1)) == _plus_equals) {
					if (isNum(lexer.at(i + 2))) {
						for (int c = 0; c < int_.ref.size(); ++c) {
							if (int_.ref.at(c) == lexer.at(i)) {
								intReg.at(c) += std::stoi(lexer.at(i + 2));
							}
						}
						for (int c = 0; c < float_.ref.size(); ++c) {
							if (float_.ref.at(c) == lexer.at(i)) {
								floatReg.at(c) += std::stof(lexer.at(i + 2));
							}
						}
					}
					if (lexer.at(i + 2).at(0) == char(36)) {
						int selectVar = 0;
						int changeVar = 0;
						for (int c = 0; c < int_.ref.size(); ++c) {
							if (int_.ref.at(c) == lexer.at(i)) {
								changeVar = c;
							}
						}
						for (int c = 0; c < float_.ref.size(); ++c) {
							if (float_.ref.at(c) == lexer.at(i)) {
								changeVar = c;
							}
						}
						for (int c = 0; c < int_.ref.size(); ++c) {
							if (int_.ref.at(c) == lexer.at(i + 2)) {
								selectVar = c;
								intReg.at(changeVar) += intReg.at(selectVar);
							}
						}
						for (int c = 0; c < float_.ref.size(); ++c) {
							if (float_.ref.at(c) == lexer.at(i + 2)) {
								selectVar = c;
								floatReg.at(changeVar) += floatReg.at(selectVar);
							}
						}
					}
				}
				if (getToken(lexer.at(i + 1)) == _minus_equals) {
					if (isNum(lexer.at(i + 2))) {
						for (int c = 0; c < int_.ref.size(); ++c) {
							if (int_.ref.at(c) == lexer.at(i)) {
								intReg.at(c) -= std::stoi(lexer.at(i + 2));
							}
						}
						for (int c = 0; c < float_.ref.size(); ++c) {
							if (float_.ref.at(c) == lexer.at(i)) {
								floatReg.at(c) -= std::stof(lexer.at(i + 2));
							}
						}
					}
					if (lexer.at(i + 2).at(0) == char(36)) {
						int selectVar = 0;
						int changeVar = 0;
						for (int c = 0; c < int_.ref.size(); ++c) {
							if (int_.ref.at(c) == lexer.at(i)) {
								changeVar = c;
							}
						}
						for (int c = 0; c < float_.ref.size(); ++c) {
							if (float_.ref.at(c) == lexer.at(i)) {
								changeVar = c;
							}
						}
						for (int c = 0; c < int_.ref.size(); ++c) {
							if (int_.ref.at(c) == lexer.at(i + 2)) {
								selectVar = c;
								intReg.at(changeVar) -= intReg.at(selectVar);
							}
						}
						for (int c = 0; c < float_.ref.size(); ++c) {
							if (float_.ref.at(c) == lexer.at(i + 2)) {
								selectVar = c;
								floatReg.at(changeVar) -= floatReg.at(selectVar);
							}
						}
					}
				}
				if (getToken(lexer.at(i + 1)) == _multiply_equals) {
					if (isNum(lexer.at(i + 2))) {
						for (int c = 0; c < int_.ref.size(); ++c) {
							if (int_.ref.at(c) == lexer.at(i)) {
								intReg.at(c) *= std::stoi(lexer.at(i + 2));
							}
						}
						for (int c = 0; c < float_.ref.size(); ++c) {
							if (float_.ref.at(c) == lexer.at(i)) {
								floatReg.at(c) *= std::stof(lexer.at(i + 2));
							}
						}
					}
					if (lexer.at(i + 2).at(0) == char(36)) {
						int selectVar = 0;
						int changeVar = 0;
						for (int c = 0; c < int_.ref.size(); ++c) {
							if (int_.ref.at(c) == lexer.at(i)) {
								changeVar = c;
							}
						}
						for (int c = 0; c < float_.ref.size(); ++c) {
							if (float_.ref.at(c) == lexer.at(i)) {
								changeVar = c;
							}
						}
						for (int c = 0; c < int_.ref.size(); ++c) {
							if (int_.ref.at(c) == lexer.at(i + 2)) {
								selectVar = c;
								intReg.at(changeVar) *= intReg.at(selectVar);
							}
						}
						for (int c = 0; c < float_.ref.size(); ++c) {
							if (float_.ref.at(c) == lexer.at(i + 2)) {
								selectVar = c;
								floatReg.at(changeVar) *= floatReg.at(selectVar);
							}
						}
					}
				}
				if (getToken(lexer.at(i + 1)) == _modulus_equals) {
					if (isNum(lexer.at(i + 2))) {
						for (int c = 0; c < int_.ref.size(); ++c) {
							if (int_.ref.at(c) == lexer.at(i)) {
								intReg.at(c) %= std::stoi(lexer.at(i + 2));
							}
						}
					}
					if (lexer.at(i + 2).at(0) == char(36)) {
						int selectVar = 0;
						int changeVar = 0;
						for (int c = 0; c < int_.ref.size(); ++c) {
							if (int_.ref.at(c) == lexer.at(i)) {
								changeVar = c;
							}
						}
						for (int c = 0; c < float_.ref.size(); ++c) {
							if (float_.ref.at(c) == lexer.at(i)) {
								changeVar = c;
							}
						}
						for (int c = 0; c < int_.ref.size(); ++c) {
							if (int_.ref.at(c) == lexer.at(i + 2)) {
								selectVar = c;
								intReg.at(changeVar) %= intReg.at(selectVar);
							}
						}
					}
				}
			}
		}
		if (getToken(lexer.at(i)) == _while) {
			while (ifVal.at(getConditionals(i) - 1)) {
				execute(i + 3, getWhileEnd(i));
			}
		}
	}
}

int main() {
	int lineNum = 0;
	std::ifstream src("main.sl");
	lexer.push_back("if");
	lexer.push_back("1");
	lexer.push_back("==");
	lexer.push_back("1");
	lexer.push_back(";");
	while (getline(src, out)) {
		std::string inp;
		indents.push_back(0);
		for (int i = 0; i < out.length(); ++i) {
			if (out.at(0) != char(35) && out.at(i) != char(9)) {
				if (out[i] == char(32)) {
					lexer.push_back(inp);
					inp = "";
					++i;
				}
				inp += out.at(i);
				if (out[i + 1] == char(59)) {
					lexer.push_back(inp);
					lexer.push_back(";");
				}
				if (out[i + 1] == char(58)) {
					lexer.push_back(inp);
					lexer.push_back(":");
				}
				if (out[i] == char(123) or out[i] == char(125)) {
					lexer.push_back(inp);
				}
			}
			if (out.at(i) == char(9)) {
				indents.at(lineNum) += 1;
			}
		}
		++lineNum;
	}
	for (int i = 0; i < lexer.size(); ++i) {
		switch (getStartInstruction(i)) {
		case _int:
			if (lexer.at(i).at(0) == char(36)) {
				int_.ref.push_back(lexer.at(i));
				if (lexer.at(i + 1) == "=" && isNum(lexer.at(i + 2))) {
					intReg.push_back(std::stoi(lexer.at(i + 2)));
				}
				else {
					intReg.push_back(0);
				}
			}
			break;
		case _float:
			if (lexer.at(i).at(0) == char(36)) {
				float_.ref.push_back(lexer.at(i));
				if (lexer.at(i + 1) == "=" && isNum(lexer.at(i + 2))) {
					floatReg.push_back(std::stof(lexer.at(i + 2)));
				}
				else {
					floatReg.push_back(0);
				}
			}
			break;
		}
	}
	for (int i = 0; i < lexer.size(); ++i) {
		if (getToken(lexer.at(i)) == _if or getToken(lexer.at(i)) == _else_if or getToken(lexer.at(i)) == _while) {
			if_.ref.push_back(lexer.at(i + 1));
			if_.ref.push_back(lexer.at(i + 2));
			if_.ref.push_back(lexer.at(i + 3));
			ifVal.push_back(false);
		}
	}
	execute(0, lexer.size());
	src.close();
}