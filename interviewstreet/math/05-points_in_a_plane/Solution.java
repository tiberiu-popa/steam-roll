import java.util.BitSet;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Scanner;
import java.util.Set;

public class Solution {

	public static final int MODULUS = 1000000007;

	public static class HyperMap {

		private final Map<Integer, Set<Integer>> map = new HashMap<Integer, Set<Integer>>();

		public void add(BitSet bs, int value) {
			int lsb = bs.nextSetBit(0);
			Set<Integer> set = map.get(lsb);
			if (set == null) {
				set = new HashSet<Integer>();
				map.put(lsb, set);
			}
			set.add(value);
		}

		public Set<Integer> get(int lsb) {
			return map.get(lsb);
		}

	}

	public static final boolean checkCollinear(int[] x, int[] y, int p, int q,
			int r) {
		int u = (y[r] - y[p]) * (x[q] - x[p]);
		int v = (x[r] - x[p]) * (y[q] - y[p]);
		return u == v;
	}

	public static final HyperMap buildCollinears(int[] x, int[] y, int n) {
		HyperMap collinears = new HyperMap();
		BitSet bs = new BitSet(n + 1);
		int k = 0;

		while (!bs.get(n)) {
			if (!bs.isEmpty()) {
				boolean collinear = true;
				int fst = bs.nextSetBit(0);
				int snd = bs.nextSetBit(fst + 1);
				if (snd >= 0) {
					int p = bs.nextSetBit(snd + 1);
					while (p >= 0) {
						if (!checkCollinear(x, y, fst, snd, p)) {
							collinear = false;
							break;
						}
						p = bs.nextSetBit(p + 1);
					}
				}
				if (collinear)
					collinears.add(bs, k);
			}
			k++;
			int pos = bs.nextClearBit(0);
			bs.set(pos);
			bs.clear(0, pos);
		}

		return collinears;
	}

	public static final void solveProblem(Scanner in) {
		int n = in.nextInt();
		int[] x = new int[n];
		int[] y = new int[n];
		for (int i = 0; i < n; i++) {
			x[i] = in.nextInt();
			y[i] = in.nextInt();
		}

		HyperMap collinears = buildCollinears(x, y, n);

		int[][] dp = new int[n + 1][1 << n];
		dp[0][(1 << n) - 1] = 1;
		for (int i = 0; i <= n; i++) {
			for (int j = 1; j < dp[i].length; j++) {
				if (dp[i][j] == 0)
					continue;
				int pos = 0;
				for (int tmp = j; tmp % 2 == 0; tmp >>= 1)
					pos++;
				for (Integer mask : collinears.get(pos)) {
					if ((mask & j) == mask) {
						int reduced = j & ~mask;
						dp[i + 1][reduced] += dp[i][j];
						dp[i + 1][reduced] %= MODULUS;
					}
				}
			}
			if (dp[i][0] > 0) {
				long prod = dp[i][0];
				for (int j = 1; j <= i; j++) {
					prod *= j;
					prod %= MODULUS;
				}
				System.out.println(i + " " + prod);
				break;
			}
		}
	}

	public static void main(String[] args) {
		Scanner in = new Scanner(System.in);

		int numTests = in.nextInt();
		for (int i = 0; i < numTests; i++)
			solveProblem(in);

		in.close();
	}

}
