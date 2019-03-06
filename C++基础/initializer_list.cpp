#include <iostream>
#include <vector>
#include <initializer_list>

using namespace std;

template <class T>
struct S{
    vector<T> v;
    S(initializer_list<T> l) :v(l){
        cout <<"constructed with a" << l.size() << "-element list\n";
    }
    void append(initializer_list<T> l){
        v.insert(v.end(), l.begin(), l.end());
    }
    pair<const T*, size_t> c_arr() const{
        return {&v[0], v.size()};
    }
};

template <typename T>
void templated_fn(T){}

int main(){

    S<int> s = {1, 2, 3, 4, 5};//复制初始化
    s.append({6, 7, 8});//函数调用中的列表初始化

    cout << "The vector size is now " << s.c_arr().second << " ints:\n";

    for(auto n : s.v)
        cout << n << ' ';
    cout << '\n';

    auto a1 = {10, 11, 12};//auto的特殊规则

    cout << "The list bound to auto has size() = " << a1.size() << '\n';

//    templated_fn({1, 2, 3});//编译错误！{1, 2, 3}不是表达式, 它无类型，故T无法推导

    templated_fn<initializer_list<int> >({1, 2, 3});//OK
    templated_fn<vector<int> >({1, 2, 3});//也OK

    return 0;
}
