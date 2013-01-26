import java.util.Scanner;

public class Solution {

	private static final int MOD_PRIME = 1000000007;

	public static int addModulo(int x, int y) {
		int s = x + y;
		if (s >= MOD_PRIME)
			s -= MOD_PRIME;
		return s;
	}

	public static int subtractModulo(int x, int y) {
		int d = x - y;
		if (d < 0)
			d += MOD_PRIME;
		return d;
	}

	public static final void solveProblem(Scanner in) {
		String line = in.nextLine();
		char[] arr = line.toCharArray();
		int n = line.length();

		// dp[k][i][j] = num seqs in [i..j] equal to a sequence in [k..n-1]
		int[][][] dp = new int[n][n][n];
		for (int i = n - 2; i >= 0; i--) {
			if (arr[i] == arr[n - 1])
				dp[n - 1][i][i] = 1;
			for (int j = i + 1; j < n - 1; j++) {
				dp[n - 1][i][j] = addModulo(dp[n - 1][i][j - 1],
						dp[n - 1][j][j]);
			}
		}
		for (int k = n - 2; k >= 0; k--)
			for (int i = k - 1; i >= 0; i--)
				for (int j = i; j < k; j++) {
					int value = subtractModulo(dp[k + 1][i][j],
							dp[k + 1][i + 1][j]);
					value = addModulo(value, dp[k][i + 1][j]);
					// It's Ok, since the sum is at most 2 * MOD_PRIME - 1
					if (arr[k] == arr[i])
						value = addModulo(value, 1 + dp[k + 1][i + 1][j]);
					dp[k][i][j] = value;
				}
		// System.out.println(Arrays.deepToString(dp));

		int answer = 0;
		for (int i = 0; i < n - 1; i++)
			answer = addModulo(answer, dp[i + 1][0][i]);
		for (int i = 0; i < n - 2; i++)
			answer = subtractModulo(answer, dp[i + 2][0][i]);
		System.out.println(answer);
	}

	public static void main(String[] args) {
		Scanner in = new Scanner(System.in);

		int numTests = Integer.parseInt(in.nextLine());
		for (int i = 0; i < numTests; i++)
			solveProblem(in);

		in.close();
	}

}
