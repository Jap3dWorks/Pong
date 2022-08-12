//
// Created by Jordi on 8/12/2022.
//

#ifndef GL_TEST_REFLECTABLE_DEMO_H
#define GL_TEST_REFLECTABLE_DEMO_H

#include <boost/preprocessor.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/range_c.hpp>
#include <type_traits>
#include <iostream>
#include <algorithm>
#include <functional>

#define REM(...) __VA_ARGS__
#define EAT(...)

// Retrieve the type
#define TYPEOF(x) DETAIL_TYPEOF(DETAIL_TYPEOF_PROBE x,)
#define DETAIL_TYPEOF(...) DETAIL_TYPEOF_HEAD(__VA_ARGS__)
#define DETAIL_TYPEOF_HEAD(x, ...) REM x
#define DETAIL_TYPEOF_PROBE(...) (__VA_ARGS__),
// Strip off the type
#define STRIP(x) EAT x
// Show the type without parenthesis
#define PAIR(x) REM x


// A helper metafunction for adding const to a type
template<class M, class T>
struct make_const
{
    using type = T;
};

template<class M, class T>
struct make_const<const M, T>
{
    using type = typename std::add_const<T>::type;
};


#define REFLECTABLE(...) \
static const int fields_n = BOOST_PP_VARIADIC_SIZE(__VA_ARGS__); \
friend struct reflector; \
template<int N, class Self> \
struct field_data {}; \
BOOST_PP_SEQ_FOR_EACH_I(REFLECT_EACH, data, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))


#define REFLECT_EACH(r, data, i, x) \
PAIR(x); \
template<class Self> \
struct field_data<i, Self> \
{ \
    Self & self; \
    field_data(Self & self) : self(self) {} \
    \
    typename make_const<Self, TYPEOF(x)>::type & get() \
    { \
        return self.STRIP(x); \
    }\
    typename boost::add_const<TYPEOF(x)>::type & get() const \
    { \
        return self.STRIP(x); \
    }\
    const char * name() const \
    {\
        return BOOST_PP_STRINGIZE(STRIP(x)); \
    } \
}; \


struct reflector
{
    //Get field_data at index N
    template<int N, class T>
    static typename T::template field_data<N, T> get_field_data(T& x)
    {
        return typename T::template field_data<N, T>(x);
    }

    // Get the number of fields
    template<class T>
    struct fields
    {
        static const int n = T::fields_n;
    };
};


struct field_visitor
{
    template<class C, class Visitor, class I>
    void operator()(C& c, Visitor v, I)
    {
        v(reflector::get_field_data<I::value>(c));
    }
};


template<class C, class Visitor>
void visit_each(C & c, Visitor v)
{
    typedef boost::mpl::range_c<int,0,reflector::fields<C>::n> range;
    std::for_each<range>(std::bind<void>(field_visitor(), std::ref(c), v, std::placeholders::_1));
}


struct print_visitor
{
    template<class FieldData>
    void operator()(FieldData f)
    {
        std::cout << f.name() << "=" << f.get() << std::endl;
    }
};

template<class T>
void print_fields(T & x)
{
    visit_each(x, print_visitor());
}


/*
struct Person
{
    Person(const char *name, int age)
        :
        name(name),
        age(age)
    {
    }
private:
    REFLECTABLE
    (
        (const char *) name,
        (int) age
    )
};


int main()
{
    Person p("Tom", 82);
    print_fields(p);
    return 0;
}

 * */



#endif //GL_TEST_REFLECTABLE_DEMO_H
