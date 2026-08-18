#pragma once

#include "Asset.h"

#include <cctype>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>

namespace BerkEngine {

class AssetManager {
public:
    bool registerTexture(TextureAsset asset) {
        const auto [it, inserted] = mTextures.insert_or_assign(asset.name, std::move(asset));
        (void)it;
        return inserted;
    }

    bool hasTexture(const std::string& name) const {
        return mTextures.find(name) != mTextures.end();
    }

    TextureAsset* findTexture(const std::string& name) {
        auto it = mTextures.find(name);
        if (it == mTextures.end()) {
            return nullptr;
        }
        return &it->second;
    }

    const TextureAsset* findTexture(const std::string& name) const {
        auto it = mTextures.find(name);
        if (it == mTextures.end()) {
            return nullptr;
        }
        return &it->second;
    }

    bool loadTextureManifest(const std::string& manifestPath) {
        std::ifstream file(manifestPath);
        if (!file.is_open()) {
            return false;
        }

        std::string line;
        while (std::getline(file, line)) {
            const std::string trimmed = trim(line);
            if (trimmed.empty() || trimmed[0] == '#') {
                continue;
            }

            std::istringstream stream(trimmed);
            TextureAsset asset{};
            if (!(stream >> asset.name >> asset.sourcePath >> asset.width >> asset.height)) {
                return false;
            }

            registerTexture(std::move(asset));
        }

        return true;
    }

private:
    static std::string trim(const std::string& value) {
        std::size_t begin = 0;
        while (begin < value.size() && std::isspace(static_cast<unsigned char>(value[begin])) != 0) {
            ++begin;
        }

        std::size_t end = value.size();
        while (end > begin && std::isspace(static_cast<unsigned char>(value[end - 1])) != 0) {
            --end;
        }

        return value.substr(begin, end - begin);
    }

    std::unordered_map<std::string, TextureAsset> mTextures{};
};

} // namespace BerkEngine
