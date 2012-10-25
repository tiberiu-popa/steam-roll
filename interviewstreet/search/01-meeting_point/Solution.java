import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Scanner;

public class Solution {

	public static class Pair<K extends Comparable<K>, V extends Comparable<V>>
			implements Comparable<Pair<K, V>> {

		private final K key;
		private final V value;

		public Pair(K key, V value) {
			this.key = key;
			this.value = value;
		}

		public K getKey() {
			return key;
		}

		public V getValue() {
			return value;
		}

		public int compareTo(Pair<K, V> o) {
			int result = key.compareTo(o.key);
			if (result != 0)
				return result;
			else
				return value.compareTo(o.value);
		}

		public String toString() {
			return String.format("(%s, %s)", key, value);
		}

	}

	public static final void solveProblem(Scanner in) {
		int n = in.nextInt();
		int[] x = new int[n];
		int[] y = new int[n];

		for (int i = 0; i < n; i++) {
			x[i] = in.nextInt();
			y[i] = in.nextInt();
		}

		if (n <= 1) {
			System.out.println(0);
			return;
		}

		List<Pair<Long, Integer>> p = new ArrayList<Solution.Pair<Long, Integer>>();
		List<Pair<Long, Integer>> q = new ArrayList<Solution.Pair<Long, Integer>>();

		for (int i = 0; i < n; i++) {
			p.add(new Pair<Long, Integer>((long) (x[i] + y[i]), i));
			q.add(new Pair<Long, Integer>((long) (x[i] - y[i]), i));
		}

		Collections.sort(p);
		Collections.sort(q);

		List<Pair<Integer, Long>> u = new ArrayList<Solution.Pair<Integer, Long>>();
		List<Pair<Integer, Long>> v = new ArrayList<Solution.Pair<Integer, Long>>();

		long sum = 0;
		for (Pair<Long, Integer> elem : p)
			sum += elem.getKey();
		Pair<Long, Integer> crt = p.get(0);
		sum -= n * crt.getKey();
		u.add(new Pair<Integer, Long>(crt.getValue(), sum));
		for (int i = 1; i < n; i++) {
			Pair<Long, Integer> prev = crt;
			crt = p.get(i);
			sum += (n - 2 * i) * (prev.getKey() - crt.getKey());
			u.add(new Pair<Integer, Long>(crt.getValue(), sum));
		}

		sum = 0;
		for (Pair<Long, Integer> elem : q)
			sum += elem.getKey();
		crt = q.get(0);
		sum -= n * crt.getKey();
		v.add(new Pair<Integer, Long>(crt.getValue(), sum));
		for (int i = 1; i < n; i++) {
			Pair<Long, Integer> prev = crt;
			crt = q.get(i);
			sum += (n - 2 * i) * (prev.getKey() - crt.getKey());
			v.add(new Pair<Integer, Long>(crt.getValue(), sum));
		}

		Collections.sort(u);
		Collections.sort(v);

		List<Long> result = new ArrayList<Long>();
		for (int i = 0; i < n; i++)
			result.add(u.get(i).getValue() + v.get(i).getValue());
		System.out.println(Collections.min(result) / 2);
	}

	public static void main(String[] args) {
		Scanner in = new Scanner(System.in);

		solveProblem(in);

		in.close();
	}

}
