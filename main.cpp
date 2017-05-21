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
	system("cls");
	print_world(world);
	fight->next_turn();
	system("pause");
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

	for (int i = 0; i < 100; i++) {
		int strateg = rand() % strategies.size();
		Field field(5, 5);
		Unit unit_first(Point(0, 0), 100);
		Unit unit_second(Point(4, 4), 100);
		World world(field, &unit_first, &unit_second);

		Fight fight(&world, strategies[strateg].get(), neural_AI.get());
		result += unit_first.get_hp() - unit_second.get_hp();
	}

	return result;
}

int main() {
	Field field(5, 5);
	Shot_AI first(0);
	Hit_AI second(1);
	Unit unit_first(Point(0, 0), 100);
	Unit unit_second(Point(4, 4), 100);
	World world(field, &unit_first, &unit_second);
	Fight fight(&world, &first, &second);

	for (int i = 0; i < 100; i++) {
		print_with_pause(&world, &fight);
	}
}
