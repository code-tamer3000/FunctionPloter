#include "Calculator.h"

#include <cmath>
#include <sstream>
#include <stack>
#include <stdexcept>
std::unordered_map<std::string, int> Calculator::opPercedence = {
    {"+", 1}, {"(", 0}, {")", 0},   {"-", 1},  {"+u", 1},  {"-u", 1},
    {"*", 2}, {"/", 2}, {"sin", 4}, {"ln", 4}, {"cos", 4}, {"^", 3}};

std::vector<std::string> Calculator::splitToTokens(const std::string &eqation) {
  if (eqation.empty())
    throw std::invalid_argument("Given the empty expression!");
  std::vector<std::string> tokens;
  for (size_t i = 0; i < eqation.size(); i++) {
    if (eqation[i] == ' ') continue;
    size_t start_iter = i;
    while (i < eqation.size() && std::isdigit(eqation[i]) || eqation[i] == '.')
      i++;
    if (start_iter != i) {
      tokens.push_back(eqation.substr(start_iter, i - start_iter));
      --i;
      continue;
    }
    if (opPercedence.contains(std::string(1, eqation[i]))) {
      if ((eqation[i] == '+' || eqation[i] == '-') &&
          (tokens.empty() || tokens[tokens.size() - 1][0] == '(')) {
        std::string tmp(1, eqation[i]);
        tmp += "u";
        tokens.push_back(tmp);
        continue;
      }
      tokens.emplace_back(std::string(1, eqation[i]));

    } else if (i + 2 < eqation.size() &&
               opPercedence.contains(eqation.substr(i, 3))) {
      tokens.push_back(eqation.substr(i, 3));
      i += 2;
    } else if (i + 1 < eqation.size() &&
               opPercedence.contains(eqation.substr(i, 2))) {
      tokens.push_back(eqation.substr(i, 2));
      i += 1;
    } else if (std::isalpha(eqation[i])) {
      std::string tmp(1, eqation[i]);
      tokens.push_back(tmp);
    } else {
      throw std::invalid_argument("Given the uncorect eqation!");
    }
  }
  return tokens;
}

std::string Calculator::getRPN(const std::string &eqation) {
  std::vector<std::string> tokens = splitToTokens(eqation);
  std::string rpn;

  std::stack<std::string> stack;
  for (auto token : tokens) {
    if (std::isdigit(token[0]) || token[0] == 'x') {
      rpn += token;
      rpn += " ";
    } else if (token == "(") {
      stack.push(token);
    } else if (token == ")") {
      while (!stack.empty() && !(stack.top() == "(")) {
        rpn += stack.top();
        stack.pop();
        rpn += " ";
      }
      if (!stack.empty())
        stack.pop();
      else
        throw std::invalid_argument("Wrong bracket seqence!");
    } else {
      while (!stack.empty() &&
             opPercedence[token] <= opPercedence[stack.top()]) {
        if ((opPercedence[token] == opPercedence[stack.top()]) && token == "^")
          break;
        rpn += stack.top();
        stack.pop();
        rpn += " ";
      }
      stack.push(token);
    }
  }
  while (!stack.empty()) {
    if (stack.top() == "(")
      throw std::invalid_argument("Wrong bracket seqence");
    rpn += stack.top();
    stack.pop();
    rpn += " ";
  }
  rpn.erase(rpn.size() - 1, 1);
  return rpn;
}

double Calculator::execOp(std::string op, double val1, double val2) {
  if (op == "+") {
    return val1 + val2;
  } else if (op == "-") {
    return val1 - val2;
  } else if (op == "*") {
    return val1 * val2;
  } else if (op == "/") {
    return val1 / val2;
  } else if (op == "-u") {
    return -val1;
  } else if (op == "+u") {
    return val1;
  } else if (op == "^") {
    return std::pow(val1, val2);
  } else if (op == "sin") {
    return sin(val1);
  } else if (op == "cos") {
    return cos(val1);
  } else if (op == "ln") {
    return std::log(val1);
  }
	return 0;
}
bool isUnary(std::string op) {
  return op == "-u" || op == "+u" || op == "sin" || op == "cos" || op == "ln";
}
double Calculator::getValue(const std::string &rpn, double x_val) {
  std::stack<std::string> stack;
  std::stringstream ss(rpn);
  std::string token;
  while (std::getline(ss, token, ' ')) {
    if (std::isdigit(token[0])) {
      stack.push(token);
    } else if (token[0] == 'x') {
      stack.push(std::to_string(x_val));
    } else {
      double first = std::stod(stack.top());
      stack.pop();
      if (isUnary(token)) {
        stack.push(std::to_string(execOp(token, first)));
      } else {
        double second = (stack.top() == "x" ? x_val : std::stod(stack.top()));
        stack.pop();
        stack.push(std::to_string(execOp(token, second, first)));
      }
    }
  }
  return std::stod(stack.top());
}

std::string Calculator::getDerivative(const std::string &eqation) {
  std::stack<std::string> stackDir;
  std::stack<std::string> stackEq;
  std::stringstream ss(eqation);
  std::string token;
  while (std::getline(ss, token, ' ')) {
    if (std::isdigit(token[0])) {
      stackEq.push(token);
      stackDir.push("0");
    } else if (token == "x") {
      stackEq.push("x");
      stackDir.push("1");
    } else if (token == "+") {
      std::string x2 = stackEq.top();
      stackEq.pop();
      std::string x1 = stackEq.top();
      stackEq.pop();
      stackEq.push(x1 + ' ' + x2 + ' ' + token);
      x2 = stackDir.top();
      stackDir.pop();
      x1 = stackDir.top();
      stackDir.pop();
      stackDir.push(x1 + ' ' + x2 + ' ' + token);
    } else if (token == "-") {
      std::string x2 = stackEq.top();
      stackEq.pop();
      std::string x1 = stackEq.top();
      stackEq.pop();
      stackEq.push(x1 + ' ' + x2 + ' ' + token);
      x2 = stackDir.top();
      stackDir.pop();
      x1 = stackDir.top();
      stackDir.pop();
      stackDir.push(x1 + ' ' + x2 + ' ' + token);
    } else if (token == "*") {
      std::string x2 = stackEq.top();
      stackEq.pop();
      std::string x1 = stackEq.top();
      stackEq.pop();
      std::string dx2 = stackDir.top();
      stackDir.pop();
      std::string dx1 = stackDir.top();
      stackDir.pop();
      stackDir.push(dx1 + ' ' + x2 + " * " + dx2 + ' ' + x1 + " * +");
      stackEq.push(x1 + ' ' + x2 + ' ' + token);
    } else if (token == "/") {
      std::string x2 = stackEq.top();
      stackEq.pop();
      std::string x1 = stackEq.top();
      stackEq.pop();
      std::string dx2 = stackDir.top();
      stackDir.pop();
      std::string dx1 = stackDir.top();
      stackDir.pop();
      stackDir.push(dx1 + ' ' + x2 + " * " + dx2 + ' ' + x1 + " * - " + x2 +
                    " / " + x2 + " /");
      stackEq.push(x1 + ' ' + x2 + ' ' + token);
    } else if (token == "^") {
      std::string x2 = stackEq.top();
      stackEq.pop();
      std::string x1 = stackEq.top();
      stackEq.pop();
      std::string dx2 = stackDir.top();
      stackDir.pop();
      std::string dx1 = stackDir.top();
      stackDir.pop();
      stackEq.push(x1 + ' ' + x2 + ' ' + token);
      if (x2.find(' ') == std::string::npos && std::isdigit(x2[0])) {
        stackDir.push(x2 + ' ' + x1 + ' ' + std::to_string(std::stod(x2) - 1) +
                      " ^ * " + dx1 + " *");
      } else if (x1.find(' ') == std::string::npos && std::isdigit(x1[0])) {
        stackDir.push(x1 + ' ' + x2 + " ^ " + x1 + " ln * " + dx2 + " *");
      } else {
        stackDir.push(x2 + ' ' + dx1 + " * " + x1 + ' ' + x2 + " 1 - ^ * " +
                      x1 + " ln " + dx2 + " * " + x1 + ' ' + x2 + " ^ * +");
      }
    } else if (token == "sin") {
      std::string x1 = stackEq.top();
      stackEq.pop();
      std::string dx1 = stackDir.top();
      stackDir.pop();
      stackEq.push(x1 + " sin");
      stackDir.push(x1 + " cos " + dx1 + " *");
    } else if (token == "cos") {
      std::string x1 = stackEq.top();
      stackEq.pop();
      std::string dx1 = stackDir.top();
      stackDir.pop();
      stackEq.push(x1 + " cos");
      stackDir.push(x1 + " sin " + dx1 + " * -u");
    } else if (token == "ln") {
      std::string x1 = stackEq.top();
      stackEq.pop();
      std::string dx1 = stackDir.top();
      stackDir.pop();
      stackEq.push(x1 + " ln");
      stackDir.push(dx1 + ' ' + x1 + " /");
    } else if (token == "-u") {
      std::string x1 = stackEq.top();
      stackEq.pop();
      std::string dx1 = stackDir.top();
      stackDir.pop();
      stackEq.push(x1 + " -u");
      stackDir.push(dx1 + " -u");
    } else if (token == "+u") {
      std::string x1 = stackEq.top();
      stackEq.pop();
      std::string dx1 = stackDir.top();
      stackDir.pop();
      stackEq.push(x1 + " +u");
      stackDir.push(dx1 + " +u");
    }
  }
  return stackDir.top();
}

std::string Calculator::fromRPNtoINFIX(const std::string &eqation) {

  std::vector<std::string> tokens = splitToTokens(eqation);
  std::string infix;

  std::stack<std::string> stack;
	
	for (auto token : tokens) {
		
		if (std::isdigit(token[0]) || token == "x") {
			stack.push(token);
		} else if (isUnary(token)) {
			std::string newToken = "(" + token  + stack.top() + ")";
			stack.pop();
			stack.push(newToken);
		} else {
			std::string second = stack.top();
			stack.pop();
			std::string first = stack.top();
			stack.pop();

			stack.push( "(" + first + token + second + ")" );
		} 

	}

  return stack.top();
}
