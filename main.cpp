#define GNUPLOT_ENABLE_PTY
#include <gnuplot-iostream.h>

#include <iostream>
#include <vector>

#include "Calculator.h"

std::vector<std::pair<double, double>> makeTuple(std::vector<double>& first, std::vector<double>& second) {
	std::vector<std::pair<double, double>> ans;
	for (int i = 0; i < first.size() ; i++ ) {
		ans.push_back(std::make_pair(first[i], second[i]));
	}
	return ans;
} 
void saveDataToFile(const std::vector<std::pair<double, double>>& data, const std::string& filename) {
    std::ofstream file(filename);
    for (const auto& point : data) {
        file << point.first << " " << point.second << "\n";
    }
}

int main() {
  std::string exp;
  std::getline(std::cin, exp);
  Calculator a;
  std::string b = a.getRPN(exp);
  std::string c = a.getDerivative(b);
  // Промежуток от -10 до 10 с шагом 0.1
  double start = -10;
  double end = 10;
  double step = 0.1;

  std::vector<double> x;
  std::vector<double> y;

  // Заполнение векторов x и y
  for (double i = start; i <= end; i += step) {
    x.push_back(i);
    y.push_back(a.getValue(b, i));  // Пример функции y = sin(x)
  }

  // Создаем объект Gnuplot
  Gnuplot gp;
  std::string title = "График функции y = " + exp;
  // Выводим график
	gp << "set yrange[-5 : 5]\n";
	gp << "set xrange[-10: 10]\n";
  gp << "set mxtics 5\n";
  gp << "set mytics 5\n";
  gp << "set grid mxtics mytics\n";
	gp << "set tmargin at screen 0.80\n";
  gp << "set label 1 \"" << title << "\" at graph 0.5, screen 0.95 center front font \",10\" textcolor lt -1\n";
	gp << "set label 2 \"Где производная dx/dy = " +  a.fromRPNtoINFIX(c) + "\" at graph 0.5, screen 0.88 center front font \",10\" textcolor lt -1\n";
	saveDataToFile(makeTuple(x, y), "plot.dat");
  gp << "plot 'plot.dat' with lines lc rgb \"black\" title '" << title << "'\n";
	int mouse_type = 0;
	double mouse_x, mouse_y;
	while (mouse_type != 3) {
		gp.getMouse(mouse_x, mouse_y, mouse_type);
		std::vector<double> x1;
		std::vector<double> y1;
		for (int i = -10; i < 11; i+= 20) {
			x1.push_back(mouse_x + i);
			y1.push_back(a.getValue(b, mouse_x) + a.getValue(c, mouse_x ) * i);
		}
		saveDataToFile(makeTuple(x1, y1), "tangent.dat");
    gp << "replot 'plot.dat' with lines lc rgb \"black\" notitle,";
		gp << "'tangent.dat' with lines lc rgb \"red\" notitle\n";
	}
	gp << "q\n";
  return 0;
}

int main1() {
	Calculator a;
	std::string eqation;
	std::string rpn;
	double v;
	while (true) {
	std::cout << '\n';
	std::cout << "Enter the eqation\n";
	std::getline(std::cin, eqation); 
	try {
		rpn = a.getRPN(eqation);
		v = a.getValue(rpn); 
	} catch (std::exception& ex) {
		std::cout << "Something went wrong: " << ex.what() << "\n";
		rpn.clear();
		continue;
	} 
	std::cout << "Infix: " << eqation << '\n';
	std::cout << "Postfix: " << rpn << '\n';
	std::cout << "Value of eqation: " << v << '\n';
	}
}	
