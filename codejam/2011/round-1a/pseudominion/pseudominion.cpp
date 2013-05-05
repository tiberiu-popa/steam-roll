#include <algorithm>
#include <functional>
#include <unordered_map>
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstring>

#define MAX_N 80

using namespace std;

struct vertex {

	int num_cards;
	int turns_left;
	size_t c_idx[2];

	vertex(int num_cards, int turns_left) : num_cards(num_cards), turns_left(turns_left)
	{
		memset(c_idx, 0, sizeof(c_idx));
	}

	bool operator== (const vertex &other) const
	{
		return num_cards == other.num_cards && turns_left == other.turns_left &&
			c_idx[0] == other.c_idx[0] && c_idx[1] == other.c_idx[1];
	}

};

namespace std {

	template <>
	struct hash<vertex> {

		size_t operator() (const vertex &t) const
		{
			size_t x = hash_pair(t.num_cards, t.turns_left);
			size_t y = hash_pair(t.c_idx[0], t.c_idx[1]);
			return hash_pair(x, y);
		}

private:

		size_t hash_pair(int x, int y) const
		{
			hash<int> hash_fn;
			size_t seed = hash_fn(x);
			return hash_fn(y) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		}

	};

}

unordered_map<vertex, int> diff_scores;
int c[MAX_N];
int s[MAX_N];
int t[MAX_N];
int dp[MAX_N + 1][MAX_N + 1];
vector<int> v[3];

int depth = 0;

int build_graph(const vertex &root, int n, int score)
{
	auto it = diff_scores.find(root);
	if (it != diff_scores.end())
		return score + it->second;

	size_t nc_idx[2];
	int best_score = score + dp[root.num_cards][min(root.turns_left, root.num_cards)];
	if (root.turns_left == 0) {
		diff_scores[root] = best_score - score;
		return best_score;
	}

	for (int i = 0; i < 2; i++)
		nc_idx[i] = min(root.c_idx[i] + 1, v[i + 1].size());

	++depth;

	for (int i = 0; i < 2; i++) {
		size_t idx = root.c_idx[i];
		if (nc_idx[i] != idx) {
			int pos = v[i + 1][idx];
			if (pos >= root.num_cards)
				continue;
			int num_cards = min(root.num_cards + (i + 1), n);
			int next_turns_left = root.turns_left + (t[pos] - 1);
			int next_score = score + s[pos];

			if (next_turns_left > 0) {
				for (int j = root.num_cards; j < num_cards; j++)
					if (t[j] > 0) {
						next_turns_left += t[j] - 1;
						next_score += s[j];
						int d = min(n - num_cards, c[j]);
						num_cards += d;
					}
			}

			vertex node(num_cards, next_turns_left);
			for (int j = 0; j < 2; j++)
				if (j == i)
					node.c_idx[j] = nc_idx[j];
				else
					node.c_idx[j] = root.c_idx[j];
			int cand_score = build_graph(node, n, next_score);
			if (cand_score > best_score)
				best_score = cand_score;

			vertex skip_node(root.num_cards, root.turns_left);
			for (int j = 0; j < 2; j++)
				if (j == i)
					skip_node.c_idx[j] = nc_idx[j];
				else
					skip_node.c_idx[j] = root.c_idx[j];
			cand_score = build_graph(skip_node, n, score);
			if (cand_score > best_score)
				best_score = cand_score;
		}
	}

	--depth;

	diff_scores[root] = best_score - score;

	return best_score;
}

int solve_problem(int num_case)
{
	int n, m;

	if (scanf("%d", &n) != 1)
		return 1;

	for (int i = 0; i < n; i++) {
		if (scanf("%d %d %d", &c[i], &s[i], &t[i]) != 3)
			return 1;
	}

	if (scanf("%d", &m) != 1)
		return 1;

	for (int i = 0; i < m; i++) {
		if (scanf("%d %d %d", &c[n + i], &s[n + i], &t[n + i]) != 3)
			return 1;
	}

	diff_scores.clear();
	for (int i = 0; i < 3; i++)
		v[i].clear();

	int turns_left = 1;
	int score = 0;
	for (int i = 0; i < n + m; i++)
		if (t[i] > 0) {
			if (i < n) {
				turns_left += t[i] - 1;
				score += s[i];
				int d = min(m, c[i]);
				n += d;
				m -= d;
			}
		} else {
			v[c[i]].push_back(i);
		}

	memset(dp, 0, sizeof(dp));
	int prev_index = 0;
	for (int pos : v[0]) {
		int new_index = pos + 1;
		for (int i = prev_index + 1; i < new_index; i++)
			for (int j = 0; j <= i; j++)
				dp[i][j] = dp[prev_index][j];
		for (int j = 0; j <= new_index; j++) {
			dp[new_index][j] = dp[prev_index][j];
			if (j > 0) {
				int cand = dp[prev_index][j - 1] + s[pos];
				if (cand > dp[new_index][j])
					dp[new_index][j] = cand;
			}
		}
		prev_index = new_index;
	}
	if (prev_index > 0) {
		for (int i = prev_index + 1; i <= n + m; i++)
			for (int j = 0; j <= i; j++)
				dp[i][j] = dp[prev_index - 1][j];
	}
	for (int i = 1; i <= n + m; i++)
		for (int j = 1; j <= i; j++) {
			if (dp[i][j - 1] > dp[i][j])
				dp[i][j] = dp[i][j - 1];
			if (dp[i - 1][j] > dp[i][j])
				dp[i][j] = dp[i - 1][j];
		}

	vertex root(n, turns_left);

	int result = build_graph(root, n + m, score);

	printf("Case #%d: %d\n", num_case, result);

	return 0;
}

int main(int argc, char *argv[])
{
	const char *filenames[] = {
		"pseudominion.in",
		"pseudominion.out",
	};
	int num_tests;

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

	if (scanf("%d", &num_tests) != 1)
		return 1;
	for (int i = 0; i < num_tests; i++)
		solve_problem(i + 1);

	return 0;
}
