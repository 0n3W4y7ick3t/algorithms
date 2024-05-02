#include <bitset>
#include <format>
#include <ios>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

struct Option {
  std::string name;
  short space;
  short value;
};

std::ostream &operator<<(std::ostream &os, const Option &opt) {
  return os << std::format("{:10} takes {:-2}, values {:-2}", opt.name + ":",
                           opt.space, opt.value);
}

std::istream &operator>>(std::istream &is, Option &opt) {
  std::string name;
  short size, value;
  is >> name >> size >> value;
  opt = {name, size, value};
  return is;
}

std::vector<Option> read_option(int option_num) {
  std::vector<Option> options;
  Option opt;
  for (int i = 0; i < option_num; i++) {
    std::cin >> opt;
    options.push_back(opt);
  }
  return std::move(options);
}

struct Result {
  std::bitset<128> choices;
  int value;
};

Result dp(const std::vector<Option> &options, const int V) {
  Result memo[options.size() + 1][V + 1];
  // o-1 is the o-th opt in options
  // s is the current pack_size
  Result smaller_pack, less_items;
  for (int o = 0; o <= options.size(); o++)
    for (int s = 0; s <= V; s++) {
      if (o == 0 || s == 0) {
        memo[o][s] = Result{{0}, 0};
      } else if (options[o - 1].space <= s) {
        // if it fits the current pack (size of s)
        less_items = memo[o - 1][s];
        /* use only old items to fill s */
        smaller_pack = memo[o - 1][s - options[o - 1].space];
        /* sub problem (s - current item size) + current item value */
        if (smaller_pack.value + options[o - 1].value > less_items.value) {
          memo[o][s] = {smaller_pack.choices.set(o - 1),
                        smaller_pack.value + options[o - 1].value};
        } else {
          memo[o][s] = less_items;
        }
      } else { // cant fit
        memo[o][s] = memo[o - 1][s];
      }
    }
  return memo[options.size()][V];
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int pack_size, option_num = 0;
  std::cin >> pack_size >> option_num;
  auto options = read_option(option_num);
  auto result = dp(options, pack_size);
  int total_value = 0, total_space = 0;
  for (int i = 0; i < result.choices.size(); i++) {
    if (result.choices.test(i)) {
      std::cout << "chose " << options[i] << std::endl;
      total_space += options[i].space;
      total_value += options[i].value;
    }
  }
  std::cout << std::format("total space: {}, total value: {}\n", total_space,
                           total_value);
}
