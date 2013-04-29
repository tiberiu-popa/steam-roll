#include <algorithm>
#include <set>
#include <cmath>
#include <cstdio>
#include <cstring>

#define BUFFER_SIZE 100
#define MAX_N 100

using namespace std;

struct train_time {

	int hour, minute;
	int direction;

	train_time & operator+= (int turnaround)
	{
		minute += turnaround;
		hour += minute / 60;
		minute %= 60;
		return *this;
	}

	bool operator< (const train_time &rhs) const
	{
		if (hour != rhs.hour)
			return hour < rhs.hour;
		return minute < rhs.minute;
	}

	bool operator<= (const train_time &rhs) const
	{
		if (hour != rhs.hour)
			return hour < rhs.hour;
		return minute <= rhs.minute;
	}

};

char *trim_line(char *str)
{
	return strtok(str, "\r\n");
}

void read_train_time(struct train_time &obj, char *str, int direction)
{
	char *p = strtok(str, ":");
	obj.hour = atoi(p);
	p = strtok(NULL, " ");
	obj.minute = atoi(p);
	obj.direction = direction;
}

int solve_problem(int num_case)
{
	int turnaround;
	int na, nb;
	pair<train_time, train_time> timetable[2 * MAX_N];
	multiset<train_time> trains;
	int waiting[2];
	int result[2];
	char str[BUFFER_SIZE + 5];

	if (fgets(str, sizeof(str), stdin) == NULL)
		return 1;
	turnaround = atoi(str);

	if (fgets(str, sizeof(str), stdin) == NULL)
		return 1;
	na = atoi(strtok(str, " "));
	nb = atoi(strtok(NULL, " "));

	for (int i = 0; i < na; i++) {
		if (fgets(str, sizeof(str), stdin) == NULL)
			return 1;
		read_train_time(timetable[i].first, str, 0);
		read_train_time(timetable[i].second, NULL, 0);
		timetable[i].second += turnaround;
	}

	for (int i = 0; i < nb; i++) {
		if (fgets(str, sizeof(str), stdin) == NULL)
			return 1;
		read_train_time(timetable[na + i].first, str, 1);
		read_train_time(timetable[na + i].second, NULL, 1);
		timetable[na + i].second += turnaround;
	}

	memset(waiting, 0, sizeof(waiting));
	memset(result, 0, sizeof(result));

	sort(timetable, timetable + na + nb);
	for (int i = 0; i < na + nb; i++) {
		int dir = timetable[i].first.direction;
		multiset<train_time>::iterator u_it = trains.upper_bound(timetable[i].first);
		for (multiset<train_time>::iterator it = trains.begin(); it != u_it; ++it) {
			trains.erase(it);
			waiting[it->direction ^ 1]++;
		}
		if (waiting[dir] > 0)
			waiting[dir]--;
		else
			result[dir]++;
		trains.insert(timetable[i].second);
	}

	printf("Case #%d: %d %d\n", num_case, result[0], result[1]);
	return 0;
}

int main(int argc, char *argv[])
{
	const char *filenames[] = {
		"train-timetable.in",
		"train-timetable.out",
	};
	int num_tests;
	char str[BUFFER_SIZE + 5];

	for (int i = 1; i < argc && i <= 2; i++)
		filenames[i - 1] = argv[i];

	if (freopen(filenames[0], "rt", stdin) == NULL) {
		fprintf(stderr, "Could not reopen stdin\n");
		return 1;
	}
	if (freopen(filenames[1], "wt", stdout) == NULL) {
		fprintf(stderr, "Could not reopen stdout\n");
		return 1;
	}

	if (fgets(str, sizeof(str), stdin) == NULL)
		return 1;
	num_tests = atoi(str);
	for (int i = 0; i < num_tests; i++)
		solve_problem(i + 1);

	return 0;
}
