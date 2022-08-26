//
// Created by Jordi on 8/18/2022.
//

#ifndef GL_TEST_INSPECTOR_H
#define GL_TEST_INSPECTOR_H

#include "Pong/config/config.h"
#include "Pong/registers/reg_data_controller.h"
#include "Pong/core/geometry_data.h"
#include "Pong/serial_data/serial_data.h"
#include <string>
#include <regex>
#include <iostream>
#include <filesystem>
#include <vector>

namespace pong::engine {

//    using ActorDtLocation = RegDataLocation<>;

    using MeshDtLocation = RegDataLocation<Mesh>;
    using CurveDtLocation = RegDataLocation<Curve>;


    std::vector<MeshDtLocation> get_shape_dt(
        serializer::AssetDescriptor & descriptor,
        const std::string &asset_path,
        Mesh
        ) {
        auto result = std::vector<RegDataLocation<T>>();
        for (auto &dt : descriptor.mesh_data.data) {
            result.push_back(
                MeshDtLocation{dt.header.reg_id, asset_path}
                );
        }
        return result;
    }

    std::vector<CurveDtLocation> get_shape_dt(
        serializer::AssetDescriptor & descriptor,
        const std::string &asset_path,
        Curve){

    }



    class AssetInspector {

    private:
        std::string asset_pattern_{std::string(".*\\") + P_ASSET_EXTENSION + "$"};
        std::string assets_path_{P_ASSETS_DIR};

        std::regex re_asset_pattern_;

        std::vector<std::string> collected_files_{};
        RegDataController<MeshDtLocation, CurveDtLocation> reg_dt;

    public:
        AssetInspector(std::string asset_dir, std::string asset_patt) :
            assets_path_(std::move(asset_dir)),
            asset_pattern_(std::move(asset_patt)) {
            re_asset_pattern_ = std::regex(asset_pattern_);
        }

        void collect_asset_files() {
            auto inspect = [&](const auto &path_, auto &&inspect) -> void {
                auto m_ = std::smatch();
                for (const auto &entry: std::filesystem::directory_iterator(path_)) {
                    if (entry.is_directory()) {
                        inspect(entry.path().c_str(), inspect);
                    } else if (std::regex_search(entry.path().string(),
                                                 re_asset_pattern_)) {
                        collected_files_.push_back(entry.path().string());
                    }
                }
            };
            inspect(assets_path_, inspect);
        }

        void collect_asset_dt() {
            for (const auto& asset_path: collected_files_) {
                auto descriptor = serializer::AssetDescriptor();
                pong::serializer::load_headers(descriptor, asset_path.c_str());

                for (auto& dt: get_shape_dt<Mesh>(descriptor, asset_path)) {
                    reg_dt.insert_type<MeshDtLocation>(dt.reg_id, std::move(dt));
                }

                for (auto)
            }
        }

    };

}



#endif //GL_TEST_INSPECTOR_H
