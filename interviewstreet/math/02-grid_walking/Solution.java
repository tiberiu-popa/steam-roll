import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class Solution {

	public static final int MODULUS = 1000000007;
	public static final int MAX_LENGTH = 300;
	public static final int[][] comb = new int[MAX_LENGTH + 1][MAX_LENGTH + 1];

	static {
		for (int i = 0; i <= MAX_LENGTH; i++)
			comb[i][0] = 1;
		for (int i = 1; i <= MAX_LENGTH; i++)
			for (int j = 1; j <= i; j++)
				comb[i][j] = (comb[i - 1][j] + comb[i - 1][j - 1]) % MODULUS;
	}

	public static final int[] solve1D(int x, int d, int m) {
		int[][] dp = new int[m + 1][d];
		for (int i = 0; i < d; i++)
			dp[m][i] = 1;
		for (int i = m - 1; i >= 0; i--) {
			for (int j = 0; j < d; j++)
				for (int u = -1; u <= 1; u += 2) {
					int k = j + u;
					if (k >= 0 && k < d) {
						dp[i][j] += dp[i + 1][k];
						dp[i][j] %= MODULUS;
					}
				}
		}

		int[] result = new int[m + 1];
		for (int i = 0; i <= m; i++)
			result[i] = dp[m - i][x - 1];
		return result;
	}

	public static final int[] combine(int[] x, int[] y) {
		int m = x.length;
		int[] z = new int[m];
		for (int i = 0; i < m; i++)
			for (int j = 0; j <= i; j++) {
				int k = i - j;
				long prod = x[j];
				prod *= y[k];
				prod %= MODULUS;
				prod *= comb[i][j];
				prod %= MODULUS;
				z[i] += prod;
				z[i] %= MODULUS;
			}
		return z;
	}

	public static final void solveProblem(Scanner in) {
		int n = in.nextInt();
		int m = in.nextInt();
		List<Integer> x = new ArrayList<Integer>();
		List<Integer> d = new ArrayList<Integer>();
		for (int i = 0; i < n; i++)
			x.add(in.nextInt());
		for (int i = 0; i < n; i++)
			d.add(in.nextInt());

		int result[] = new int[m + 1];
		result[0] = 1;
		for (int i = 0; i < n; i++) {
			int[] tmp = solve1D(x.get(i), d.get(i), m);
			result = combine(result, tmp);
			// System.out.println("> " + Arrays.toString(tmp));
			// System.out.println("! " + Arrays.toString(result));
		}
		System.out.println(result[m]);
	}

	public static void main(String[] args) {
		Scanner in = new Scanner(System.in);

		int numTests = in.nextInt();
		for (int i = 0; i < numTests; i++)
			solveProblem(in);

		in.close();
	}

}
