#include <iostream>
#include "world.h"
#include "random_AI.h"
#include "fight.h"
#include "shot_AI.h"
#include "hit_AI.h"

using namespace std;

void print_world(World* world) {
	vector<vector<char> > field(world->get_h(), vector<char>(world->get_w(), '.'));
	for (size_t i = 0; i < world->get_units().size(); i++) {
		auto u = world->get_units()[i];
		field[u->get_x()][u->get_y()] = '0' + static_cast<char>(i); // the number of units is less than 10
		cout << "Hp " << i << " = " << u->get_hp() << '\n';
	}
	
	for(auto &u : field) {
		for(auto &v : u) {
			cout << v;
		}
		cout << '\n';
	}
	cout << '\n';
}

int main() {
	Field field(5, 5);
	Shot_AI first(0);
	Hit_AI second(1);
	Unit unit_first(Point(0, 0), 100);
	Unit unit_second(Point(4, 4), 100);
	World world(field, &unit_first, &unit_second);
	Fight fight(&world, &first, &second);

	for(int i = 0; i < 100; i++) {
		system("cls");
		print_world(&world);
		fight.next_turn();
		system("pause");
	}
}