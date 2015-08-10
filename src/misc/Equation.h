#include <string.h>
#include <math.h>
#include <stdexcept>
#include <cstdlib>
#include <queue>
#include <stack>
using std::string;
using std::stack;
using std::queue;
namespace ADBLib
{
	enum operations {EXPON, MULTI, DIVIS, ADDIT, SUBTRA, LPAREN, RPAREN};
	enum tokenType {VALUE, VARIABLE, OPERATOR};

	/**
	 * Equation class for handeling simple equations (PEMDAS) when the equation is not
	 * known at compile-time.
	 *
	 * @note This class throws the parse_error exception in case issues arise.
	 */
	class Equation
	{
	public:
		Equation() {}
		Equation(string newEqu);
		void parse(string newEqu);
		long double evaluate(long double value);
	protected:
		inline tokenType getTType(string::iterator iter) const;
		inline operations getOType(string::iterator iter) const;
		long double getValue(string::iterator& iter, string& equation) const;

		queue<class Token> outputQueue;
	};

	class Token
	{
	public:
		operations operation;
		tokenType type;
		long double value;
		Token evaluate(Token arg1, Token arg2, long double variable);
	};

	/** Exception indicating issue during parsing of the equation.
	 *
	 */
	class parse_error: public std::runtime_error
	{
	public:
		parse_error(string const& msg) : std::runtime_error(msg) {}
	};
}
