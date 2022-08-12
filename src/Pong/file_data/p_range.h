//
// Created by Jordi on 8/12/2022.
//

#ifndef GL_TEST_P_RANGE_H
#define GL_TEST_P_RANGE_H


template<typename T, typename U, U  value_, typename V, V ...values_>
struct op_value: op_value<T, U, value_, bool>, op_value<T, V, values_...>
{};

template<typename T, T value_>
struct op_value<T, T, value_, std::nullopt_t> {
    using type=T;
    static constexpr T value{value_};
    static constexpr bool has_value{true};
};

template<typename T>
struct op_value<T, std::nullopt_t, std::nullopt, std::nullopt_t> {
    using type=T;
    static constexpr bool has_value{false};
};


#define VAREXPR(x) VAREXPR_FRONT x
#define VAREXPR_FRONT(...) __VA_ARGS__,
#define VAREEXPR_VALUE(a, b) a,


template<typename T>
struct p_range_it {
    using it_type = p_range_it<T>;
    using const_type = const p_range_it<T>;
    using reference = it_type&;
    using const_ref = const it_type&;
    using value_ref = T&;
    using const_val_ref = const T&;

    T value;
    explicit p_range_it(T value_): value(value_) {}

    bool operator==(const_ref other) const {
        return value == other.value;
    }

    bool operator!=(const_ref other) const {
        return value != other.value;
    }

    reference operator++() noexcept {
        ++value;
        return *this;
    }

    it_type operator++(int) {
        auto temp = *this;
        ++value;
        return temp;
    }

    value_ref operator*() {
        return value;
    }

    const_val_ref operator*() const {
        return value;
    }

};

template<typename T, T start_, T final_>
struct p_range {
    static constexpr T start{start_};
    static constexpr T final{final_};

    static constexpr p_range_it<T> begin() {
        return p_range_it<T>(start_);
    }

    static constexpr const p_range_it<T> end() {
        return p_range_it<T>(final_);
    }
};

#endif //GL_TEST_P_RANGE_H
