#pragma once

#include "Asset.h"

#include <cctype>
#include <fstream>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>

namespace BerkEngine {

class AssetManager {
public:
    bool registerTexture(TextureAsset asset) {
        mLoadedTextures.erase(asset.name);
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

    bool loadTexture(const std::string& name) {
        if (!hasTexture(name)) {
            return false;
        }
        mLoadedTextures.insert(name);
        return true;
    }

    bool unloadTexture(const std::string& name) {
        return mLoadedTextures.erase(name) == 1U;
    }

    void unloadAllTextures() {
        mLoadedTextures.clear();
    }

    bool isTextureLoaded(const std::string& name) const {
        return mLoadedTextures.find(name) != mLoadedTextures.end();
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

    bool registerShader(ShaderAsset shader) {
        const auto [it, inserted] = mShaders.insert_or_assign(shader.name, std::move(shader));
        (void)it;
        return inserted;
    }

    bool hasShader(const std::string& name) const {
        return mShaders.find(name) != mShaders.end();
    }

    ShaderAsset* findShader(const std::string& name) {
        auto it = mShaders.find(name);
        if (it == mShaders.end()) {
            return nullptr;
        }
        return &it->second;
    }

    const ShaderAsset* findShader(const std::string& name) const {
        auto it = mShaders.find(name);
        if (it == mShaders.end()) {
            return nullptr;
        }
        return &it->second;
    }

    bool registerMaterial(MaterialAsset material) {
        const auto [it, inserted] = mMaterials.insert_or_assign(material.name, std::move(material));
        (void)it;
        return inserted;
    }

    bool hasMaterial(const std::string& name) const {
        return mMaterials.find(name) != mMaterials.end();
    }

    MaterialAsset* findMaterial(const std::string& name) {
        auto it = mMaterials.find(name);
        if (it == mMaterials.end()) {
            return nullptr;
        }
        return &it->second;
    }

    const MaterialAsset* findMaterial(const std::string& name) const {
        auto it = mMaterials.find(name);
        if (it == mMaterials.end()) {
            return nullptr;
        }
        return &it->second;
    }

    bool registerMesh(MeshAsset mesh) {
        const auto [it, inserted] = mMeshes.insert_or_assign(mesh.name, std::move(mesh));
        (void)it;
        return inserted;
    }

    bool hasMesh(const std::string& name) const {
        return mMeshes.find(name) != mMeshes.end();
    }

    MeshAsset* findMesh(const std::string& name) {
        auto it = mMeshes.find(name);
        if (it == mMeshes.end()) {
            return nullptr;
        }
        return &it->second;
    }

    const MeshAsset* findMesh(const std::string& name) const {
        auto it = mMeshes.find(name);
        if (it == mMeshes.end()) {
            return nullptr;
        }
        return &it->second;
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
    std::unordered_map<std::string, ShaderAsset> mShaders{};
    std::unordered_map<std::string, MaterialAsset> mMaterials{};
    std::unordered_map<std::string, MeshAsset> mMeshes{};
    std::set<std::string> mLoadedTextures{};
};

} // namespace BerkEngine
