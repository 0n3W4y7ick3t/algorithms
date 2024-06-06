/**
 * @author leon
 * @date Jun 06, 2024 at 20:18:08
 * @tag
 * @problem SPOJ PRO - Promotion
 * @link https://www.spoj.com/problems/PRO
 * @result WA, passed local test, should be something strange,
 * since this answer used the std, that can't be wrong.
 * using cpp multiset
 */
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <set>

using std::cin;
using std::multiset;

int main(void) {
  size_t n, k, cost;
  long long result = 0;
  multiset<size_t, std::less<size_t>> min_heap;
  multiset<size_t, std::greater<size_t>> max_heap;
  cin >> n;
  for (size_t i = 0; i < n; ++i) {
    cin >> k;
    for (size_t j = 0; j < k; ++j) {
      cin >> cost;
      min_heap.emplace(cost);
      max_heap.emplace(cost);
    }
    result += *max_heap.begin() - *min_heap.begin();
    max_heap.erase(max_heap.begin());
    min_heap.erase(min_heap.begin());
  }
  std::cout << result << std::endl;
  return EXIT_SUCCESS;
}
