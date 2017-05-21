#include <iostream>
#include <memory>
#include "Game/world.h"
#include "Game/fight.h"
#include "Neural/neural_network.h"
#include "AI/strategy.h"
#include "AI/shot_AI.h"
#include "AI/neural_AI.h"
#include "AI/hit_AI.h"
#include "Game/unit.h"
#include <ctime>
#include <fstream>
#include "GameConst.h"
#include "Random.h"

using namespace std;

void print_world(World* world) {
	vector<vector<char> > field(world->get_h(), vector<char>(world->get_w(), '.'));
	for (size_t i = 0; i < world->get_units().size(); i++) {
		auto u = world->get_units()[i];
		field[u->get_x()][u->get_y()] = '0' + static_cast<char>(i); // the number of units is less than 10
		cout << "Hp " << i << " = " << u->get_hp() << '\n';
	}

	for (auto& u : field) {
		for (auto& v : u) {
			cout << v;
		}
		cout << '\n';
	}
	cout << '\n';
}

void print_with_pause(World* world, Fight* fight) {
	print_world(world);
	fight->next_turn();
	system("pause");
	system("cls");
}


vector<Point> start_points;
vector<pair<Point, Point> > two_start_points;

void init(int n = 60) {
	start_points.clear();
	two_start_points.clear();
	for (int i = 0; i < MAX_H; i++) {
		for (int j = 0; j < MAX_W; j++) {
			start_points.emplace_back(i, j);
		}
	}
	for (auto p1 : start_points) {
		for (auto p2 : start_points) {
			if (p1 == p2) {
				continue;
			}
			two_start_points.emplace_back(p1, p2);
		}
	}
	random_shuffle(two_start_points.begin(), two_start_points.end());
	while (two_start_points.size() > n) {
		two_start_points.pop_back();
	}
}

vector<shared_ptr<Strategy> > strategies = {
	(make_shared<Shot_AI>(0)),
	(make_shared<Hit_AI>(0))
};

vector<pair<Unit, Unit> > gen_fights(Neural_network const& neural_network) {
	vector<pair<Unit, Unit> > result;
	shared_ptr<Strategy> neural_AI = make_shared<Neural_AI>(
		1,
		neural_network
	);
	for (size_t i = 0; i < strategies.size(); i++) {
		for (auto p12 : two_start_points) {
			auto p1 = p12.first;
			auto p2 = p12.second;

			Field field(MAX_H, MAX_W);
			Unit unit_first(p1, START_HP);
			Unit unit_second(p2, START_HP);
			World world(field, &unit_first, &unit_second);

			Fight fight(&world, strategies[i].get(), neural_AI.get());

			fight.play();
			result.emplace_back(unit_second, unit_first);
		}
	}
	return result;
}

double Test_neural_network(Neural_network const& neural_network) {
	auto temp = gen_fights(neural_network);
	double res = 0;
	double e = START_HP;
	double z = 0;
	for (auto& v : temp) {
		double f = v.first.get_hp() - v.second.get_hp();
		res += f;
		e = min(f, e);
		z += START_HP - v.second.get_hp();
	}
	res = (res / temp.size() * 2 + e) / 3 + z / temp.size() * 0.3;
	return res;
}

double Super_test_neural_network(Neural_network const& neural_network) {
	auto temp = gen_fights(neural_network);
	double res = 0;
	for (auto& v : temp) {
		res += v.first.get_hp() - v.second.get_hp();
	}
	return res / temp.size();
}

void fight(Strategy* first, Strategy* second) {
	Point p1, p2;
	while (p1 == p2) {
		p1.x = get_rand_int(MAX_H);
		p1.y = get_rand_int(MAX_W);
		p2.x = get_rand_int(MAX_H);
		p2.y = get_rand_int(MAX_W);
	}
	Field field(MAX_H, MAX_W);
	Unit unit_first(p1, START_HP);
	Unit unit_second(p2, START_HP);
	World world(field, &unit_first, &unit_second);
	Fight fight(&world, first, second);

	for (int i = 0; i < MAX_TIME_GAME; i++) {
		print_with_pause(&world, &fight);
	}
}

void simple_fight() {
	Shot_AI first(0);
	Hit_AI second(1);
	fight(&first, &second);
}

void hard_fight_1(Neural_network const& neural_network) {
	Neural_AI first(0, neural_network);
	Shot_AI second(1);
	fight(&first, &second);
}

void hard_fight_2(Neural_network const& neural_network) {
	Neural_AI first(0, neural_network);
	Hit_AI second(1);
	fight(&first, &second);
}

void print_coeff(Neural_coef const& a) {
	ofstream out("Result.txt", ofstream::out);
	out << MAX_H << ' ' << MAX_W << ' ' << START_HP << ' ' << MAX_TIME_GAME << '\n';
	out << a.layers_size.size() << '\n';
	for (auto v : a.layers_size) {
		out << v << ' ';
	}
	out << '\n';
	out << a.coefficient.size() << '\n';
	for (auto v : a.coefficient) {
		out << v << ' ';
	}
}

Neural_network read_coeff(string name) {
	name = "Result_" + name + ".txt";
	ifstream in(name.c_str(), ifstream::in);
	int n;
	in >> n;
	vector<size_t> layers_size(n);
	for (auto& v : layers_size) {
		in >> v;
	}
	int m;
	in >> m;
	vector<double> coefficient(m);
	for (auto& v : coefficient) {
		in >> v;
	}
	return Neural_network(Neural_coef(layers_size, coefficient));
}

double times() {
	return clock() * 1.0 / CLOCKS_PER_SEC;
}

int main() {
	srand('C' + 'T');
	init();
	strategies.push_back(
		(std::make_shared<Neural_AI>(0, read_coeff("0")))
	);
	Neural_network neural_network(vector<size_t>{14, 28, 16});
	double s = 10;
	double d = 10;
	for (int i = 0; times() < 60 * 10; i++) {
		double res = Test_neural_network(neural_network);
		if (i % 3000 == 0 && i > 0) {
			strategies.push_back(
				(std::make_shared<Neural_AI>(0, neural_network))
			);
		}
		auto coef = neural_network.get_coefficient();
		int number_it = static_cast<int>(get_rand_double(d) + 1 + d);
		for (int j = 0; j < number_it; j++) {
			int x = get_rand_int(coef.coefficient.size());
			coef.coefficient[x] += get_rand_double(s);
		}
		auto test_net = Neural_network(coef);
		double new_res = Test_neural_network(test_net);
		if (new_res > res) {
			neural_network = test_net;
			double super_test = Super_test_neural_network(test_net);
			cerr << "New result=" << new_res << ' '
				<< "Test=" << super_test << ' '
				<< "i=" << i << ' '
				<< "Time=" << times() << '\n'
				<< "S=" << s << ' '
				<< "D=" << d << ' '
				<< "Number it=" << number_it << "\n\n";
			s *= 0.99;
			d *= 0.99;
		}
	}
	print_coeff(neural_network.get_coefficient());
	while (true) {
		hard_fight_1(neural_network);
		hard_fight_2(neural_network);
	}
}
