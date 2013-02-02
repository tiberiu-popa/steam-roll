import java.util.ArrayList;
import java.util.List;
import java.util.Random;
import java.util.Scanner;

public class Solution {

	private static final Random random = new Random();

	public static class Pair {

		private int key, value;

		public Pair(int key, int value) {
			this.key = key;
			this.value = value;
		}

		public int getKey() {
			return key;
		}

		public int getValue() {
			return value;
		}

	}

	public static class Node {

		private int parent, rank;

		public Node(int parent, int rank) {
			this.parent = parent;
			this.rank = rank;
		}

		public int getParent() {
			return parent;
		}

		public int getRank() {
			return rank;
		}

		public void setParent(int parent) {
			this.parent = parent;
		}

		public void incrementRank() {
			this.rank++;
		}

	}

	public static int find(Node[] ds, int x) {
		List<Integer> positions = new ArrayList<Integer>();
		while (true) {
			int parent = ds[x].getParent();
			if (parent == x) {
				break;
			} else {
				positions.add(x);
				x = parent;
			}
		}
		for (Integer pos : positions)
			ds[pos].setParent(x);
		return x;
	}

	public static int union(Node[] ds, int x, int y) {
		int u = find(ds, x);
		int v = find(ds, y);
		if (u == v)
			return 0;

		int p = ds[u].getRank();
		int q = ds[v].getRank();
		if (p < q) {
			ds[u].setParent(v);
		} else {
			ds[v].setParent(u);
			if (p == q)
				ds[u].incrementRank();
		}
		return 1;
	}

	public static int simulate(List<Pair> edges, int n) {
		Node[] ds = new Node[n];
		for (int i = 0; i < n; i++)
			ds[i] = new Node(i, 0);

		int numComponents = 0;
		int m = edges.size();
		for (int i = 0; i < m; i++) {
			int j = i + random.nextInt(m - i);
			Pair tmp = edges.get(j);
			edges.set(j, edges.get(i));
			edges.set(i, tmp);
			numComponents += union(ds, tmp.getKey(), tmp.getValue());
			if (numComponents == n - 1)
				return i + 1;
		}
		return 0;
	}

	public static final void main(String[] args) {
		Scanner in = new Scanner(System.in);
		int n = in.nextInt();
		List<Pair> edges = new ArrayList<Pair>();
		for (int i = 0; i < n; i++)
			for (int j = i + 1; j < n; j++)
				edges.add(new Pair(i, j));

		int numSimulations = 1000;
		int sum = 0;
		for (int i = 0; i < numSimulations; i++)
			sum += simulate(edges, n);
		// System.out.println((float) sum / numSimulations);
		System.out.println(sum / numSimulations);
	}

}
