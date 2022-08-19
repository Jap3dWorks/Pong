//
// Created by Jordi on 8/19/2022.
//

#ifndef PONG_ASSET_DATA_REG_H
#define PONG_ASSET_DATA_REG_H

#include "Pong/registers/reg_id.h"
#include "Pong/registers/type_reg_map.h"
#include "Utils/type_conditions.h"
#include "Pong/registers/sparse_set.h"
#include <string>


namespace Pong {
    
    // a type conjuntion
    template<typename ...Types>
    struct conjunction_type {
//        using types = Types;
    };
    
    struct file_data {
        std::string file_path;
        size_t offset;
    };

    template<typename ...Types>
    class FileRegData {
    private:
        using Registry = TypeRegMap;
        using id_set_t = RegIdArray<>;
        
        template<Intersects<Types...> comp>
        using type_data = SparseSet<comp>;
        
    private:
        Registry registry_{};
        id_set_t reg_id_{};
    };
}

#endif //PONG_ASSET_DATA_REG_H
