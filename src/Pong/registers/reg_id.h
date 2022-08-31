//
// Created by Jordi on 8/8/2022.
//

#ifndef PONG_REG_ID_H_
#define PONG_REG_ID_H_

#include<iostream>
#include<unordered_set>
#include <queue>
#include <cassert>
#include <array>
#include <cstdlib>
#include <random>

namespace pong {
    struct RegId {
        using IdType = uint32_t;
        using IndexType = IdType;

        IdType id{0};

//        RegId()=default;
        ~RegId()=default;
//        RegId(IdType id_p) noexcept : id(id_p) {}

        static inline constexpr RegId from_index(IndexType index) {
            return RegId{index + 1};
        }

        explicit operator bool() const noexcept {
            return id;
        }

        [[nodiscard]] IndexType index() const noexcept {
            return id - 1;
        }

        RegId& operator++() {
            ++id;
            return *this;
        }

        RegId operator++(int) {
            auto temp = RegId{id};
            ++id;
            return temp;
        }

        RegId& operator--() {
            --id;
            return *this;
        }

        RegId operator--(int) {
            auto temp = RegId{id};
            --id;
            return temp;
        }

        bool operator==(const RegId& other) const {
            return id == other.id;
        }

        bool operator<(const RegId& other) const {
            return id < other.id;
        }
    };

    std::ostream & operator<<(std::ostream& out, const RegId& reg_id) {
        out << reg_id.id;
        return out;
    }


#ifndef NDEBUG
#define DECLARE_ASSERT_MAX_SIZE(max_size) \
    void inline constexpr assert_max_size(const size_t& size) const { \
        assert(size <= (max_size)); \
    }

#define ASSERT_MAX_SIZE(size_) \
        assert_max_size(size_);

#else
#define DECLARE_ASSERT_MAX_SIZE(max_size)
#define ASSERT_MAX_SIZE(size_)
#endif


    template<size_t MAX=4096>
    class StaticSparseSet {
    public:
        using index_type = RegId::IndexType;
    private:
        index_type dense_[MAX]{0};
        index_type sparse_[MAX]{0};
        uint8_t values_[MAX]{0};
        size_t size_{0};
        size_t highest_{0};

    public:
        StaticSparseSet() noexcept = default;

        DECLARE_ASSERT_MAX_SIZE(MAX);

        inline auto constexpr insert(index_type index) noexcept {
            if (!contains(index)) {
                sparse_[index] = size_;
                dense_[size_]=index;
                values_[index] = 1;
                ++size_;

                highest_ = std::max(highest_, static_cast<decltype(highest_)>(index));
            }
        }

        inline void constexpr update_highest() noexcept {
            for (; highest_ > 0; --highest_) {
                if (contains(highest_)) {
                    break;
                }
            }
        }

        inline auto constexpr erase(index_type index) noexcept {
            if (contains(index)) {
                auto dense_id = sparse_[index];
                dense_[dense_id] = dense_[size_ - 1];
                values_[index] = 0;
                sparse_[dense_[dense_id]] = dense_id;
                --size_;

                if (index == highest_) {
                    --highest_;
                    update_highest();
                }
            }
        }

        [[nodiscard]] inline auto constexpr contains(index_type index) const noexcept {
//            ASSERT_MAX_SIZE(index);
            if (values_[index] != 0) {
                return true;
            }
            return false;
        }

        [[nodiscard]] inline auto constexpr begin() const noexcept {
            return dense_;
        }

        [[nodiscard]] inline auto constexpr end() const noexcept {
            return dense_ + std::min(size_ - 1, MAX-1);
        }

        [[nodiscard]] inline auto constexpr max_size() const noexcept  {
            return MAX;
        }

        [[nodiscard]] inline auto constexpr size() const noexcept {
            return size_;
        }

        [[nodiscard]] inline auto constexpr front() const noexcept {
            return dense_[0];
        }

        [[nodiscard]] inline auto constexpr back() const noexcept {
            return dense_[size_ - 1];
        }

        [[nodiscard]] inline auto constexpr middle() const noexcept {
            return dense_[(size_ - 1) / 2];
        }

        /*
         * Return a Random index
         */
        [[nodiscard]] inline auto constexpr get() const noexcept {
            return dense_[(std::rand() % size_)];
        }

        [[nodiscard]] inline auto constexpr highest() const noexcept {
            return highest_;
        }
    };


    // TODO: min id: 1, 0 invalid id
    template<size_t MAX=4096>
    class RegIdArray {
    private:
        StaticSparseSet<MAX> indices_free{};
        StaticSparseSet<MAX> indices_valid{};

    public:

        RegIdArray() noexcept = default;

        DECLARE_ASSERT_MAX_SIZE(MAX);


        inline auto create() {
            ASSERT_MAX_SIZE(indices_valid.size());

            auto index = indices_free.get();
            indices_free.erase(index);
            indices_valid.insert(index);

            if (index == indices_valid.highest()) {
                indices_free.insert(indices_valid.highest() + 1);
            }

            return RegId::from_index(index);
        }

        inline auto constexpr insert(RegId::IndexType index) noexcept {
            ASSERT_MAX_SIZE(index);

            if (!contains(index)) {
                auto temp_highest = indices_valid.highest();
                indices_valid.insert(index);

                if (indices_free.contains(index)) {
                    indices_free.erase(index);
                }

                if (temp_highest < indices_valid.highest()) {
                    for (size_t i = temp_highest + 2; i < indices_valid.highest(); ++i) {
                        indices_free.insert(i);
                    }
                    indices_free.insert(index + 1);
                }
            }
        }

        inline auto constexpr insert(RegId reg_id) noexcept {
            insert(reg_id.index());
        }


        inline auto constexpr erase(RegId::IndexType index) noexcept {
            indices_valid.erase(index);
            indices_free.insert(index);
        }

        inline auto constexpr erase(RegId index) noexcept {
            indices_valid.erase(index.index());
            indices_free.insert(index.index());
        }

        [[nodiscard]] inline auto constexpr contains(RegId reg_id) const noexcept {
            return contains(reg_id.index());
        }

        [[nodiscard]] inline auto constexpr contains(RegId::IndexType index) const noexcept {
            return indices_valid.contains(index);
        }

        [[nodiscard]] inline auto constexpr max_size() const noexcept  {
            return MAX;
        }

        [[nodiscard]] inline auto constexpr size() const noexcept {
            return indices_valid.size();
        }

        struct Iterator {
            RegId::IndexType * ptr_;

            RegId operator*() {
                return RegId::from_index(*ptr_);
            }

            Iterator& operator++ () {
                ++ptr_;
                return *this;
            }

            Iterator operator++ (int) {
                auto temp = *this;
                ++ptr_;
                return temp;
            }

            Iterator& operator-- () {
                --ptr_;
                return *this;
            }

            Iterator operator-- (int) {
                auto temp = *this;
                --ptr_;
                return temp;
            }

            bool operator==(const Iterator& other) {
                return ptr_ == other.ptr_;
            }
        };

        [[nodiscard]] inline auto constexpr begin() const noexcept {
            return RegIdArray::Iterator{indices_valid.begin()};
        }

        [[nodiscard]] inline auto constexpr end() const noexcept {
            return RegIdArray::Iterator{indices_valid.end()};
        }
    };


//    template<typename ...Types>
//    struct RegIdCollection {
//        inline static constexpr RegIdArray<> reg_id_array_{};
//
//        inline static auto& get() {
//            return reg_id_array_;
//        }
//
//    };


}

template<>
struct std::hash <pong::RegId> {
    std::size_t operator()(const pong::RegId& reg_id) const noexcept {
        return std::hash<pong::RegId::IdType>()(reg_id.id);
    }
};

#endif //PONG_REG_ID_H_
