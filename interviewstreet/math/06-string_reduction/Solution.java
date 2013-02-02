import java.util.ArrayDeque;
import java.util.Deque;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Scanner;
import java.util.Set;

public class Solution {

	public static final void solveProblem(Scanner in) {
		String text = in.nextLine();
		int n = text.length();
		boolean[][][] dp = new boolean[n][n][3];
		for (int i = 0; i < n; i++)
			dp[i][i][text.charAt(i) - 'a'] = true;
		for (int j = 1; j < n; j++) {
			for (int i = 0; i + j < n; i++) {
				int k = i + j;
				for (int t = 0; t < 3; t++)
					for (int p = i; p < k; p++) {
						if (dp[i][p][(t + 1) % 3] && dp[p + 1][k][(t + 2) % 3]) {
							dp[i][k][t] = true;
							break;
						}
						if (dp[i][p][(t + 2) % 3] && dp[p + 1][k][(t + 1) % 3]) {
							dp[i][k][t] = true;
							break;
						}
					}
			}
		}

		Deque<Integer> queue = new ArrayDeque<Integer>();
		Set<Integer> visited = new HashSet<Integer>();
		Map<Integer, Integer> parent = new HashMap<Integer, Integer>();
		visited.add(0);
		queue.add(0);
		while (!queue.isEmpty()) {
			int node = queue.removeFirst();
			if (node == n)
				break;
			for (int i = node; i < n; i++)
				if (!visited.contains(i + 1)
						&& (dp[node][i][0] || dp[node][i][1] || dp[node][i][2])) {
					visited.add(i + 1);
					queue.addLast(i + 1);
					parent.put(i + 1, node);
				}
		}

		int cycle = 0;
		int x = n;
		while (x != 0) {
			x = parent.get(x);
			cycle++;
		}
		System.out.println(cycle);
	}

	public static void main(String[] args) {
		Scanner in = new Scanner(System.in);

		int numTests = Integer.parseInt(in.nextLine());
		for (int i = 0; i < numTests; i++)
			solveProblem(in);

		in.close();
	}

}
