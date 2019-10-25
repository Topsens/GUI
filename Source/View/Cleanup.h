#include <functional>

class _cleanup
{
public:
    _cleanup(const std::function<void()>& func) : func(func) {}
   ~_cleanup() { if (this->func) this->func(); }
    std::function<void()> func;
};
#define ONCLEANUP(X, F)  _cleanup _cleanup_##X(F)
#define RESETCLEANUP(X, F) _cleanup_##X.func = F

// Usage:
//   when 'a' is out of code scope, clean up lambda function will be called to delete 'a'
//
//  {
//      int a = new int[10];
//      ONCLEANUP(a, [&]{ delete[] a; });
//  } // 'a' is deleted here