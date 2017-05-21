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

vector<Point> start_points;
vector<pair<Point, Point> > two_start_points;
vector<unique_ptr<Strategy> > strategies;

void print_with_pause(World const& world, Fight& fight) {
	cout << world;
	fight.next_turn();
	system("pause");
	system("cls");
}

void init(int n = 60) {
	two_start_points.clear();
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


vector<pair<Unit, Unit> > gen_fights(Neural_network const& neural_network) {
	vector<pair<Unit, Unit> > result;
	shared_ptr<Strategy> neural_AI = make_shared<Neural_AI>(
		1,
		neural_network
	);
	for (size_t i = 0; i < strategies.size(); i++) {
		for (auto p12 : two_start_points) {
			Unit unit_first(p12.first, START_HP);
			Unit unit_second(p12.second, START_HP);
			Field field(MAX_H, MAX_W);
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
	double z = 0;
	double e = START_HP;
	for (auto& v : temp) {
		double f = v.first.get_hp() - v.second.get_hp();
		if (v.first.get_hp() == 0) {
			f -= 5;
		}
		if (v.second.get_hp() == 0) {
			f += 5;
		}
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
		print_with_pause(world, fight);
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
	out << a << '\n' << MAX_H << ' ' << MAX_W << ' ' << START_HP << ' ' << MAX_TIME_GAME << '\n';
}

Neural_network read_coeff(string name) {
	name = "Result_" + name + ".txt";
	ifstream in(name.c_str(), ifstream::in);
	int n, m;
	in >> n;
	vector<size_t> layers_size(n);
	for (auto& v : layers_size) {
		in >> v;
	}
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

void start() {
	srand('C' + 'T');
	strategies.push_back(
		make_unique<Shot_AI>(0)
	);
	strategies.push_back(
		make_unique<Hit_AI>(0)
	);
	strategies.push_back(
		make_unique<Neural_AI>(0, read_coeff("0"))
	);
	strategies.push_back(
		make_unique<Neural_AI>(0, read_coeff("1"))
	);
	for (int i = 0; i < MAX_H; i++) {
		for (int j = 0; j < MAX_W; j++) {
			start_points.emplace_back(i, j);
		}
	}
	init();
}

int main() {
	start();

	Neural_network neural_network(vector<size_t>{14, 20, 20, 16});
	double res = Test_neural_network(neural_network);
	double s = 10;
	double d = 10;
	int time_to_end = 60 * 60 * 7;
	cerr << time_to_end << '\n';

	for (int i = 0, last_new = 0, last_added = 0; times() < time_to_end; i++) {
		if (i % 5000 == 0 && i > 0 && last_added != last_new) {
			last_added = last_new;
			strategies.push_back(
				make_unique<Neural_AI>(0, neural_network)
			);
			if (strategies.size() > 7) {
				strategies.erase(strategies.begin() + 2);
			}
		}
		if (i == 100000) {
			init(130);
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
			res = new_res;
			last_new = i;
			neural_network = test_net;
			cerr << "New result=" << new_res << ' '
				<< "Test=" << Super_test_neural_network(test_net) << ' '
				<< "i=" << i << ' '
				<< "Time=" << times() << '\n'
				<< "S=" << s << ' '
				<< "D=" << d << ' '
				<< "Number it=" << number_it << "\n\n";
			s *= 0.99;
			d *= 0.99;
			s = max(s, 0.5);
			d = max(d, 2.0);
		}
	}
	print_coeff(neural_network.get_coefficient());
	while (true) {
		hard_fight_1(neural_network);
		hard_fight_2(neural_network);
	}
}
