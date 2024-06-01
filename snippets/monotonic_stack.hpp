#include <functional>
#include <initializer_list>
#include <stack>
#include <vector>

template <typename T> class MonoStack {
  using Compare = std::function<bool(T, T)>;

public:
  MonoStack(Compare cmp) : cmp{cmp} {}

  MonoStack(Compare cmp, std::initializer_list<T> inits) : cmp{cmp} {
    push(inits);
  }

  size_t size() { return _stack.size(); }

  T& top() { return _stack.top(); }

  void pop() { _stack.pop(); }

  void push(T item) {
    std::stack<T> _aux;
    while (size()) {
      T t = top();
      if (!cmp(t, item)) {
        _aux.push(t);
        _stack.pop();
      } else
        break;
    }
    _stack.push(item);
    while (!_aux.empty()) {
      _stack.push(_aux.top());
      _aux.pop();
    }
  }

  void push(std::vector<T> items) {
    for (auto item : items)
      push(item);
  }

private:
  Compare cmp;
  std::stack<T> _stack;
};
