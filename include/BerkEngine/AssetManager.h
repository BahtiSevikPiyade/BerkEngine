#pragma once

#include "Asset.h"

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

private:
    std::unordered_map<std::string, TextureAsset> mTextures{};
};

} // namespace BerkEngine
