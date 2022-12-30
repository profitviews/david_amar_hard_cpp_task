#include <type_traits>
#include <tuple>
#include <cstddef>
#include <iostream>

template <typename T>
class Callable;

template <typename R, typename... Args>
class Callable<R(Args...)> {
public:
    Callable(R(*func)(Args...)) : func_(func), isFunction_(true) {}

    template <typename T, typename U>
    Callable(T&& obj, U&& method) : obj_(std::forward<T>(obj)), method_(std::forward<U>(method)), isFunction_(false) {}

    template <typename T>
    Callable(T&& obj) : obj_(std::forward<T>(obj)), isFunction_(false) {}

    R operator()(Args... args) {
        if (isFunction_) {
            return func_(std::forward<Args>(args)...);
        } else {
            return std::invoke(method_, obj_, std::forward<Args>(args)...);
        }
    }

private:
    union {
        R(*func_)(Args...);
        struct {
            std::decay_t<decltype(obj_)> obj_;
            std::decay_t<decltype(method_)> method_;
        };
    };
    bool isFunction_;
};

int Foo()
{
    return ( 8 );
}

Callable< int ()> callableWithFunctionPointer( &Foo );

int returnValue = callableWithFunctionPointer();

class Boo
{
public:
    void Moo(const int kk)
    {
        std::cout << kk << std::endl;
    }
    void operator() ()
    {
        std::cout << "Hello" << std::endl;
    }
};

int main() {

    Boo instance;

    Callable< void (const int ) > callableWithMethodPointer( &Boo::Moo, instance ); 
    callableWithMethodPointer( 89 );

    Callable< void () > callableWithFunctor( instance ); 
    callableWithFunctor();

    Callable< int (const int ) > callableWithLambdaExpression([](const int vv) { return (vv + 34); });

    int returnValue_1 = callableWithLambdaExpression(45);

}
