//
// Created by Jordi on 8/4/2022.
//

#ifndef GL_TEST_SPARSE_SET_H
#define GL_TEST_SPARSE_SET_H

#include <typeinfo>
#include <iostream>
#include <vector>
#include <exception>
#include <optional>

// https://www.geeksforgeeks.org/sparse-set/

namespace Pong {

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
        using size_type = typename denset_set::size_type;
        using denset_set_index = std::vector<size_type>;

        using sparse_value_type = std::optional<size_type>;
        using sparse_set = std::vector<sparse_value_type>;
        using sparse_set_class = SparseSet<Type_>;

        using value_type = Type_;
        using reference = value_type &;
        using const_reference = const value_type &;

        using pos_data = SSetPosData<size_type>;

        using dense_iterator = typename denset_set::iterator;
        using sparse_iterator = typename sparse_set::iterator;

    private:
        denset_set dense_set_{};
        denset_set_index dense_set_index_{};
        sparse_set sparse_set_{};

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
        }

    private:
        inline void ensure_sparse_size_(size_type pos) {
            if (pos > sparse_set_.max_size() - 1) {
                sparse_set_.resize(pos + 1);
            }
        }

        inline void ensure_dense_size_(size_type size_) {
            dense_set_.resize(size_);
            dense_set_index_.resize(size_);
        }

    public:
        auto &insert(size_type pos, const_reference value) {
            ensure_sparse_size_(pos);
            auto dense_position = size();
            dense_set_.push_back(value);
            dense_set_index_.push_back(pos);
            sparse_set_[pos] = dense_position;

            return dense_set_[dense_position];
        }

        auto &push_back(const_reference value) {
            auto pos = dense_set_.size();
            return insert(pos, value);
        }

        constexpr auto &at(size_type pos) {
            return dense_set_.at(*sparse_set_.at(pos));
        }

        constexpr auto &at(size_type pos) const {
            return dense_set_.at(*sparse_set_.at(pos));
        }

        size_type size() {
            return dense_set_.size();
        }

        void erase(size_type pos) {
            size_type temp_size = size();

            auto erase_pos = assert_position(pos);
            auto back_pos = assert_position(std::nullopt,
                                            temp_size - 1);

            dense_set_[erase_pos.dense_pos] =
                    std::move(dense_set_[back_pos.dense_pos]);

            dense_set_index_[erase_pos.dense_pos] =
                    std::move(dense_set_index_[back_pos.dense_pos]);

            sparse_set_[back_pos.sparse_pos] = erase_pos.dense_pos;

            sparse_set_[erase_pos.sparse_pos] = std::nullopt;
            ensure_dense_size_(temp_size - 1);
        }

        pos_data assert_position(
                std::optional<size_type> sparse_pos = std::nullopt,
                std::optional<size_type> dense_pos = std::nullopt) {
            auto dense_size = size();

            if (sparse_pos && *sparse_pos < dense_size) {
                return {*sparse_pos, *sparse_set_[*sparse_pos]};
            } else if (dense_pos && *dense_pos < dense_size) {
                return {dense_set_index_[*dense_pos], *dense_pos};
            }

            throw SSIndexError("Pos out of sparse set.");
        }

        auto &data() {
            return dense_set_.data();
        }

    public:
        auto &operator[](size_type pos) {
            return dense_set_[*sparse_set_[pos]];
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
}


#endif //GL_TEST_SPARSE_SET_H
