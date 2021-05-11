#include <iostream>
#include <string>
#include <stack>
#include <cassert>
using namespace std;
int evaluate(string infix, string& postfix, bool& result);

int precedence(char symbol)
{
	if (symbol == '^')
		return 0;
	if (symbol == '&')
		return 1;
	if (symbol == '!')
		return 2;
	return 0;
};
int evaluate(string infix, string& postfix, bool& result)
{
	//check for valid infix string
	if (infix.size() == 0)
		return 1;


	int openbracket = 0;
	int closebracket = 0;
	int x = 0;
	int y = 0;
	int letter = 0;
	int symbol = 0;
	int ex = 0;

	for (int s = 0; s < (int)infix.size(); s++)
	{
		if (infix.at(s) != '(' && infix.at(s) != ')' && infix.at(s) != 'T' && infix.at(s) != 'F' &&
			infix.at(s) != ' ' && infix.at(s) != '^' && infix.at(s) != '!' && infix.at(s) != '&')
			return 1;

		if (infix.at(s) == '!') {
			ex++;
			if (s == infix.size() - 1)
				return 1;

			if (s < (int)infix.size() - 1) {
				if (infix.at(s + 1) == '&' || infix.at(s + 1) == '^')
					return 1;
				int i = s + 1;
				while (i < (int)infix.size()) {
					if (infix.at(i) == '&' || infix.at(i) == '^' || infix.at(i) == ')')
						return 1;
					if (infix.at(i) == ' ')
						i++;
					else break;
				}
			}
		}


		if (infix.at(s) == '(' || infix.at(s) == ')') {
			if (infix.at(s) == '(')
				openbracket++;
			else {
				if (s != 0) 					//checks for symbol right before ')'
					if (infix.at(s - 1) == '&' || infix.at(s - 1) == '^' ||
						infix.at(s - 1) == '!' || infix.at(s - 1) == '(') //closing nothin
						return 1;
				closebracket++;
			}
		}


		if (x == 1)
		{
			if (infix.at(s) == '^' || infix.at(s) == '&')
			{
				x = 0;
				y++;
				symbol++;
			}
		}
		if (infix.at(s) == 'F' || infix.at(s) == 'T')
		{
			y = 0;
			x++;
			letter++;
		}
		if (x > 1 || y > 1)
			return 1;

	}
	if (openbracket != closebracket)   //odd number brackets
		return 1;

	int open = 0;
	for (int s = 0; s < (int)infix.size(); s++)
	{
		if (infix.at(s) == '(')
			open++;
		if (infix.at(s) == ')')
			open--;
		if (open < 0)
			return 1;
	}

	//if ((ex > 0 || symbol>0) && letter == 0)
		//return 1;



	postfix = "";
	stack<char> operatorStack;
	for (int s = 0; s < (int)infix.size(); s++)
	{
		switch (infix.at(s))
		{
		case ' ':
			break;
		case 'T':
			postfix.push_back(infix.at(s));
			break;
		case 'F':
			postfix.push_back(infix.at(s));
			break;
		case '(':
			if (s < (int)infix.size() - 1)                          //check for symbol following open brace
				if ((infix.at(s + 1) == '&' || infix.at(s + 1) == '^'))
					return 1;

			operatorStack.push(infix.at(s));
			break;
		case ')':
			while (operatorStack.top() != '(')
			{
				postfix.push_back(operatorStack.top());
				operatorStack.pop();
			}
			operatorStack.pop();
			break;
		case '!':
			/*while (!operatorStack.empty() && operatorStack.top() != '(' && precedence(infix.at(s)) <= precedence(operatorStack.top()))
			{
				postfix.push_back(operatorStack.top());
				operatorStack.pop();
			}*/
			operatorStack.push(infix.at(s));
			break;
		case '^':
			while (!operatorStack.empty() && operatorStack.top() != '(' && precedence(infix.at(s)) <= precedence(operatorStack.top()))
			{
				postfix.push_back(operatorStack.top());
				operatorStack.pop();
			}
			operatorStack.push(infix.at(s));
			break;
		case '&':
			while (!operatorStack.empty() && operatorStack.top() != '(' && precedence(infix.at(s)) <= precedence(operatorStack.top()))
			{
				postfix.push_back(operatorStack.top());
				operatorStack.pop();
			}
			operatorStack.push(infix.at(s));
			break;
		}
	}
	while (!operatorStack.empty()) {
		postfix.push_back(operatorStack.top());
		operatorStack.pop();
	}


	//cout << postfix << endl;
	/*
	for (int s = 0; s < (int)postfix.size(); s++)
	{
		if (postfix.at(s) == '!') {
			if (s < (int)postfix.size() - 1)                          //check for symbol following "!"
				if ((postfix.at(s + 1) == '&' || postfix.at(s + 1) == '^'))
					return 1;
		}
	}*/


	//evaluation of postfix
	stack<char> operandStack;

	if (postfix.size() == 1) {
		if (postfix.at(0) == 'F') {
			operandStack.push('F');
			result = false;
			return 0;
		}
		else if (postfix.at(0) == 'T') {
			operandStack.push('T');
			result = true;
			return 0;
		}
		else return 1;
	}

	//for only operands
	int letters = 0;
	int symbols = 0;
	int exs = 0;
	for (int s = 0; s < (int)postfix.size(); s++) {             //can just be one letter
		if (postfix.at(s) == 'T' || postfix.at(s) == 'F')
			letters++;
		if (postfix.at(s) == '^' || postfix.at(s) == '&')
			symbols++;
		if (postfix.at(s) == '!')
			exs++;
	}
	if (letters == 0 && (symbols > 0 || exs > 0))
		return 1;
	if (letters > 1 && symbols == 0)
		return 1;
	if (letters != symbols + 1)
		return 1;



	for (int s = 0; s < (int)postfix.size(); s++) {
		if (postfix.at(s) == 'T' || postfix.at(s) == 'F')
			operandStack.push(postfix.at(s));
		else
		{
			char operand2 = operandStack.top();
			operandStack.pop();                         //postfix.at(s) is an operator

			if (operandStack.empty() && postfix.at(s) != '!')   //for string cases like "F^"
				return 1;

			if (postfix.at(s) == '!') {                         //for string cases like "T!"
				if (operand2 == 'T')
					operandStack.push('F');
				else
					operandStack.push('T');
			}
			else {
				char operand1 = operandStack.top();		//regular procedure
				operandStack.pop();

				if (operand2 == operand1) {
					if (postfix.at(s) == '^')
						operandStack.push('F');
					if (postfix.at(s) == '&')
						operandStack.push(operand2);
				}
				else {
					if (postfix.at(s) == '^')
						operandStack.push('T');
					if (postfix.at(s) == '&')
						operandStack.push('F');
				}
			}
		}
	}

	if (operandStack.top() == 'T')
		result = true;
	else
		result = false;

	return 0;


	//	 infix to postfix
	//   Evaluates a boolean expression
	//   If infix is a syntactically valid infix boolean expression,
	//   then set postfix to the postfix form of that expression, set
	//   result to the value of the expression, and return zero.  If
	//   infix is not a syntactically valid expression, return 1; in
	//   that case, postfix may or may not be changed, but result must
	//   be unchanged.

}


int main()
{
	string pf;
	bool answer;
	assert(evaluate("T^ F", pf, answer) == 0 && pf == "TF^" && answer);
	assert(evaluate("T^", pf, answer) == 1);
	assert(evaluate("F F", pf, answer) == 1);
	assert(evaluate("TF", pf, answer) == 1);
	assert(evaluate("()", pf, answer) == 1);
	assert(evaluate("()T", pf, answer) == 1);
	assert(evaluate("T(F^T)", pf, answer) == 1);
	assert(evaluate("T(&T)", pf, answer) == 1);
	assert(evaluate("(T&(F^F)", pf, answer) == 1);
	assert(evaluate("T|F", pf, answer) == 1);
	assert(evaluate("", pf, answer) == 1);
	assert(evaluate("F  ^  !F & (T&F) ", pf, answer) == 0
		&& pf == "FF!TF&&^" && !answer);
	assert(evaluate(" F  ", pf, answer) == 0 && pf == "F" && !answer);
	assert(evaluate("((T))", pf, answer) == 0 && pf == "T" && answer);
	assert(evaluate("T", pf, answer) == 0 && answer == true);
	assert(evaluate("(F)", pf, answer) == 0 && answer == false);
	assert(evaluate("T ^(F)", pf, answer) == 0 && answer == true);
	assert(evaluate("T ^ !F", pf, answer) == 0 && answer == false);
	assert(evaluate("!(T&!!  !!F)", pf, answer) == 0 && answer == true);
	assert(evaluate("!T&F ", pf, answer) == 0 && answer == false);
	assert(evaluate("T^F&F", pf, answer) == 0 && answer == true);
	assert(evaluate("T&! (F^T&T^F  )^!!!(F&T&F)", pf, answer) == 0 && answer == true);
	assert(evaluate("TS DF:L", pf, answer) == 1);
	assert(evaluate("( F^T&!  F)", pf, answer) == 0 && answer == true);
	assert(evaluate("(( F))&((! T))", pf, answer) == 0 && answer == false);
	assert(evaluate("T^!F&T^T^(!F&T^!!T&F)", pf, answer) == 0 && answer == false);
	assert(evaluate("!!(F^T&F)", pf, answer) == 0 && answer == false);
	assert(evaluate("F^!T&T^!F", pf, answer) == 0 && answer == true);
	assert(evaluate("T&(F^!T&  T^!F)&!!(F^T& !F)", pf, answer) == 0 && answer == true);
	assert(evaluate("T^ F", pf, answer) == 0 && pf == "TF^" && answer);
	assert(evaluate("T^", pf, answer) == 1);
	assert(evaluate("F F", pf, answer) == 1);
	assert(evaluate("TF", pf, answer) == 1);
	assert(evaluate("()", pf, answer) == 1);
	assert(evaluate("()T", pf, answer) == 1);
	assert(evaluate("T(F^T)", pf, answer) == 1);
	assert(evaluate("T(&T)", pf, answer) == 1);
	assert(evaluate("(T&(F^F)", pf, answer) == 1);
	assert(evaluate("T|F", pf, answer) == 1);
	assert(evaluate("T&()F", pf, answer) == 1);
	assert(evaluate("T(&)F", pf, answer) == 1);
	assert(evaluate("T&F^T()", pf, answer) == 1);
	assert(evaluate("T&(F&T))&((T&T)", pf, answer) == 1);
	assert(evaluate("((T))(F)", pf, answer) == 1);
	assert(evaluate("F  ^  !F & (T&F) ", pf, answer) == 0
		&& pf == "FF!TF&&^" && !answer);
	assert(evaluate(" F  ", pf, answer) == 0 && pf == "F" && !answer);
	assert(evaluate("((((T))))", pf, answer) == 0 && pf == "T" && answer);
	assert(evaluate("(((T^F)()))", pf, answer) == 1);
	assert(evaluate("T^ F", pf, answer) == 0 && pf == "TF^" && answer);
	assert(evaluate("T^", pf, answer) == 1);
	assert(evaluate("F F", pf, answer) == 1);
	assert(evaluate("TF", pf, answer) == 1);
	assert(evaluate("()", pf, answer) == 1);
	assert(evaluate("()T", pf, answer) == 1);
	assert(evaluate("T(F^T)", pf, answer) == 1);
	assert(evaluate("T(&T)", pf, answer) == 1);
	assert(evaluate("(T&(F^F)", pf, answer) == 1);
	assert(evaluate("T&(F^F))", pf, answer) == 1);
	assert(evaluate("T&(F^F)", pf, answer) == 0 && !answer && pf == "TFF^&");
	assert(evaluate("     T   &     (    F   ^   F   )   ", pf, answer) == 0 && !answer && pf == "TFF^&");
	assert(evaluate("T&(F^(T^(F&T)))", pf, answer) == 0 && answer && pf == "TFTFT&^^&");
	assert(evaluate("!T&(F^(T^(F&T)))", pf, answer) == 0 && !answer && pf == "T!FTFT&^^&");
	assert(evaluate("!T&!(T^F)", pf, answer) == 0 && !answer && pf == "T!TF^!&");
	assert(evaluate("((T))", pf, answer) == 0 && answer && pf == "T");
	assert(evaluate("((F))", pf, answer) == 0 && !answer && pf == "F");
	assert(evaluate("(!)", pf, answer) == 1);
	assert(evaluate("(^)", pf, answer) == 1);
	assert(evaluate("(&)", pf, answer) == 1);
	assert(evaluate("(T!)", pf, answer) == 1);
	assert(evaluate("(T^)", pf, answer) == 1);
	assert(evaluate("(T&)", pf, answer) == 1);
	assert(evaluate("(&T)", pf, answer) == 1);
	assert(evaluate("(^T)", pf, answer) == 1);
	assert(evaluate("((T)", pf, answer) == 1);
	assert(evaluate("(T))", pf, answer) == 1);
	assert(evaluate("T|F", pf, answer) == 1);
	assert(evaluate("", pf, answer) == 1);
	assert(evaluate("    ", pf, answer) == 1);
	assert(evaluate("!", pf, answer) == 1);
	assert(evaluate("^", pf, answer) == 1);
	assert(evaluate("&", pf, answer) == 1);
	assert(evaluate("TF", pf, answer) == 1);
	assert(evaluate("FT", pf, answer) == 1);
	assert(evaluate("F  ^  !F & (T&F) ", pf, answer) == 0 && pf == "FF!TF&&^" && !answer);
	assert(evaluate(" F  ", pf, answer) == 0 && pf == "F" && !answer);
	assert(evaluate("((T))", pf, answer) == 0 && pf == "T" && answer);
	assert(evaluate("", pf, answer) == 1);
	assert(evaluate("(sdf)", pf, answer) == 1);
	assert(evaluate("()sdf", pf, answer) == 1);
	assert(evaluate("!", pf, answer) == 1);
	assert(evaluate("(", pf, answer) == 1);
	assert(evaluate("!!!", pf, answer) == 1);
	assert(evaluate("TF", pf, answer) == 1);
	assert(evaluate("T(F", pf, answer) == 1);
	assert(evaluate("T(F)", pf, answer) == 1);
	assert(evaluate("()()", pf, answer) == 1);
	assert(evaluate("&!", pf, answer) == 1);
	assert(evaluate("^!", pf, answer) == 1);
	assert(evaluate("&()", pf, answer) == 1);
	assert(evaluate("!()", pf, answer) == 1);
	assert(evaluate("F && F", pf, answer) == 1);
	assert(evaluate("T", pf, answer) == 0);
	assert(evaluate("!T", pf, answer) == 0);
	assert(evaluate("(!(T))", pf, answer) == 0);
	assert(evaluate("!(((T&F)))", pf, answer) == 0);
	assert(evaluate("!T", pf, answer) == 0);
	assert(evaluate("!( T & !F ^ T)", pf, answer) == 0);
	assert(evaluate("T & !!F", pf, answer) == 0);
	assert(evaluate("F", pf, answer) == 0);
	assert(evaluate("T", pf, answer) == 0 && pf == "T" && answer);

	assert(evaluate("(!(T))", pf, answer) == 0 && pf == "T!" && !answer);
	assert(evaluate("!(((T&F)))", pf, answer) == 0 && pf == "TF&!" && answer);
	assert(evaluate("!T", pf, answer) == 0 && pf == "T!" && !answer);
	assert(evaluate("F", pf, answer) == 0 && pf == "F" && !answer);
	assert(evaluate("!(T&!F) ^!T& !F", pf, answer) == 0 && pf == "TF!&!T!F!&^" && !answer);
	assert(evaluate("!!!!T", pf, answer) == 0 && pf == "T!!!!" && answer);
	assert(evaluate("!!T", pf, answer) == 0);
	assert(evaluate("!!!!T", pf, answer) == 0);
	assert(evaluate("!!(T) & !F ^ T", pf, answer) == 0);
	assert(evaluate("!!!!T", pf, answer) == 0 && pf == "T!!!!" && answer);
	assert(evaluate("T & !!F", pf, answer) == 0 && pf == "TF!!&" && !answer);
	assert(evaluate("!( T & !F ^ T)", pf, answer) == 0 && pf == "TF!&T^!" && answer);
	assert(evaluate("!!( T & !F ^ T)", pf, answer) == 0 && pf == "TF!&T^!!" && !answer);
	assert(evaluate("!!!( T & !F ^ T)", pf, answer) == 0 && pf == "TF!&T^!!!" && answer);
	assert(evaluate("!!(T) & !F ^ T", pf, answer) == 0 && pf == "T!!F!&T^" && !answer);
	assert(evaluate("(T&T)(T&T)", pf, answer) == 1);
	assert(evaluate("T!T", pf, answer) == 1);
	assert(evaluate("f&t", pf, answer) == 1);
	assert(evaluate("(((T&F)^((T)))", pf, answer) == 1);
	assert(evaluate("!(!F)^T", pf, answer) == 0);
	assert(evaluate("(!!)F^T", pf, answer) == 1);
	assert(evaluate("! ! F", pf, answer) == 0);
	assert(evaluate("T!", pf, answer) == 1);
	assert(evaluate("!T!", pf, answer) == 1);
	assert(evaluate("T!!", pf, answer) == 1);
	assert(evaluate("T!!&F", pf, answer) == 1);
	assert(evaluate("! !", pf, answer) == 1);
	assert(evaluate("!! ", pf, answer) == 1);
	assert(evaluate("!!", pf, answer) == 1);
	assert(evaluate("T!!   &F", pf, answer) == 1);
	assert(evaluate("!!!!!!T", pf, answer) == 0 && answer);
	assert(evaluate("T^ F", pf, answer) == 0 && pf == "TF^" && answer);
	assert(evaluate("T^", pf, answer) == 1);
	assert(evaluate("F F", pf, answer) == 1);
	assert(evaluate("TF", pf, answer) == 1);
	assert(evaluate("()", pf, answer) == 1);
	assert(evaluate("()T", pf, answer) == 1);
	assert(evaluate("(T", pf, answer) == 1);
	assert(evaluate("T)", pf, answer) == 1);
	assert(evaluate("(", pf, answer) == 1);
	assert(evaluate(" !    F  ", pf, answer) == 0 && pf == "F!" && answer);

	assert(evaluate(")", pf, answer) == 1);
	assert(evaluate("(())", pf, answer) == 1);
	assert(evaluate("T(F^T)", pf, answer) == 1);
	assert(evaluate("T(&T)", pf, answer) == 1);
	assert(evaluate("(T&(F^F)", pf, answer) == 1);
	assert(evaluate("T|F", pf, answer) == 1);
	assert(evaluate("", pf, answer) == 1);
	assert(evaluate("F  ^  !F & (T&F) ", pf, answer) == 0
		&& pf == "FF!TF&&^" && !answer);
	assert(evaluate(" !F  ", pf, answer) == 0 && pf == "F!" && answer);
	assert(evaluate(" F  ", pf, answer) == 0 && pf == "F" && !answer);
	assert(evaluate("((T))", pf, answer) == 0 && pf == "T" && answer);
	assert(evaluate("((!T))", pf, answer) == 0 && pf == "T!" && !answer);
	assert(evaluate("!(!F)^T", pf, answer) == 0);
	assert(evaluate("(!!)F^T", pf, answer) == 1);
	assert(evaluate("! ! F", pf, answer) == 0);
	assert(evaluate("T!", pf, answer) == 1);
	assert(evaluate("!T!", pf, answer) == 1);
	assert(evaluate("T!!", pf, answer) == 1);
	assert(evaluate("T!!&F", pf, answer) == 1);
	assert(evaluate("! !", pf, answer) == 1);
	assert(evaluate("!! ", pf, answer) == 1);
	assert(evaluate("!!", pf, answer) == 1);
	assert(evaluate("T!!   &F", pf, answer) == 1);
	assert(evaluate("!!!!!!T", pf, answer) == 0 && answer);
	cout << "Passed all tests" << endl;
}