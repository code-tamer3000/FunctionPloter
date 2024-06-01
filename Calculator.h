#include <string>
#include <unordered_map>
#include <vector>

class Calculator {
public:
  std::string getRPN(const std::string &eqation);
  std::string getDerivative(const std::string &eqation);
	std::string fromRPNtoINFIX(const std::string &eqation);
	double getValue(const std::string& RPN, double x_val = 0);
	
private:
  std::vector<std::string> splitToTokens(const std::string &eqation);
  static std::unordered_map<std::string, int> opPercedence;
  double execOp(std::string op, double val1, double val2 = 0);
};

