import java.util.Arrays;
import java.util.Scanner;

public class Solution {

	public static final void solveProblem(Scanner in) {
		int n = in.nextInt();
		int k = in.nextInt();

		int[] x = new int[n];
		for (int i = 0; i < n; i++)
			x[i] = in.nextInt();

		Arrays.sort(x);
		for (int i = 0; i < n / 2; i++) {
			int tmp = x[i];
			x[i] = x[n - 1 - i];
			x[n - 1 - i] = tmp;
		}

		int sum = 0;
		for (int i = 0; i < n; i++)
			sum += (1 + i / k) * x[i];

		System.out.println(sum);
	}

	public static void main(String[] args) {
		Scanner in = new Scanner(System.in);

		solveProblem(in);

		in.close();
	}

}
