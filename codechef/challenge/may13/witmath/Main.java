import java.math.*;
import java.util.*;

public class Main {

	public static final BigInteger TWO = BigInteger.valueOf(2);
	public static final BigInteger THREE = BigInteger.valueOf(3);

	public static BigInteger mulmod(BigInteger x, BigInteger y, BigInteger p) {
		return x.multiply(y).remainder(p);
	}

	public static BigInteger powmod(BigInteger x, long e, BigInteger p) {
		BigInteger result = BigInteger.ONE;
		BigInteger base = x;

		if (e > 0) {
			while (true) {
				if ((e & 1) != 0) {
					result = mulmod(result, base, p);
				}
				e >>= 1;
				if (e <= 0)
					break;
				base = mulmod(base, base, p);
			}
		}

		return result;
	}

	public static boolean millerRabin(BigInteger n) {
		int numWitnesses = 0;
		long sn = n.longValue();
		long m = sn - 1;
		int s = 0;
		while ((m & 1) == 0) {
			m >>= 1;
			s++;
		}
		for (int i = 2; i <= 31; i++) {
			BigInteger a = BigInteger.valueOf(i);
			if (i >= sn)
				break;
			if (a.gcd(n).equals(BigInteger.ONE)) {
				BigInteger x = powmod(a, m, n);
				boolean composite = !BigInteger.ONE.equals(x) && !n.equals(x.add(BigInteger.ONE));
				if (composite) {
					for (int r = 1; r < s; r++) {
						x = mulmod(x, x, n);
						composite = !n.equals(x.add(BigInteger.ONE));
						if (!composite)
							break;
					}
				}
				if (composite) {
					++numWitnesses;
					return false;
				}
			} else {
				return false;
			}
		}
		return true;
	}

	public static void solveProblem(Scanner in) {
		long sn = in.nextLong();
		if (sn == 2) {
			System.out.println(sn);
		} else {
			if (sn % 2 == 0)
				--sn;
			BigInteger n = BigInteger.valueOf(sn);
			while (true) {
				if (sn % 3 == 0) {
					if (sn == 3)
						break;
				} else {
					if (millerRabin(n))
						break;
				}
				sn -= 2;
				n = n.subtract(TWO);
			}
			System.out.println(sn);
		}
	}

	public static void main(String[] args) {
		Scanner in = new Scanner(System.in);
		int numTests = in.nextInt();
		for (int i = 0; i < numTests; i++)
			solveProblem(in);
	}

}
