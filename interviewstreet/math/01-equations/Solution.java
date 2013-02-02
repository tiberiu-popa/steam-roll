import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class Solution {

	public static final long MODULUS = 1000007;

	public static void main(String[] args) {
		Scanner in = new Scanner(System.in);
		try {
			in = new Scanner(new File("input00.txt"));
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}

		int n = Integer.parseInt(in.nextLine());
		List<Integer> primes = new ArrayList<Integer>();
		primes.add(2);
		for (int i = 3; i <= n; i += 2) {
			boolean isPrime = true;
			for (Integer prime : primes) {
				if (prime * prime > n)
					break;
				if (i % prime == 0) {
					isPrime = false;
					break;
				}
			}
			if (isPrime)
				primes.add(i);
		}

		List<Integer> exponents = new ArrayList<Integer>();
		for (Integer prime : primes) {
			int exp = 0;
			// i *= prime can exceed an int
			for (long i = prime; i <= n; i *= prime) {
				exp += n / i;
				exp %= MODULUS;
			}
			exponents.add(exp);
		}

		// Note that x is part of a pair such that 1/x+1/y=1/c iff (x-c)|c^2
		long numDivisors = 1;
		for (Integer exponent : exponents) {
			numDivisors *= (2 * exponent + 1);
			numDivisors %= MODULUS;
		}

		System.out.println(numDivisors);

		in.close();
	}

}
