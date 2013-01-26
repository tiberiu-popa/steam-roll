import java.util.BitSet;
import java.util.HashSet;
import java.util.Scanner;

public class Solution {

	private static class IntegerList extends HashSet<Integer> {

		private static final long serialVersionUID = -1354922756358977782L;

	}

	private static final int MAX_HEAP_SIZE = 50;
	private static final int[] mex = new int[MAX_HEAP_SIZE + 1];

	static {
		IntegerList[][] dp = new IntegerList[MAX_HEAP_SIZE + 1][MAX_HEAP_SIZE + 1];
		for (int i = 0; i <= MAX_HEAP_SIZE; i++)
			for (int j = 0; j <= MAX_HEAP_SIZE; j++)
				dp[i][j] = new IntegerList();
		dp[1][1].add(0);
		dp[2][2].add(0);

		// http://www.gabrielnivasch.org/fun/combinatorial-games/sprague-grundy
		for (int i = 3; i <= MAX_HEAP_SIZE; i++) {
			for (int j = 1; j <= i; j++) {
				for (int k = 1; k < j; k++)
					if (!dp[i - j][k].isEmpty()) {
						for (Integer bit : dp[i - j][k])
							dp[i][j].add(bit ^ mex[j]);
					}
			}
			BitSet bs = new BitSet(0);
			for (int j = 1; j <= i; j++)
				for (Integer bit : dp[i][j])
					bs.set(bit);
			mex[i] = bs.nextClearBit(0);
			dp[i][i].add(mex[i]);
		}
	}

	public static final void solveProblem(Scanner in) {
		int n = in.nextInt();
		int result = 0;
		for (int i = 0; i < n; i++)
			result ^= mex[in.nextInt()];
		System.out.println((result == 0) ? "BOB" : "ALICE");
	}

	public static void main(String[] args) {
		Scanner in = new Scanner(System.in);

		int numTests = in.nextInt();
		for (int i = 0; i < numTests; i++)
			solveProblem(in);

		in.close();
	}

}
