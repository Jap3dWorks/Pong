//
// Created by Jordi on 8/8/2022.
//

#ifndef GL_TEST_REG_ID_MANAGER_H
#define GL_TEST_REG_ID_MANAGER_H

#include<iostream>
#include<unordered_set>
#include <queue>
#include <cassert>
#include <array>
#include <cstdlib>
#include <random>


namespace Pong {

//    enum class RegId : uint32_t {
//    };
//
//    constexpr auto to_integer(RegId reg_id) noexcept {
//        return static_cast<std::underlying_type_t<RegId>>(reg_id);
//    }
//
//    RegId &operator++(RegId &reg_id) {
//        using IntType = typename std::underlying_type<RegId>::type;
//        reg_id = static_cast<RegId>( static_cast<IntType>(reg_id) + 1 );
//        return reg_id;
//    }
//
//    RegId operator++(RegId &c, int) {
//        RegId result = c;
//        ++c;
//        return result;
//    }


    using RegId = uint32_t;


#ifndef NDEBUG
#define DECLARE_ASSERT_MAX_SIZE(max_size) \
    void inline constexpr assert_max_size(const size_t& size) const { \
        assert(size <= max_size); \
    }

#define ASSERT_MAX_SIZE(size_) \
        assert_max_size(size_);

#else
#define DECLARE_ASSERT_MAX_SIZE(max_size)
#define ASSERT_MAX_SIZE(size_)
#endif


    template<size_t MAX=4096>
    class StaticSparseSet {
    private:
        RegId dense_[MAX]{0};
        RegId sparse_[MAX]{0};
        uint8_t values_[MAX]{0};
        size_t size_{0};
        size_t highest_{0};

    public:
        StaticSparseSet() noexcept = default;

        DECLARE_ASSERT_MAX_SIZE(MAX);

        inline auto constexpr insert(RegId index) noexcept {
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

        inline auto constexpr erase(RegId index) noexcept {
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

        [[nodiscard]] inline auto constexpr contains(RegId index) const noexcept {
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


    template<size_t MAX=4096>
    class RegIdArray {
    private:
        StaticSparseSet<MAX> indices_free{};
        StaticSparseSet<MAX> indices_valid{};

    public:

        RegIdArray() noexcept = default;

        DECLARE_ASSERT_MAX_SIZE(MAX);


        inline auto create() {
            ASSERT_MAX_SIZE();

            auto index = indices_free.get();
            indices_free.erase(index);
            indices_valid.insert(index);

            if (index == indices_valid.highest()) {
                indices_free.insert(indices_valid.highest() + 1);
            }

            return index;
        }

        inline auto constexpr insert(RegId index) noexcept {

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

        inline auto constexpr erase(RegId index) noexcept {
            indices_valid.erase(index);
            indices_free.insert(index);
        }

        [[nodiscard]] inline auto constexpr contains(RegId index) const noexcept {
            return indices_valid.contains(index);
        }

        [[nodiscard]] inline auto constexpr begin() const noexcept {
            return indices_valid.begin();
        }

        [[nodiscard]] inline auto constexpr end() const noexcept {
            return indices_valid.end();
        }

        [[nodiscard]] inline auto constexpr max_size() const noexcept  {
            return MAX;
        }

        [[nodiscard]] inline auto constexpr size() const noexcept {
            return indices_valid.size();
        }
    };


    template<typename ...Types>
    struct RegIdCollection {
        inline static constexpr RegIdArray<> reg_id_array_{};

        inline static auto& get() {
            return reg_id_array_;
        }
    };


}
#endif //GL_TEST_REG_ID_MANAGER_H
