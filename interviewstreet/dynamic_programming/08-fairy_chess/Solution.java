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

	public static int query(int[][] sum, int x, int y, int s) {
		int n = sum.length;
		int rx = n / 2 + x - y;
		int ry = x + y + 1;
		int high_x = Math.min(rx + s, n - 1);
		int low_x = Math.max(rx - s, 1);
		int high_y = Math.min(ry + s, n - 1);
		int low_y = Math.max(ry - s, 1);
		int pos = addModulo(sum[high_x][high_y], sum[low_x - 1][low_y - 1]);
		int neg = addModulo(sum[high_x][low_y - 1], sum[low_x - 1][high_y]);
		return subtractModulo(pos, neg);
	}

	public static void createSumTable(int[][] sum, int[][] matrix) {
		for (int i = 0; i < matrix.length; i++) {
			int partialSum = 0;
			for (int j = 0; j < matrix[i].length; j++) {
				partialSum = addModulo(partialSum, matrix[i][j]);
				sum[i][j] = partialSum;
				if (i > 0)
					sum[i][j] = addModulo(sum[i][j], sum[i - 1][j]);
			}
		}
	}

	public static final void solveProblem(Scanner in) {
		String line = in.nextLine();
		String tokens[] = line.split(" ");
		int n = Integer.parseInt(tokens[0]);
		int m = Integer.parseInt(tokens[1]);
		int s = Integer.parseInt(tokens[2]);

		int[][] rotated = new int[2 * n][2 * n];
		int[][] sum = new int[2 * n][2 * n];
		int[][] next = new int[2 * n][2 * n];

		int lx = -1, ly = -1;
		for (int i = 0; i < n; i++) {
			line = in.nextLine();
			for (int j = 0; j < line.length(); j++) {
				char ch = line.charAt(j);
				if (ch != 'P') {
					rotated[n + i - j][i + j + 1] = 1;
					if (ch == 'L') {
						lx = i;
						ly = j;
					}
				}
			}
		}
		createSumTable(sum, rotated);

		for (int k = 2; k <= m; k++) {
			for (int i = 0; i < n; i++)
				for (int j = 0; j < n; j++) {
					int x = n + i - j;
					int y = i + j + 1;
					if (rotated[x][y] != 0)
						next[x][y] = query(sum, i, j, s);
				}
			int[][] tmp = next;
			next = rotated;
			rotated = tmp;
			createSumTable(sum, rotated);
		}

		System.out.println(query(sum, lx, ly, s));
	}

	public static void main(String[] args) {
		Scanner in = new Scanner(System.in);

		int numTests = Integer.parseInt(in.nextLine());
		for (int i = 0; i < numTests; i++)
			solveProblem(in);

		in.close();
	}

}
