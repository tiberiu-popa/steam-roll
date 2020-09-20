#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

using namespace std;

class Solution {
public:
  bool containsNearbyAlmostDuplicate(vector<int>& nums, int k, int t) {
    set<long long> used;
    vector<int>::size_type k_sz = k;
    for (vector<int>::size_type i = 0; i < nums.size(); i++) {
      long long x = nums[i];
      auto lit = used.lower_bound(x - t);
      if (lit != used.end() && *lit <= x + t) {
          return true;
      }
      used.insert(x);
      if (i >= k_sz) {
        used.erase(nums[i - k]);
      }
    }
    return false;
  }

  int lengthOfLongestSubstring(string s) {
    int last[256], maxlen = 0, start = 0;
    for (int i = 0; i < 256; i++) {
      last[i] = -1;
    }
    for (string::size_type i = 0; i < s.size(); i++) {
      int ch = s[i];
      if (last[ch] >= 0) {
        int diff = i - start;
        if (diff > maxlen) {
          maxlen = diff;
        }
        int after = last[ch] + 1;
        if (after > start) {
          start = after;
        }
      }
      last[ch] = i;
    }
    int cand = s.size() - start;
    return cand > maxlen ? cand : maxlen;
  }

  string longestPalindrome(string s) {
    pair<int, int> result(-1, -1);
    int sz = (int) s.size();
    for (int i = 0; i < sz; i++) {
      int j, cand;
      for (j = 1; i - j >= 0 && i + j < sz; j++) {
        if (s[i - j] != s[i + j]) {
          break;
        }
      }
      cand = 2 * j - 1;
      if (cand > result.first) {
        result = make_pair(cand, 2 * i);
      }
    }
    for (int i = 1; i < sz; i++) {
      int j, cand;
      for (j = 0; i - 1 - j >= 0 && i + j < sz; j++) {
        if (s[i - 1 - j] != s[i + j]) {
          break;
        }
      }
      cand = 2 * j;
      if (cand > result.first) {
        result = make_pair(cand, 2 * i + 1);
      }
    }
    int x = result.second / 2;
    int y = result.first / 2;
    if (result.second % 2 == 0) {
      return string(s.begin() + x - y, s.begin() + x + y + 1);
    } else {
      return string(s.begin() + x - y, s.begin() + x + y);
    }
  }

  bool isMatch(string s, string p) {
    vector<vector<int>> dp;
    dp.push_back(vector<int>());
    dp[0].push_back(1);
    for (string::size_type i = 1; i <= s.size(); i++) {
      dp[0].push_back(0);
    }
    dp.push_back(dp[0]);
    string::size_type j = 0;
    for (string::size_type k = 0; k < p.size(); k++, j++) {
      char ch = p[k];
      bool is_optional = k + 1 < p.size() && p[k + 1] == '*';
      int crt = (j + 1) % 2;
      int prev = j % 2;
      if (is_optional) {
        dp[crt][0] = dp[prev][0];
      } else {
        dp[crt][0] = 0;
      }
      for (string::size_type i = 0; i < s.size(); i++) {
        int val = 0;
        if (s[i] == ch || ch == '.') {
          val = dp[prev][i] || (is_optional && dp[crt][i]);
        }
        if (is_optional && !val) {
          val = dp[prev][i + 1];
        }
        dp[crt][i + 1] = val;
      }
      if (is_optional) {
        ++k;
      }
    }
    return dp[j % 2][s.size()];
  }

  int maxAreaSlow(vector<int>& height) {
    int max_area = 0;
    int sz = (int) height.size();
    vector<pair<int, int>> v;
    for (int i = 0; i < sz; i++) {
      v.push_back(make_pair(height[i], i));
    }
    sort(v.rbegin(), v.rend());
    int left = sz, right = -1;
    bool first = true;
    for (auto x : v) {
      if (first) {
        first = false;
      } else {
        int ld = labs(x.second - left);
        int rd = labs(x.second - right);
        int cand = max(ld, rd) * x.first;
        if (cand > max_area) {
            max_area = cand;
        }
      }
      if (x.second < left) {
        left = x.second;
      }
      if (x.second > right) {
        right = x.second;
      }
    }
    return max_area;
  }

  int maxArea(vector<int>& height) {
    int max_area = 0;
    int sz = (int) height.size();
    int i = 0, j = sz - 1;
    while (i < j) {
      int d = j - i;
      int x;
      if (height[i] < height[j]) {
        x = height[i];
        while (++i < sz && height[i] <= x);
      } else {
        x = height[j];
        while (--j >= 0 && height[j] <= x);
      }
      int cand = d * x;
      if (cand > max_area) {
        max_area = cand;
      }
    }
    return max_area;
  }

  vector<vector<int>> threeSum(vector<int>& nums) {
    vector<vector<int>> solutions;
    int sz = (int) nums.size();
    sort(nums.begin(), nums.end());
    for (int i = 0; i < sz; i++) {
      if (i >= 1 && nums[i] == nums[i - 1]) {
        continue;
      }
      int j = i + 1;
      int k = sz - 1;
      while (j < k) {
        int left = nums[j], right = nums[k];
        int sum = nums[i] + left + right;
        if (sum < 0) {
          j++;
        } else if (sum > 0) {
          k--;
        } else {
          solutions.push_back(vector<int>{nums[i], left, right});
          while (++j < sz && nums[j] == left);
          while (--k >= 0 && nums[k] == right);
        }
      }
    }
    return solutions;
  }
};

int main()
{
  Solution soln;

  vector<int> nums { 1, 2 };
  bool res1 = soln.containsNearbyAlmostDuplicate(nums, 0, 1);
  cout << "containsNearbyAlmostDuplicate: " << res1 << endl;

  int res2 = soln.lengthOfLongestSubstring("abcabcbb");
  cout << "lengthOfLongestSubstring: " << res2 << endl;

  string res3 = soln.longestPalindrome("cbbd");
  cout << "longestPalindrome: " << res3 << endl;

  bool res4 = soln.isMatch("aab", "c*a*b");
  cout << "isMatch: " << res4 << endl;

  vector<int> height {1, 8, 6, 2, 5, 4, 8, 3, 7};
  int res5 = soln.maxAreaSlow(height);
  cout << "maxAreaSlow: " << res5 << endl;
  int res6 = soln.maxArea(height);
  cout << "maxArea: " << res6 << endl;

  vector<int> nums7 {-1, 0, 1, 2, -1, -4};
  vector<vector<int>> res7 = soln.threeSum(nums7);
  cout << "threeSum:" << endl;
  for (auto &v : res7) {
    for (auto x : v) {
      cout << ' ' << x;
    }
    cout << endl;
  }

  return 0;
}
