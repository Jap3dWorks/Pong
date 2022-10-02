//
// Created by Jordi on 8/4/2022.
//

#ifndef PONG_SRC_PONG_REGISTERS_SPARSE_SET_H_
#define PONG_SRC_PONG_REGISTERS_SPARSE_SET_H_

#include "Pong/registers/reg_id.h"
#include "Utils/type_conditions.h"
#include <typeinfo>
#include <iostream>
#include <vector>
#include <exception>
#include <optional>
#include <type_traits>
#include <algorithm>

// https://www.geeksforgeeks.org/sparse-set/

namespace pong {

    class SSException : public std::exception {
    public:
        using message_type = const char *;
    private:
        message_type what_;
    public:
        explicit SSException(message_type what) : what_(what) {}

        message_type what() noexcept { return what_; }
    };


    template<typename T>
    struct SSetPosData {
        T sparse_pos;
        T dense_pos;
    };


    class SSIndexError : public SSException {
        using SSException::SSException;
    };


    class SparseSet_ {
    };


    /**
     * The "id" of the object is the position in th sparse vector.
     * */
    template<typename Type_>
    class SparseSet : public SparseSet_ {
    public:
        using denset_set = std::vector<Type_>;

        using size_type = size_t;
        using index_type = RegId::IndexType;

        using denset_set_index = std::vector<index_type>;

        using sparse_value_type = std::optional<index_type>;

//        using sparse_value_type = index_type;
        using sparse_set = std::vector<sparse_value_type>;
        using sparse_set_class = SparseSet<Type_>;

        using value_type = Type_;
        using reference = value_type &;
        using const_reference = const value_type &;

        using pos_data = SSetPosData<index_type>;

    private:
        denset_set dense_set_{};
        denset_set_index dense_set_index_{};
        sparse_set sparse_set_{};

        RegId max_id_{0};

    public:
        SparseSet() = default;

        ~SparseSet() = default;

        SparseSet(const sparse_set_class &other) :
                dense_set_(other.dense_set_),
                sparse_set_(other.sparse_set_) {}

        SparseSet(sparse_set_class &&other) noexcept:
                dense_set_(std::move(other.dense_set_)),
                sparse_set_(std::move(other.sparse_set_)) {}

        sparse_set_class &operator=(const SparseSet<Type_> &other) {
            dense_set_ = other.dense_set_;
            sparse_set_ = other.sparse_set_;
            return *this;
        }

        sparse_set_class &operator=(sparse_set_class &&other) noexcept {
            dense_set_ = std::move(other.dense_set_);
            sparse_set_ = std::move(other.sparse_set_);
            return *this;
        }

    private:
        inline void ensure_sparse_size_(index_type index) {
            auto l = sparse_set_.size();
            if (index > std::min(size_t{0}, sparse_set_.size() - 1)) {
                sparse_set_.resize(index + 1);
            }
        }

        inline void ensure_dense_size_(size_t size_) {
            dense_set_.resize(size_);
            dense_set_index_.resize(size_);
        }

    public:
        [[nodiscard]] RegId calc_max_id(index_type from_index) const noexcept {
            for(index_type i = from_index; i>=0; --i) {
                if (contains(i)) {
                    return RegId::from_index(i);
                }
            }

            return {0};
        }

        [[nodiscard]] size_type size() const noexcept {
            return dense_set_.size();
        }

        [[nodiscard]] const RegId& get_max_id() const noexcept {
            return max_id_;
        }

        inline void insert(index_type index, const_reference value) {
            ensure_sparse_size_(index);
            auto dense_position = size();
            dense_set_.push_back(value);
            dense_set_index_.push_back(index);

            auto l = sparse_set_.size();
            sparse_set_[index] = {dense_position};

            dense_set_[dense_position];

            if (index > max_id_.index()) {
                max_id_ = RegId::from_index(index);
            }
        }

        inline void insert(RegId reg_id, const_reference value) {
            insert(reg_id.index(), value);
        }

        void push_back(const_reference value) {
            index_type index = dense_set_.size();
            insert(index, value);
        }

        constexpr auto &at(index_type index) {
            return dense_set_.at(*sparse_set_.at(index));
        }

        inline constexpr auto &at(index_type index) const {
            return dense_set_.at(*sparse_set_.at(index));
        }

        inline constexpr auto& at(RegId reg_id) {
            return at(reg_id.index());
        }

        inline void erase(index_type index) {
            auto temp_size = size();

            auto erase_pos = assert_position(index);
            auto back_pos = assert_position(std::nullopt,
                                            temp_size - 1);

            dense_set_[erase_pos.dense_pos] =
                    std::move(dense_set_[back_pos.dense_pos]);

            dense_set_index_[erase_pos.dense_pos] =
                    std::move(dense_set_index_[back_pos.dense_pos]);

            sparse_set_[back_pos.sparse_pos] = {erase_pos.dense_pos};

            sparse_set_[erase_pos.sparse_pos] = std::nullopt;
            ensure_dense_size_(temp_size - 1);

            if (index == max_id_.index()) {
                max_id_ = calc_max_id(index-1);
            }
        }

        inline void erase(RegId reg_id) {
            erase(reg_id.index());
        }

        pos_data assert_position(
                std::optional<index_type> sparse_index = std::nullopt,
                std::optional<index_type> dense_index = std::nullopt) {
            auto dense_size = size();

            if (sparse_index && *sparse_index < dense_size) {
                return {*sparse_index, *sparse_set_[*sparse_index]};
            } else if (dense_index && *dense_index < dense_size) {
                return {dense_set_index_[*dense_index], *dense_index};
            }

            throw SSIndexError("Pos out of sparse set.");
        }

        auto &data() {
            return dense_set_.data();
        }

        [[nodiscard]] bool contains(index_type index) const {
            return sparse_set_[index].has_value();
        }

        [[nodiscard]] bool contains(RegId reg_id) const {
            return contains(reg_id.index());
        }


    public:
        auto &operator[](index_type index) {
            return dense_set_[*sparse_set_[index]];
        }

        auto &operator[](RegId reg_id) {
            return dense_set_[*sparse_set_[reg_id.index()]];
        }


    public:
        [[nodiscard]] inline constexpr auto begin() noexcept {
            return dense_set_.begin();
        }

        [[nodiscard]] inline constexpr auto end() noexcept {
            return dense_set_.end();
        }

        [[nodiscard]] inline constexpr auto cbegin() const noexcept {
            return dense_set_.cbegin();
        }

        [[nodiscard]] inline constexpr auto cend() const noexcept {
            return dense_set_.cend();
        }
    };


    template<templated_from<SparseSet> T>
    class SparseSetRegIdIter {
    public:
        using SparseType = T;

        SparseType &sparse_set;
        explicit SparseSetRegIdIter(SparseType& sparse_set_p):
            sparse_set(sparse_set_p) {}

        class Iterator {
        public:
            T &value;
            RegId current{0};

            explicit Iterator(T& val) : value(val) {
                current = next_reg_id();
            }

            Iterator(T &val, RegId pos) : value(val), current(pos) {}

            RegId next_reg_id() {
                for (RegId i = current; i < value.get_max_id(); ++i) {
                    if (value.contains(i)) {
                        return i;
                    }
                }
                return value.get_max_id();
            }

            Iterator &operator++() {
                current = next_reg_id();
                return *this;
            }

            RegId &operator*() {
                return current;
            }

            bool operator!=(const Iterator &other) const noexcept {
                return current!=other.current;
            }

            bool operator==(const Iterator &other) const noexcept {
                return current==other.current;
            }
        };

        SparseSetRegIdIter::Iterator begin() {
            return Iterator(sparse_set);
        }

        SparseSetRegIdIter::Iterator end() {
            return Iterator(sparse_set, sparse_set.get_max_id());
        }
    };


    template<templated_from<SparseSet> T>
    inline SparseSetRegIdIter<T> to_sparse_set_id_iter(T& sparse_set) {
        auto result = SparseSetRegIdIter<T>(sparse_set);
        return result;
    }

}


#endif //PONG_SRC_PONG_REGISTERS_SPARSE_SET_H_
