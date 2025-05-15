#include <iostream>
#include <vector>
#include <math.h>
#include <optional>
#include <QString>
#include <QList>


class Calc final {
private:
	// Custom structures declarations
	struct Stack {
		double contents;
		Stack* next;
	};

	struct Conv_Stack {
		QChar contents;
		Conv_Stack* next;
	};

	// Field data structures declarations
	Conv_Stack* conv_st = nullptr;
	Stack* st = nullptr;
	QList<QString> postfix;

	// Stack related functions
	void conv_push(QChar a) { // No dependencies | Calls to malloc
		Conv_Stack* new_st = (Conv_Stack*)malloc(sizeof(Conv_Stack));
		new_st -> contents = a;
		new_st -> next = conv_st;
		conv_st = new_st;
	}

	QChar conv_pop() { // No dependencies
		if (conv_st == nullptr)
			throw(1);
		QChar output = conv_st -> contents;
		Conv_Stack* temp_ptr = conv_st -> next;
		free(conv_st);
		conv_st = temp_ptr;
		return output;
	}

	void push(double a) { // No dependencies | Calls to malloc
		Stack* new_st = (Stack*)malloc(sizeof(Stack));
		new_st -> contents = a;
		new_st -> next = st;
		st = new_st;
	}

	double pop() { // No dependencies
		if (st == nullptr)
			throw(2);
		double output = st -> contents;
		Stack* temp_ptr = st -> next;
		free(st);
		st = temp_ptr;
		return output;
	}

	// Extra functions
	int prec(QChar a) { // No dependencies
		int precedence;
		switch (a.toLatin1()) {
			case '+': [[fallthrough]];
			case '-':
				precedence = 0;
				break;
			case '*': [[fallthrough]];
			case '/':
				precedence = 1;
				break;
			case '^':
				precedence = 2;
				break;
			case 's': [[fallthrough]];
			case 'c': [[fallthrough]];
			case 't': [[fallthrough]];
			case 'r': [[fallthrough]];
			case 'l':
				precedence = 3;
				break;
			default:
				throw(3);
		}

		return precedence;
	}

	QString parse(QString const& a) { // No dependencies
		QString parsed_input = "";
		// Check for unary minuses
		for (int i = 0; i < a.size() - 1; i++) {
			if (a[i] == '(' && a[i + 1] == '-') {
				parsed_input += "(0";
				continue;
			} else if (i == 0 && a[i] == '-')
				parsed_input += "0";
			parsed_input += a[i];
		}
		parsed_input += a[a.size() - 1];

		// Check for parenthesis correctness
		int paren = 0;
		for (QChar i : parsed_input) {
			if (i == '(')
				paren++;
			else if (i == ')')
				paren--;
			
			if (paren < 0)
				throw(4);
		}
		if (paren != 0)
			throw(5);

		// Replace multicharacter operators with single character ones
		parsed_input.replace("sin", "s"); // Sine
		parsed_input.replace("cos", "c"); // Cosine
		parsed_input.replace("tan", "t"); // Tangent
		parsed_input.replace("sqrt", "r"); // Square root
		parsed_input.replace("ln", "l"); // Logarithm

		return parsed_input;
	}

	// Conversion
	void conversion(QString const& input) { // 1 dependency - prec
		QString cur_num;
		for (auto cur_char : input)
			if (('0' <= cur_char && cur_char <= '9') || cur_char == '.') // If it's an operand
				cur_num += cur_char;
			else { // If it's an operator
				// Add what number we have
				if (cur_num != "") {
					/* unsigned short pointCount = 0;
					for (auto i : cur_num)
						pointCount += (i == '.');
					if (cur_num[0] == '.' || cur_num[cur_num.size() - 1] == '.' || pointCount > 1)
						throw(6); */
					postfix.push_back(cur_num);
					cur_num = "";
				}

				// Handle parenthesis
				if (cur_char == '(') {
					conv_push(cur_char);
					continue;
				}
				
				if (cur_char == ')') {
					while (conv_st -> contents != '(')
						postfix.push_back(QString(conv_pop()));
					conv_pop();
					continue;
				}

				// Handle other operands
				if (conv_st == nullptr || conv_st -> contents == '(' || prec(cur_char) > prec(conv_st -> contents))
					conv_push(cur_char);
				else {
					while (conv_st != nullptr && conv_st -> contents != '(' && prec(cur_char) <= prec(conv_st -> contents))
						postfix.push_back(QString(conv_pop()));
					conv_push(cur_char);
				}
			}

		// Add what we have (again)
		if (cur_num != "") {
			/* unsigned short pointCount = 0;
			for (auto i : cur_num)
				pointCount += (i == '.');
			if (cur_num[0] == '.' || cur_num[cur_num.size() - 1] == '.' || pointCount > 1)
				throw(6); */
			postfix.push_back(cur_num);
		}
		// Clear the conversion stack (finalize the conversion)
		while (conv_st != nullptr)
			postfix.push_back(QString(conv_pop()));;
	}

public:
	std::optional<double> operator()(QString const& expression, bool doThrow = false) {
		try {
			QString input = parse(expression);
			conversion(input);

			// Calculating
			for (auto i : postfix) { //! UPDATE OPERATORS HERE WHEN ADDING NEW ONES TO prec()
				double num1;
				double num2;

				switch (i[0].toLatin1()) {
					case '+':
						num2 = pop();
						num1 = pop();
						push(num1 + num2);
						break;
					case '-':
						num2 = pop();
						num1 = pop();
						push(num1 - num2);
						break;
					case '*':
						num2 = pop();
						num1 = pop();
						push(num1 * num2);
						break;
					case '/':
						num2 = pop();
						num1 = pop();
						if (num2 != 0) // Handle division by 0
							push(num1 / num2);
						else
							throw(7);
						break;
					case '^':
						num2 = pop();
						num1 = pop();
						push(pow(num1, num2));
						break;
					case 's':
						push(sin(pop()));
						break;
					case 'c':
						push(cos(pop()));
						break;
					case 't':
						push(tan(pop()));
						break;
					case 'r':
						num1 = pop();
						if (num1 >= 0) // Handle imaginary roots (we're keeping it real :speaking_head: :fire:)
							push(sqrt(num1));
						else
							throw(8);
						break;
					case 'l':
						num1 = pop();
						if (num1 >= 0) // Handle negative values, how am I supposed to know this wth
							push(log(num1));
						else
							throw(9);
						break;

					default:
						try {
							push(stod(i.toStdString()));
						} catch (...) {
							throw(10);
						}
				}
			}
		} catch (int except) {
			/*
				1 - popping from an empty conversion stack
				2 - popping from an empty stack
				3 - precedence operator reaching an unsupported character
				4 - left parenthesis outweighing right parenthesis (paren < 0)
				5 - right parenthesis outweighing left parenthesis (paren != 0 in the end)
				//6 - faulty FP number format (disabled)
				7 - division by 0
				8 - square root of a negative
				9 - log of a negative
				10 - invalid stod conversion (likely a forbidden character)
			*/

			if (doThrow)
				throw except;

			return std::nullopt;
		}

		return pop();
	}
	static std::optional<double> calc_once(QString const& expr) {
		Calc calc;
		return calc(expr);
	}
	static QList<QString> postfixList(QString const& expression, bool doThrow = false) {
		Calc calc;
		try {
			QString input = calc.parse(expression);
			calc.conversion(input);
		} catch (int except) {
			if (doThrow)
				throw except;
		}

		return calc.postfix;
	}
};

class CalcX final {
public:
	std::optional<double> operator()(QList<QString> const& postfix, double x, bool doThrow = false) {
		try {
			for (auto i : postfix) { //! Depends on the OG function
				double num1;
				double num2;

				switch (i[0].toLatin1()) {
					case '+':
						num2 = pop();
						num1 = pop();
						push(num1 + num2);
						break;
					case '-':
						num2 = pop();
						num1 = pop();
						push(num1 - num2);
						break;
					case '*':
						num2 = pop();
						num1 = pop();
						push(num1 * num2);
						break;
					case '/':
						num2 = pop();
						num1 = pop();
						if (num2 != 0) // Handle division by 0
							push(num1 / num2);
						else
							throw(7);
						break;
					case '^':
						num2 = pop();
						num1 = pop();
						push(pow(num1, num2));
						break;
					case 's':
						push(sin(pop()));
						break;
					case 'c':
						push(cos(pop()));
						break;
					case 't':
						push(tan(pop()));
						break;
					case 'r':
						num1 = pop();
						if (num1 >= 0) // Handle imaginary roots (we're keeping it real :speaking_head: :fire:)
							push(sqrt(num1));
						else
							throw(8);
						break;
					case 'l':
						num1 = pop();
						if (num1 >= 0) // Handle negative values, how am I supposed to know this wth
							push(log(num1));
						else
							throw(9);
						break;
					case 'x':
						if (i.size() == 1)
							push(x);
						else
							throw(11);
						break;

					default:
						try {
							push(stod(i.toStdString()));
						} catch (...) {
							throw(10);
						}
				}
			}
		} catch (int except) {
			// 11 - token starting with `x` is longer than 1 character

			if (doThrow)
				throw except;

			return std::nullopt;
		}

		return pop();
	}

private:
	struct Stack {
		double contents;
		Stack* next;
	};

	void push(double a) {
		Stack* new_st = (Stack*)malloc(sizeof(Stack));
		new_st -> contents = a;
		new_st -> next = st;
		st = new_st;
	}

	double pop() {
		if (st == nullptr)
			throw(2);
		double output = st -> contents;
		Stack* temp_ptr = st -> next;
		free(st);
		st = temp_ptr;
		return output;
	}

	Stack* st = nullptr;
};