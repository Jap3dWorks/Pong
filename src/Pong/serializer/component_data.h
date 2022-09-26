//
// Created by Jordi on 9/21/2022.
//

#ifndef PONG_SRC_PONG_SERIALIZER_COMPONENT_DATA_H_
#define PONG_SRC_PONG_SERIALIZER_COMPONENT_DATA_H_

#include <boost/preprocessor.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/arithmetic/add.hpp>
#include <boost/preprocessor/control/while.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>

#include "Pong/components/component.h"

namespace pong::serializer {
// https://metricpanda.com/rival-fortress-update-39-how-i-use-__counter__-to-localize-text-and-hash-strings-at-compile-time/

#define T(name) name##__COUNTER__


#define prev_count_range_1(...)(1)
#define prev_count_range_2(...) prev_count_range_1()(2)
#define prev_count_range_3(...) prev_count_range_2()(3)
#define prev_count_range_4(...) prev_count_range_3()(4)
#define prev_count_range_5(...) prev_count_range_4()(5)
#define prev_count_range_6(...) prev_count_range_5()(6)
#define prev_count_range_7(...) prev_count_range_6()(7)
#define prev_count_range_8(...) prev_count_range_7()(8)
#define prev_count_range_9(...) prev_count_range_8()(9)
#define prev_count_range_10(...) prev_count_range_9()(10)
#define prev_count_range_11(...) prev_count_range_10()(11)
#define prev_count_range_12(...) prev_count_range_11()(12)
#define prev_count_range_13(...) prev_count_range_12()(13)
#define prev_count_range_14(...) prev_count_range_13()(14)
#define prev_count_range_15(...) prev_count_range_14()(15)
#define prev_count_range_16(...) prev_count_range_15()(16)
#define prev_count_range_17(...) prev_count_range_16()(17)
#define prev_count_range_18(...) prev_count_range_17()(18)
#define prev_count_range_19(...) prev_count_range_18()(19)
#define prev_count_range_20(...) prev_count_range_19()(20)
#define prev_count_range_21(...) prev_count_range_20()(21)
#define prev_count_range_22(...) prev_count_range_21()(22)
#define prev_count_range_23(...) prev_count_range_22()(23)
#define prev_count_range_24(...) prev_count_range_23()(24)
#define prev_count_range_25(...) prev_count_range_24()(25)
#define prev_count_range_26(...) prev_count_range_25()(26)
#define prev_count_range_27(...) prev_count_range_26()(27)
#define prev_count_range_28(...) prev_count_range_27()(28)
#define prev_count_range_29(...) prev_count_range_28()(29)
#define prev_count_range_30(...) prev_count_range_29()(30)
#define prev_count_range_31(...) prev_count_range_30()(31)
#define prev_count_range_32(...) prev_count_range_31()(32)
#define prev_count_range_33(...) prev_count_range_32()(33)
#define prev_count_range_34(...) prev_count_range_33()(34)
#define prev_count_range_35(...) prev_count_range_34()(35)
#define prev_count_range_36(...) prev_count_range_35()(36)
#define prev_count_range_37(...) prev_count_range_36()(37)
#define prev_count_range_38(...) prev_count_range_37()(38)
#define prev_count_range_39(...) prev_count_range_38()(39)
#define prev_count_range_40(...) prev_count_range_39()(40)
#define prev_count_range_41(...) prev_count_range_40()(41)
#define prev_count_range_42(...) prev_count_range_41()(42)
#define prev_count_range_43(...) prev_count_range_42()(43)
#define prev_count_range_44(...) prev_count_range_43()(44)
#define prev_count_range_45(...) prev_count_range_44()(45)
#define prev_count_range_46(...) prev_count_range_45()(46)
#define prev_count_range_47(...) prev_count_range_46()(47)
#define prev_count_range_48(...) prev_count_range_47()(48)
#define prev_count_range_49(...) prev_count_range_48()(49)
#define prev_count_range_50(...) prev_count_range_49()(50)
#define prev_count_range_51(...) prev_count_range_50()(51)
#define prev_count_range_52(...) prev_count_range_51()(52)
#define prev_count_range_53(...) prev_count_range_52()(53)
#define prev_count_range_54(...) prev_count_range_53()(54)
#define prev_count_range_55(...) prev_count_range_54()(55)
#define prev_count_range_56(...) prev_count_range_55()(56)
#define prev_count_range_57(...) prev_count_range_56()(57)
#define prev_count_range_58(...) prev_count_range_57()(58)
#define prev_count_range_59(...) prev_count_range_58()(59)
#define prev_count_range_60(...) prev_count_range_59()(60)
#define prev_count_range_61(...) prev_count_range_60()(61)
#define prev_count_range_62(...) prev_count_range_61()(62)
#define prev_count_range_63(...) prev_count_range_62()(63)
#define prev_count_range_64(...) prev_count_range_63()(64)


#define clean_val(...) __VA_ARGS__
#define seq_val(...) (__VA_ARGS__)

#define REVERSE_SEQ(v) prev_count_range_##v()

constexpr uint32_t total_components = COUNTER_READ(component::component_tag);

#if defined(component_count_32)
#define comp_seq REVERSE_SEQ(32)
#elif defined(component_count_31)
#define comp_seq REVERSE_SEQ(31)
#elif defined(component_count_30)
#define comp_seq REVERSE_SEQ(30)
#elif defined(component_count_29)
#define comp_seq REVERSE_SEQ(29)
#elif defined(component_count_28)
#define comp_seq REVERSE_SEQ(28)
#elif defined(component_count_27)
#define comp_seq REVERSE_SEQ(27)
#elif defined(component_count_26)
#define comp_seq REVERSE_SEQ(26)
#elif defined(component_count_25)
#define comp_seq REVERSE_SEQ(25)
#elif defined(component_count_24)
#define comp_seq REVERSE_SEQ(24)
#elif defined(component_count_23)
#define comp_seq REVERSE_SEQ(23)
#elif defined(component_count_22)
#define comp_seq REVERSE_SEQ(22)
#elif defined(component_count_21)
#define comp_seq REVERSE_SEQ(21)
#elif defined(component_count_20)
#define comp_seq REVERSE_SEQ(20)
#elif defined(component_count_19)
#define comp_seq REVERSE_SEQ(19)
#elif defined(component_count_17)
#define comp_seq REVERSE_SEQ(17)
#elif defined(component_count_16)
#define comp_seq REVERSE_SEQ(16)
#elif defined(component_count_15)
#define comp_seq REVERSE_SEQ(15)
#elif defined(component_count_14)
#define comp_seq REVERSE_SEQ(14)
#elif defined(component_count_13)
#define comp_seq REVERSE_SEQ(13)
#elif defined(component_count_12)
#define comp_seq REVERSE_SEQ(12)
#elif defined(component_count_12)
#define comp_seq REVERSE_SEQ(12)
#elif defined(component_count_11)
#define comp_seq REVERSE_SEQ(11)
#elif defined(component_count_10)
#define comp_seq REVERSE_SEQ(10)
#elif defined(component_count_9)
#define comp_seq REVERSE_SEQ(9)
#elif defined(component_count_8)
#define comp_seq REVERSE_SEQ(8)
#elif defined(component_count_7)
#define comp_seq REVERSE_SEQ(7)
#elif defined(component_count_6)
#define comp_seq REVERSE_SEQ(6)
#elif defined(component_count_5)
#define comp_seq REVERSE_SEQ(5)
#elif defined(component_count_4)
#define comp_seq REVERSE_SEQ(4)
#elif defined(component_count_3)
#define comp_seq REVERSE_SEQ(3)
#elif defined(component_count_2)
#define comp_seq REVERSE_SEQ(2)
#else
#define comp_seq REVERSE_SEQ(1)
#endif



template<typename C>
struct CompoData{};


#define COMPONENT_ATTRIB(z, n, k, total) \
    template<>                                      \
    struct CompoData<typename component::components_collected<total>::combine_types::variadic_types<z>::type> { \
    using compo_type = typename component::components_collected<total>::combine_types::variadic_types<z>::type; \
    compo_type component_type{};                 \
    };


BOOST_PP_SEQ_FOR_EACH_I(COMPONENT_ATTRIB, int, comp_seq);


//
//#define _ALLCOMPODATAS() \
//BOOST_PP_TUPLE_ELEM(2, 0, BOOST_PP_WHILE(PRED, OP, (n, n)))



//static void all_compodatas() {
//    using Range = boost::mpl::range_c<uint32_t, 1 , COUNTER_READ(component::component_tag)>;
//    boost::mpl::for_each<Range>(
//        [&]<typename I>(I i) constexpr -> void {
//            typename component::components_collected<COUNTER_READ(component::component_tag)>::combine_types::variadic_types<I::value>::type;
//        }
//        );
//}





}

#endif //PONG_SRC_PONG_SERIALIZER_COMPONENT_DATA_H_
