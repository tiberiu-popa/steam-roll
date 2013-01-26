/*
 Enter your code here. Read input from STDIN. Print output to STDOUT
 Your class should be named Solution
*/
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.IOException;

import java.math.BigInteger;

import java.util.ArrayList;
import java.util.List;

public class Solution {

	public static void main(String[] args) throws IOException {
		BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
		String line = br.readLine();
		int numTests = Integer.parseInt(line);
		for (int i = 0; i < numTests; i++) {
			line = br.readLine();
			String[] tokens = line.split(" ");
			BigInteger n = new BigInteger(tokens[0]);
			BigInteger p = new BigInteger(tokens[1]);
			List<BigInteger> digits = new ArrayList<BigInteger>();
			BigInteger tmp = n;
			while (!BigInteger.ZERO.equals(tmp)) {
				BigInteger[] qr = tmp.divideAndRemainder(p);
				digits.add(qr[1]);
				tmp = qr[0];
			}
			BigInteger bad = BigInteger.ONE;
			for (BigInteger digit : digits)
				bad = bad.multiply(digit.add(BigInteger.ONE));
			System.out.println(n.add(BigInteger.ONE).subtract(bad));
		}
	}

}
