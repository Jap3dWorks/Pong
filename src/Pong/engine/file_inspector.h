//
// Created by Jordi on 8/18/2022.
//

#ifndef GL_TEST_INSPECTOR_H
#define GL_TEST_INSPECTOR_H

#include "Pong/engine/engine_registers.h"
#include "Pong/config/config.h"
#include "Pong/registers/reg_data_controller.h"
#include "Pong/core/geometry_data.h"
#include "Pong/core/material.h"
#include "Pong/serial_data/header_data.h"
#include "Pong/serial_data/descriptors.h"
#include <string>
#include <regex>
#include <iostream>
#include <filesystem>
#include <vector>


namespace pong::engine {
    /**
    * RegDataLocation represents where you can find the RegId associated data
    * in the file system.
    */

    class AssetInspector {
    public:


    private:
        std::string asset_pattern_{std::string(".*\\") + P_ASSET_EXTENSION + "$"};
        std::string assets_path_{P_ASSETS_DIR};
        std::regex re_asset_pattern_{asset_pattern_};

        std::vector<std::string> collected_files_{};

        LocationRegister data_location_reg_{};

    public:
        AssetInspector() = default;

        AssetInspector(std::string asset_dir, std::string asset_patt) :
            assets_path_(std::move(asset_dir)),
            asset_pattern_(std::move(asset_patt)) {
            re_asset_pattern_ = std::regex(asset_pattern_);
        }

        void collect_asset_files() {
            auto inspect = [&](const auto &path_, auto &&inspect) -> void {
                auto m_ = std::smatch();
                for (const auto &entry : std::filesystem::directory_iterator(path_)) {
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

        void collect_asset_data() {
            for (const auto &asset_path : collected_files_) {
                auto descriptor = serializer::AssetDescriptor();
                pong::serializer::load_headers(descriptor, asset_path.c_str());

                auto mesh_iter = serializer::descriptor_data<
                    serializer::AssetDescriptor, Mesh>(descriptor);
                for (auto &dt : mesh_iter) {
                    data_location_reg_.graphic_reg.insert_type<MeshDtLocation>(
                        dt.header.reg_id, {dt.header.reg_id, asset_path}
                    );
                }

                auto curve_iter = serializer::descriptor_data<
                    serializer::AssetDescriptor, Curve>(descriptor);
                for (auto &dt : curve_iter) {
                    data_location_reg_.graphic_reg.insert_type<CurveDtLocation>(
                        dt.header.reg_id, {dt.header.reg_id, asset_path}
                    );
                }

                auto material_iter = serializer::descriptor_data<
                    serializer::AssetDescriptor, Material>(descriptor);
                for (auto &dt : material_iter) {

                }
            }
        }

        auto& get_data_location_reg() {
            return data_location_reg_;
        }

    };

}

#endif //GL_TEST_INSPECTOR_H
