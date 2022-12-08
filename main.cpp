#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <sstream>
#include <array>
#include <iterator>


template<typename T>
std::ostream& operator<<(std::ostream& strm, std::pair<T, T> p) {
    strm << '[' << p.first << ',' << p.second << ']';
    return strm;
}

template<typename T>
class Converter{
private:
    std::string value;
public:
    std::string get_string(){
        return value;
    }
    Converter() : value(std::string()) {};
    ~Converter() = default;
    explicit Converter(const T& _array);
    explicit Converter(T&& _array);
    Converter(const Converter& other);
    Converter(Converter&& other);
    Converter& operator=(const Converter& other);
    Converter& operator=(Converter&& other);
};

template<typename T>
Converter<T>::Converter(const T &_array) {
    // copy constructor for container
    value = std::string();
    if(!_array.empty()){
        value = std::string();
        std::ostringstream strs;
        std::size_t i = 0;
        for (auto it = _array.begin(); it != _array.end(); ++it, ++i){
            strs << *it;
            value += strs.str() + (i == _array.size() - 1 ? "\0" : ", ");
            strs.str("");
        }
    }
}

template<typename T>
Converter<T>::Converter(T &&_array) {
    // move constructor for container
    value = std::string();
    if(!_array.empty()){
        value = std::string();
        std::ostringstream strs;
        std::size_t i = 0;
        for (auto it = _array.begin(); it != _array.end(); ++it, ++i){
            strs << *it;
            value += strs.str() + (i == _array.size() - 1 ? "\0" : ", ");
            strs.str("");
        }
        //_array.clear();  - works for vector and deque, but not for std::array
        std::fill( std::begin( _array ), std::end( _array ), 0 ); // - works for all containers
                                                                              // but don't change size
    }
}

template<typename T>
Converter<T> &Converter<T>::operator=(const Converter &other) {
    // copy assignment operator
    Converter<T> tmp(other);
    std::swap(this->value, tmp.value);
    return *this;
}

template<typename T>
Converter<T> &Converter<T>::operator=(Converter &&other) {
    // move assignment operator
    std::swap(this->value, other.value);
    std::fill( std::begin( other.value ), std::end( other.value ), 0 );
    return *this;
}

template<typename T>
Converter<T>::Converter(const Converter &other) {
    // copy constructor for converter
    this->value = other.value;
}

template<typename T>
Converter<T>::Converter(Converter &&other) {
    // move assignment operator
    std::swap(this->value, other.value);
    std::fill( std::begin( other.value ), std::end( other.value ), 0 );
}


int main() {
    std::vector<int> v = {1, 2, 3, 45, 5};
    Converter<std::vector<int>> c_lvalue_vector(v);
    std::cout << "create converter using vector as lvalue: " << c_lvalue_vector.get_string() << std::endl;
    std::cout << "it's size didn't change: " << v.size() << std::endl;
    Converter<std::vector<int>> c_rvalue_vector(std::move(v));
    std::cout << "create converter using same vector but as rvalue: " << c_rvalue_vector.get_string() << std::endl;
    std::cout << "it's size didn't change: " << v.size() << std::endl;
    Converter<std::vector<int>> c_rvalue_vector_after_move(v);
    std::cout << "but vector after being moved and being used as rvalue: "\
                << c_rvalue_vector_after_move.get_string() << std::endl;
    std::cout << "it's size still didn't change: " << v.size() << std::endl;
    std::array<double, 5> a = {0.2, 0, 12.3, 4, -5};
    Converter<std::array<double, 5>> c_array = Converter<std::array<double, 5>>(a);
    std::cout << "creating converter using copy assignment operator and array with doubles: " \
                                                                            << c_array.get_string() << std::endl;
    Converter<std::array<double, 5>> c_array_rvalue = (std::move(c_array));
    std::cout << "creating converter using move assignment operator and previous container: " \
                                                                            << c_array_rvalue.get_string() << std::endl;
    std::cout << "meanwhile, previous container now has: \'" << c_array.get_string()  << '\'' << std::endl;
    std::deque<std::pair<int, int>> d = {{1, 2}, {-1, -1}, {0, 0}};
    Converter<std::deque<std::pair<int, int>>> c_deque_of_pair(d);
    std::cout << "deque of pairs convert into(using overridden for pair operator '<<'): " \
                                                                           << c_deque_of_pair.get_string() << std::endl;
    Converter<std::deque<std::pair<int, int>>> c_deque_of_pair_lvalue(c_deque_of_pair);
    std::cout << "creating converter using copy constructor and first deque converter as lvalue: " \
                                                                    << c_deque_of_pair_lvalue.get_string() << std::endl;
    Converter<std::deque<std::pair<int, int>>> c_deque_of_pair_rvalue(std::move(c_deque_of_pair));
    std::cout << "creating converter using move constructor and first deque converter as rvalue: " \
                                                                    << c_deque_of_pair_rvalue.get_string() << std::endl;
    std::cout << "meanwhile data in first deque converter after using it as a rvalue looks like: \'" \
                                                                   << c_deque_of_pair.get_string() << '\'' << std::endl;
    return 0;
}
