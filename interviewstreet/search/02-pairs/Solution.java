import java.util.Arrays;
import java.util.Scanner;

public class Solution {

	public static final void solveProblem(Scanner in) {
		int n = in.nextInt();
		int k = in.nextInt();
		long[] x = new long[n];
		for (int i = 0; i < n; i++)
			x[i] = in.nextInt();

		if (n <= 1) {
			System.out.println(0);
			return;
		}

		Arrays.sort(x);

		long result = 0;
		int j = 0;
		for (int i = 0; i < n; i++) {
			long tmp = x[i] + k;
			for (; j < n; j++)
				if (x[j] >= tmp) {
					if (x[j] == tmp)
						result++;
					break;
				}
		}

		System.out.println(result);
	}

	public static void main(String[] args) {
		Scanner in = new Scanner(System.in);

		solveProblem(in);

		in.close();
	}

}
