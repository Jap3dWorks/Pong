//
// Created by Jordi on 10/9/2022.
//

#ifndef PONG_SRC_PONG_SERIALIZER_DATA_BASE_H_
#define PONG_SRC_PONG_SERIALIZER_DATA_BASE_H_


namespace pong::serializer {

    template<typename DataTypeT, typename FieldTypeT>
    struct GetDataField {
    public:
        using DataType = DataTypeT;
        using FieldType = FieldTypeT;
    public:
        static inline void get(FieldType & field_type) {}
    };

#define IMPL_GET_DATA_FLD(DataTypeM, DataFldTypeM, FieldName) \
    template<> \
    struct GetDataField<DataTypeM, DataFldTypeM> { \
    using DataType = DataTypeM; \
    using FieldType = DataFldTypeM; \
    public: \
    static inline  SerializeDataT<FieldType>& get(DataType &data_obj) { \
        return data_obj.FieldName; \
        } \
    };

}

#endif //PONG_SRC_PONG_SERIALIZER_DATA_BASE_H_
