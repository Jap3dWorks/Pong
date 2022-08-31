//
// Created by Jordi on 8/18/2022.
//

#ifndef PONG_SRC_PONG_ENGINE_ASSET_INSPECTOR_H_
#define PONG_SRC_PONG_ENGINE_ASSET_INSPECTOR_H_

#include "Pong/engine/engine_registers.h"
#include "Pong/config/config.h"
#include "Pong/registers/reg_data_controller.h"
#include "Pong/core/geometry_data.h"
#include "Pong/core/material.h"
#include "Pong/serializer/header_data.h"
#include "Pong/serializer/descriptors.h"
#include "Pong/map/map_logic.h"
#include "Pong/map/map.h"

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

    class ContentInspector {
    private:
        std::string asset_pattern_{std::string(".*\\") + P_ASSET_EXTENSION + "$"};
        std::string map_pattern_{std::string(".*\\") + P_ASSET_EXTENSION + "$"};
        std::string content_path_{P_CONTENT_DIR};

        std::regex re_asset_pattern_{asset_pattern_};
        std::regex re_map_pattern_{map_pattern_};

        std::vector<std::string> asset_files_{};
        std::vector<std::string> map_files_{};

        LocationRegisters data_location_reg_{};

    public:
        ContentInspector() = default;

        ContentInspector(std::string content_path, std::string asset_pattern, std::string map_pattern) :
            content_path_(std::move(content_path)),
            asset_pattern_(std::move(asset_pattern)),
            map_pattern_(std::move(map_pattern)) {

            re_asset_pattern_ = std::regex(asset_pattern_);
            re_map_pattern_ =std::regex(map_pattern_);

        }

        void collect_files() {
            auto inspect = [&](const auto &path_, auto &&inspect) -> void {
                auto m_ = std::smatch();
                for (const auto &entry : std::filesystem::directory_iterator(path_)) {
                    if (entry.is_directory()) {
                        inspect(entry.path().c_str(), inspect);
                    }
                    else if (std::regex_search(entry.path().string(),
                                                 re_asset_pattern_)) {
                        asset_files_.push_back(entry.path().string());
                    }
                    else if(std::regex_search(entry.path().string(),
                                                re_map_pattern_)) {
                        map_files_.push_back(entry.path().string());
                    }
                }
            };
            inspect(content_path_, inspect);
        }

        void collect_asset_data() {
            for (const auto &asset_path : asset_files_) {
                auto descriptor = serializer::AssetDescriptor();
                pong::serializer::load_headers(descriptor, asset_path.c_str());

                auto entity_iter = serializer::descriptor_data<
                    serializer::AssetDescriptor, serializer::EntityData>(descriptor);
                for (auto &dt: entity_iter) {

                }

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

        void collect_map_data() {
            for (auto &map_path : map_files_) {
                auto descriptor = serializer::MapDescriptor();
                pong::serializer::load_headers(descriptor, map_path.c_str());

                data_location_reg_.map_reg.insert_type<MapDtLocation>(
                    descriptor.map_data.header.reg_id,
                    {descriptor.map_data.header.reg_id, map_path}
                );
            }
        }

        void collect_maps_data() {}

        auto& get_data_location_reg() {
            return data_location_reg_;
        }
    };

    map::Map load_location(MapDtLocation & map_location) {
        auto descriptor = serializer::MapDescriptor();
        serializer::load_file(descriptor, map_location.file_path.c_str());

        return from_descriptor(descriptor);
    }

}

#endif //PONG_SRC_PONG_ENGINE_ASSET_INSPECTOR_H_
