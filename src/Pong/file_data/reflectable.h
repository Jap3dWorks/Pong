//
// Created by Jordi on 8/12/2022.
//

#ifndef GL_TEST_REFLECTABLE_H
#define GL_TEST_REFLECTABLE_H

#include "Pong/file_data/p_range.h"
#include <boost/preprocessor.hpp>
#include <boost/mpl/at.hpp>
#include <type_traits>
#include <iostream>
#include <algorithm>
#include <functional>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/bind.hpp>


#define REM(...) __VA_ARGS__
#define EAT(...)

#define TYPEOF(x) DETAIL_TYPEOF(DETAIL_TYPEOF_PROBE x,)  // x-> (type)a, contains the required(), so preprocessor can be executed
#define DETAIL_TYPEOF(...) DETAIL_TYPEOF_HEAD(__VA_ARGS__)
#define DETAIL_TYPEOF_HEAD(x, ...) REM x
#define DETAIL_TYPEOF_PROBE(...) (__VA_ARGS__),

#define STRIP(x) EAT x
#define PAIR(x) REM x

#define EXPOSE(...) (__VA_ARGS__)

#define DECLARE(type, name, ...) type name{__VA_ARGS__}
#define TYPE(type, name, ...) type
#define NAME(type, name, ...) name
#define NAME_STR(type, name, ...) #name


template<typename M, typename T>
struct make_const {
    using type=T;
};

template<typename M, typename T>
struct make_const<const M, T> {
    using type = typename std::add_const<T>::type;
};

#define REFLECTABLE(...) \
static const int fields_n = BOOST_PP_VARIADIC_SIZE(__VA_ARGS__); \
friend struct reflector; \
template<int N, typename Self> \
struct field_data{};     \
BOOST_PP_SEQ_FOR_EACH_I(REFLECT_EACH, data, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))


#define REFLECT_EACH(r, data, i, x)                      \
DECLARE x;                                               \
template<typename Self>                                  \
struct field_data<i, Self>                               \
{                                                        \
Self & self;                                             \
explicit field_data(Self & self):self(self) {}           \
                                                         \
typename make_const<Self, TYPE x>::type & get()          \
{                                                        \
    return self.NAME x;                                  \
}                                                        \
                                                         \
typename std::add_const<TYPE x>::type & get() const      \
{                                                        \
    return self.NAME x;                                  \
}                                                        \
const char* name() const                                 \
{                                                        \
    return NAME_STR x;                                   \
}                                                        \
};


struct reflector {
    template<int N, typename T>
    static constexpr typename T::template field_data<N, T> get_field_data(T &x) {
        return typename T::template field_data<N, T>(x);
    }

    template<typename T>
    struct fields {
        static const int n = T::fields_n;
    };
};


template<typename C, typename Visitor, class I>
inline static constexpr void field_visitor(C& c, Visitor v, I) {
    v(reflector::get_field_data<I::value, C>(c));
}


template<typename C, typename Visitor>
void visit_each(C &c, Visitor v) {
    typedef boost::mpl::range_c<int, 0, reflector::fields<C>::n> range;

    boost::mpl::for_each<range>(
            [&]<typename I>(I i) constexpr -> void {
                field_visitor<C, Visitor, I>(
                        c, v, i
                );
            }
        );
}


struct print_visitor {
        template<typename FieldData>
        void operator()(FieldData f) {
            std::cout << f.name() << "=" << f.get() << std::endl;
        }
    };


template<typename T>
void print_fields(T& x) {
    visit_each(x, print_visitor());
}

#endif //GL_TEST_REFLECTABLE_H
