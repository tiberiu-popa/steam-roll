#include <iostream>
#include <map>
#include <set>
#include <cstdio>
#include <climits>

using namespace std;

#define MAX_ROOMS 20
#define MAX_EXPLOSIVES 50
#define MAX_DESTROYED_ROOMS 4

#define all_rooms_mask(num_rooms) ((1 << num_rooms) - 1)
#define MAKE_MOVE(x, valid_mask) (((x << 1) | (x >> 1)) & valid_mask)

struct explosive {
	int price;
	int num_destroyed_rooms;
	int rooms[MAX_DESTROYED_ROOMS];
	int mask;
};

struct entry {

	int mask;
	int price;
	entry *parent;
	int choice;

	entry(int mask, int price, entry *parent, int choice) :
		mask(mask), price(price), parent(parent), choice(choice) { }

};

struct entry_compare {

	bool operator() (const entry *lhs, const entry *rhs)
	{
		if (lhs->price != rhs->price)
			return lhs->price < rhs->price;
		if (lhs->mask != rhs->mask)
			return lhs->mask < rhs->mask;
		return false;
	}

};

set<entry*, entry_compare> queue;
static int path[1 << MAX_ROOMS];
static int path_size = 0;

static void print_solution(entry *min_entry)
{
	FILE *f;
	int i;

	f = fopen("boom.out", "w");
	if (f != NULL) {
		fprintf(f, "%d\n", min_entry->price);
		while (min_entry != NULL) {
			path[path_size++] = min_entry->choice;
			min_entry = min_entry->parent;
		}
		fprintf(f, "%d\n", path_size - 1);
		for (i = path_size - 2; i >= 0; i--)
			fprintf(f, "%d\n", path[i] + 1);

		fclose(f);
	}
}

int main()
{
	struct explosive explosives[MAX_EXPLOSIVES];
	FILE *f;
	int num_rooms, num_explosives, num_destroyed_rooms;
	int destroyed_room;
	int cand_price, cand_mask;
	int valid_mask;
	int i, j;

	f = fopen("boom.in", "r");
	if (f == NULL)
		return 1;

	fscanf(f, "%d", &num_rooms);
	fscanf(f, "%d", &num_explosives);

	for (i = 0; i < num_explosives; i++) {
		fscanf(f, "%d", &explosives[i].price);
		fscanf(f, "%d", &num_destroyed_rooms);
		explosives[i].mask = 0;
		for (j = 0; j < num_destroyed_rooms; j++) {
			fscanf(f, "%d", &destroyed_room);
			destroyed_room--;
			explosives[i].rooms[j] = destroyed_room;
			explosives[i].mask |= (1 << destroyed_room);
		}
		explosives[i].num_destroyed_rooms = num_destroyed_rooms;
		explosives[i].mask = ~explosives[i].mask;
	}

	fclose(f);

	/*
	for (i = 0; i < num_explosives; i++) {
		printf("Sarja %d: price(%d) rooms(%d)[", i, explosives[i].price,
			explosives[i].num_destroyed_rooms);
		num_destroyed_rooms = explosives[i].num_destroyed_rooms;
		for (j = 0; j < num_destroyed_rooms; j++) {
			destroyed_room = explosives[i].rooms[j];
			printf("%s%d", (j > 0) ? "," : "", explosives[i].rooms[j]);
		}
		printf("] mask(0x%x)\n", explosives[i].mask);
	}
	*/

	valid_mask = all_rooms_mask(num_rooms);
	map<int, entry*> reverse;
	entry *elem = new entry(valid_mask, 0, NULL, -1);
	queue.insert(elem);
	reverse[valid_mask] = elem;

	while (1) {
		set<entry*, entry_compare>::iterator it = queue.begin();
		if (it == queue.end())
			break;
		entry *min_entry = *it;
		queue.erase(it);
		//printf("min_index: %d mask(0x%x)\n", min_index, min_mask);

		if (min_entry->mask == 0) {
			print_solution(min_entry);
			break;
		}

		for (i = 0; i < num_explosives; i++) {
			cand_mask = min_entry->mask & explosives[i].mask;
			cand_mask = MAKE_MOVE(cand_mask, valid_mask);
			cand_price = min_entry->price + explosives[i].price;
			entry *next_entry = reverse[cand_mask];
			if (next_entry != NULL) {
				if (queue.erase(next_entry) > 0) {
					if (cand_price < next_entry->price) {
						next_entry->price = cand_price;
						next_entry->parent = min_entry;
						next_entry->choice = i;
					}
					queue.insert(next_entry);
				}
			} else {
				next_entry = new entry(cand_mask, cand_price, min_entry, i);
				queue.insert(next_entry);
				reverse[cand_mask] = next_entry;
			}
		}
	}

	return 0;
}
