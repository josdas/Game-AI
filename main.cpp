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

using namespace std;

double get_rand_double(double s) {
	return 2 * (rand() - RAND_MAX / 2) * 1.0 / RAND_MAX * s;
}

int get_rand_int(int x) {
	return rand() % x;
}


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

double Test_neural_network(Neural_network const& neural_network) {
	double result = 0;
	vector<shared_ptr<Strategy> > strategies = {
		(std::make_shared<Shot_AI>(0)),
		(std::make_shared<Hit_AI>(0))
	};
	shared_ptr<Strategy> neural_AI = std::make_shared<Neural_AI>(
		1,
		neural_network
	);
	

	int h = 5, w = 5;
	for (int i = 0; i < 100; i++) {
		int strateg = i % 2;
		Field field(h, w);
		Point p1(0, 0);
		Point p2(0, 0);
		while(p1 == p2) {
			p1.x = get_rand_int(h);
			p1.y = get_rand_int(w);
			p2.x = get_rand_int(h);
			p2.y = get_rand_int(w);
		}
		Unit unit_first(p1, 100);
		Unit unit_second(p2, 100);
		World world(field, &unit_first, &unit_second);

		Fight fight(&world, strategies[strateg].get(), neural_AI.get());

		fight.play();
		result += unit_second.get_hp() - unit_first.get_hp();
	}

	return result;
}

void fight(Strategy* first, Strategy* second) {
	int h = 5, w = 5;
	Point p1(0, 0);
	Point p2(0, 0);
	while (p1 == p2) {
		p1.x = get_rand_int(h);
		p1.y = get_rand_int(w);
		p2.x = get_rand_int(h);
		p2.y = get_rand_int(w);
	}
	Field field(5, 5);
	Unit unit_first(p1, 100);
	Unit unit_second(p2, 100);
	World world(field, &unit_first, &unit_second);
	Fight fight(&world, first, second);

	for (int i = 0; i < 50; i++) {
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
	ofstream out("Result.txt", std::ofstream::out);
	out << a.layers_size.size() << '\n';
	for(auto v : a.layers_size) {
		out << v << ' ';
	}
	out << '\n';
	out << a.coefficient.size() << '\n';
	for (auto v : a.coefficient) {
		out << v << ' ';
	}
}

int main() {
	srand(time(0));
	Neural_network neural_network(vector<size_t> {8, 16, 16});
	double s = 1;
	double d = 100;
	for(int i = 0;; i++) {
		double res = Test_neural_network(neural_network);
		if (i % 1000 == 999) {
			cerr << res << '\n';
			hard_fight_1(neural_network);
			hard_fight_2(neural_network);

			s *= 0.99;
			d *= 0.9;
		}
		auto coef = neural_network.get_coefficient();
		for(int j = 0; j < d || j == 0; j++) {
			int x = get_rand_int(coef.coefficient.size());
			coef.coefficient[x] += get_rand_double(s);
		}
		auto test_net = Neural_network(coef);
		double new_res = Test_neural_network(test_net);
		if (new_res > res) {

			neural_network = test_net;
			cerr << new_res << ' ' << res << ' ' << i << '\n' << s << ' ' << d << "\n\n";
		}
	}

}
