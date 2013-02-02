import java.security.InvalidParameterException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Scanner;

public class Solution {

	public static final int MODULUS = 1000000007;

	private static class Matrix {

		private int[][] arr;

		public Matrix(int n) {
			this.arr = new int[n][n];
		}

		public Matrix multiply(Matrix other) {
			int n = arr.length;
			if (n == other.arr.length) {
				Matrix product = new Matrix(n);
				for (int i = 0; i < n; i++)
					for (int j = 0; j < n; j++)
						for (int k = 0; k < n; k++) {
							long tmp = arr[i][k];
							tmp = (tmp * other.arr[k][j]) % MODULUS;
							product.arr[i][j] += tmp;
							product.arr[i][j] %= MODULUS;
						}
				return product;
			} else {
				throw new InvalidParameterException();
			}
		}

		public int[] mulvec(Integer[] vec) {
			int n = arr.length;
			if (n == vec.length) {
				int[] result = new int[n];
				for (int i = 0; i < n; i++)
					for (int k = 0; k < n; k++) {
						long tmp = arr[i][k];
						tmp = (tmp * vec[k]) % MODULUS;
						result[i] += tmp;
						result[i] %= MODULUS;
					}
				return result;
			} else {
				throw new InvalidParameterException();
			}
		}

		public Matrix pow(int e) {
			Matrix result = createIdentity(arr.length);
			Matrix crt = this;
			if (e > 0) {
				while (true) {
					if ((e & 1) != 0)
						result = result.multiply(crt);
					e >>= 1;
					if (e == 0)
						break;
					crt = crt.multiply(crt);
				}
			}
			return result;
		}

		public String toString() {
			return Arrays.deepToString(arr);
		}

		public static Matrix createIdentity(int n) {
			Matrix matrix = new Matrix(n);
			for (int i = 0; i < n; i++)
				matrix.arr[i][i] = 1;
			return matrix;
		}

		public static Matrix createForwardMatrix(int n, int k) {
			Matrix matrix = createIdentity(n);
			for (int i = -1; i <= 1; i += 2) {
				int j = k + i;
				if (j < 0)
					j += n;
				else if (j >= n)
					j -= n;
				matrix.arr[k][j] = 1;
			}
			return matrix;
		}

		public static Matrix createBackwardMatrix(int n, int k) {
			Matrix matrix = createIdentity(n);
			for (int i = -1; i <= 1; i += 2) {
				int j = k + i;
				if (j < 0)
					j += n;
				else if (j >= n)
					j -= n;
				// This is actually "-1", but we are doing matrix operations mod
				matrix.arr[k][j] = MODULUS - 1;
			}
			return matrix;
		}

	}

	public static final void printResult(int[] result) {
		for (int i = 0; i < result.length; i++)
			if (i < result.length - 1)
				System.out.print(result[i] + " ");
			else
				System.out.println(result[i]);
	}

	public static final void solveProblem(Scanner in) {
		int n = in.nextInt();
		int m = in.nextInt();
		List<Integer> x = new ArrayList<Integer>();
		for (int i = 0; i < n; i++)
			x.add(in.nextInt());

		List<Matrix> forwardMatrices = new ArrayList<Solution.Matrix>();
		List<Matrix> backwardMatrices = new ArrayList<Solution.Matrix>();
		for (int i = 0; i < n; i++) {
			forwardMatrices.add(Matrix.createForwardMatrix(n, i));
			backwardMatrices.add(Matrix.createBackwardMatrix(n, i));
		}

		Matrix base = Matrix.createIdentity(n);
		List<Matrix> first = new ArrayList<Solution.Matrix>();
		first.add(base);
		for (int i = 0; i < n; i++) {
			base = forwardMatrices.get(i).multiply(base);
			first.add(base);
		}

		int g = m / n;
		Matrix cool = base.pow(g);
		int p = m % n;
		int q = 0;
		cool = first.get(p).multiply(cool);

		Integer[] xarr = x.toArray(new Integer[0]);
		printResult(cool.mulvec(xarr));
		for (int i = 1; i < n; i++) {
			cool = cool.multiply(backwardMatrices.get(q));
			cool = forwardMatrices.get(p).multiply(cool);
			printResult(cool.mulvec(xarr));
			p++;
			if (p == n)
				p = 0;
			q++;
		}

		System.out.println();
	}

	public static void main(String[] args) {
		Scanner in = new Scanner(System.in);

		int numTests = in.nextInt();
		for (int i = 0; i < numTests; i++)
			solveProblem(in);

		in.close();
	}

}
