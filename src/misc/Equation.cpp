#include "Equation.h"

namespace ADBLib
{
	/**
	 * @brief Constructor. Lets you parse a new equation upon construction.
	 * @param newEqu An equation as a string.
	 */
	Equation::Equation(string newEqu)
	{
		parse(newEqu);
	}

	/**
	 * @brief Parses an equation into more easily processable form (RPN). Write equations like you would in C++, but with ^ for exponents and forward slashes ONLY for division.
	 * @param newEqu An equation as a string.
	 * @throw parse_error
	 * @note You MUST load an equation with this function before using evaluate()!
	 */
	void Equation::parse(string newEqu)
	{
		//Erase queue
		while (!outputQueue.empty())
			outputQueue.pop();

		//Remove spaces, for some reason c++ regex files won't work with eclipse without special config
		for (auto iter = newEqu.begin(); iter != newEqu.end(); iter++)
		{
			if (*iter == ' ')
				iter = newEqu.erase(iter);
		}

		stack<Token> operStack;

		//Shunting Yard Algorithm, by Edsger Dijkstra. Original implementation.
		for (auto iter = newEqu.begin(); iter != newEqu.end(); iter++)
		{
			Token newToken;
			if (getTType(iter) == VALUE) //Operand
			{
				newToken.value = getValue(iter, newEqu);
				newToken.type = VALUE;
				outputQueue.push(newToken);
			}

			else if (getTType(iter) == VARIABLE) //Variable
			{
				newToken.type = VARIABLE;
				outputQueue.push(newToken);
			}

			else if (getTType(iter) == OPERATOR && *iter != '(' && *iter != ')') //Operators
			{
				newToken.type = OPERATOR;
				newToken.operation = getOType(iter);
				//Pop off tokens of lesser priority than the new token and add them to the output queue.
				while (!operStack.empty())
				{
					if (operStack.top().operation <= newToken.operation)
					{
						outputQueue.push(operStack.top());
						operStack.pop();
					}
					else
						break;
				}
				operStack.push(newToken);
			}

			else if (*iter == '(') //Left parentheses
			{
				newToken.type = OPERATOR;
				newToken.operation = LPAREN;
				operStack.push(newToken);
			}

			else if (*iter == ')') //Right parnetheses
			{
				while (operStack.top().operation != LPAREN)
				{
					outputQueue.push(operStack.top());
					operStack.pop();
				}
				if (operStack.top().operation == LPAREN)
					operStack.pop();
				else
					throw parse_error("Couldn't find a left paren to match a right paren!");
			}
		}

		//Once finished...
		while (!operStack.empty())
		{
			if (operStack.top().operation == LPAREN)
				throw parse_error("Found a left paren that really shouldn't be here!");
			outputQueue.push(operStack.top());
			operStack.pop();
		}
	}

	/**
	 * @brief Evaluate the parsed equation, replacing "x" with the given value.
	 * @param value The value to substitue "x" for.
	 * @return The value of the evaluated equation
	 * @throw parse_error
	 */
	long double Equation::evaluate(long double value)
	{
		if (outputQueue.size() == 0)
			throw parse_error("No parsed formula found; did you forget to call Equation::parse()?");
		if (outputQueue.size() == 1 && outputQueue.front().type == VARIABLE)
			return value; // 'x' hack

		stack<Token> stk;
		queue<Token> tempque;
		while (!outputQueue.empty())
		{
			Token newToken = outputQueue.front();
			tempque.push(newToken);
			outputQueue.pop();
			if (newToken.type == VALUE || newToken.type == VARIABLE)
				stk.push(newToken);
			else //It's an operator
			{
				//Make sure enough values are on the stack
				while (stk.size() < 2)
				{
					stk.push(outputQueue.front());
					tempque.push(outputQueue.front());
					outputQueue.pop();
				}
				Token token1, token2;
				token1 = stk.top(); stk.pop();
				token2 = stk.top(); stk.pop();
				stk.push(newToken.evaluate(token1, token2, value));
			}
		}
		outputQueue = tempque;
		if (stk.size() > 1)
			throw parse_error("There are too many values on the stack!");
		return stk.top().value;
	}

	/**
	 * @brief Determines what type of token a given character is.
	 * @param iter An iterator to a character in an equation.
	 * @return The type of token this is (variable, value, operator).
	 * @throw parse_error
	 */
	tokenType Equation::getTType(string::iterator iter) const
	{
		if (*iter == 'x')
			return VARIABLE;
		else if (*iter == '^' || *iter == '*' || *iter == '/' || *iter == '+' || *iter == '-' || *iter == '(' || *iter == ')')
			return OPERATOR;
		else if (*iter == '0' ||
				*iter == '1' ||
				*iter == '2' ||
				*iter == '3' ||
				*iter == '4' ||
				*iter == '5' ||
				*iter == '6' ||
				*iter == '7' ||
				*iter == '8' ||
				*iter == '9')
			return VALUE;
		else
		{
			string error = "Unidentified operator ('";
			error = error + *iter + "') found.";
			throw parse_error(error.c_str());
		}
	}

	/**
	 * @brief Converts a character as an operator to one of the enumerated operations
	 * @param iter An iterator to a character in an equation.
	 * @return The type of operator this is.
	 * @throw parse_error
	 */
	operations Equation::getOType(string::iterator iter) const
	{
		operations returnval;
		switch (*iter)
		{
			case '(':
				returnval = LPAREN;
				break;
			case ')':
				returnval = RPAREN;
				break;
			case '^':
				returnval = EXPON;
				break;
			case '*':
				returnval = MULTI;
				break;
			case '/':
				returnval = DIVIS;
				break;
			case '+':
				returnval = ADDIT;
				break;
			case '-':
				returnval = SUBTRA;
				break;
			default:
				string error = "Unidentified operator ('";
				error = error + *iter + "') found.";
				throw parse_error(error.c_str());
				break;
		}
		return returnval;
	}

	/**
	 * @brief Given an iterator to the start of a number, converts the string number to a long double number, and advances the iterator to the end of the number.
	 * @param iter An iterator to the start of a number in the sequence.
	 * @param equation An equation as a string.
	 * @return The value of the number.
	 */
	long double Equation::getValue(string::iterator& iter, string& equation) const
	{
		string temp;
		for (; iter != equation.end(); iter++)
		{
			if (*iter != '0' &&
					*iter != '1' &&
					*iter != '2' &&
					*iter != '3' &&
					*iter != '4' &&
					*iter != '5' &&
					*iter != '6' &&
					*iter != '7' &&
					*iter != '8' &&
					*iter != '9' &&
					*iter != '.')
				break;					//Halt when the number ends
			temp.push_back(*iter);
		}
		iter--; //The parse function would skip the operator after the number without this
		return atof(temp.c_str());
	}

	/**
	 * @brief Internal use. Assuming this token is an operator, evaluates the results of the operation with the two given arguments.
	 * @param arg1 A token of type value or variable.
	 * @param arg2 A token of type value or variable.
	 * @param variable The value of the variable to use in case either argument is a variable.
	 * @return A token of type value containing the results of the operation.
	 * @throw parse_error
	 */
	Token Token::evaluate(Token arg1, Token arg2, long double variable)
	{
		if (arg1.type == VARIABLE)
			arg1.value = variable;
		if (arg2.type == VARIABLE)
			arg2.value = variable;

		long double value = 0;
		switch (operation)
		{
			case EXPON:
				value = pow(arg2.value, arg1.value);
				break;
			case MULTI:
				value = arg1.value * arg2.value;
				break;
			case DIVIS:
				value = arg2.value / arg1.value;
				break;
			case ADDIT:
				value = arg1.value + arg2.value;
				break;
			case SUBTRA:
				value = arg2.value - arg1.value;
				break;
			default:
				throw parse_error("Unidentified operator of some kind found in evaluation algorithm! You REALLY screwed up!");
				break;
		}
		Token newToken;
		newToken.type = VALUE;
		newToken.value = value;
		return newToken;
	}
}
