﻿#include "asset.h"

#define NIT_CHECK_ASSET_REGISTRY_CREATED NIT_CHECK_MSG(asset_registry, "Forget to call SetAssetRegistryInstance!");

namespace Nit
{
    AssetRegistry* asset_registry = nullptr;
    
    void SetAssetRegistryInstance(AssetRegistry* asset_registry_instance)
    {
        NIT_CHECK(asset_registry_instance);
        asset_registry = asset_registry_instance;
    }
    
    AssetRegistry* GetAssetRegistryInstance()
    {
        NIT_CHECK_ASSET_REGISTRY_CREATED
        return asset_registry;
    }

    AssetPool* GetAssetPool(Type* type)
    {
        NIT_CHECK_ASSET_REGISTRY_CREATED

        if (!type)
        {
            return nullptr;
        }
        
        Array<AssetPool>* assets = &asset_registry->asset_pools;
        
        auto it = std::ranges::find_if(*assets, [&type](AssetPool& asset){
            return asset.data_pool.type == type;
        });
        
        if (it == assets->end())
        {
            return nullptr;
        }
        
        return &(*it);
    }

    AssetPool* GetAssetPoolSafe(Type* type)
    {
        NIT_CHECK_ASSET_REGISTRY_CREATED
        AssetPool* pool = GetAssetPool(type);
        if (!pool)
        {
            NIT_CHECK_MSG(false, "Trying to get the asset pool from non registered asset type");
        }
        return pool;
    }

    AssetPool* GetAssetPoolSafe(const AssetHandle& asset)
    {
        NIT_CHECK(asset.id != 0);
        return GetAssetPoolSafe(asset.type);
    }

    AssetPool* GetAssetPoolSafe(const AssetInfo& info)
    {
        NIT_CHECK(info.id != 0);
        return GetAssetPoolSafe(GetType(info.type_name));
    }

    bool IsAssetTypeRegistered(Type* type)
    {
        return GetAssetPool(type) != nullptr;
    }

    void BuildAssetPath(const String& name, String& path)
    {
        //TODO: sanity checks?
        if (!path.empty())
        {
            path.append("\\");    
        }
        path.append(name).append(asset_registry->extension);
    }

    void PushAssetInfo(AssetInfo& asset_info, u32 index, bool build_path)
    {
        AssetPool* pool = GetAssetPoolSafe(asset_info);
        
        if (build_path)
        {
            BuildAssetPath(asset_info.name, asset_info.path);
        }
        
        pool->asset_infos[index] = asset_info;
    }
    
    void EraseAssetInfo(AssetInfo& asset_info, SparseSetDeletion deletion)
    {
        AssetPool* pool = GetAssetPoolSafe(asset_info);
        pool->asset_infos[deletion.deleted_slot] = pool->asset_infos[deletion.last_slot];
    }

    AssetInfo* GetAssetInfo(const AssetHandle& asset)
    {
        AssetPool* pool = GetAssetPoolSafe(asset);
        
        if (!IsValid(&pool->data_pool, asset.id))
        {
            return nullptr;
        }
        
        return &pool->asset_infos[IndexOf(&pool->data_pool, asset.id)];
    }

    AssetInfo* GetAssetInfoSafe(const AssetHandle& asset)
    {
        AssetInfo* info = GetAssetInfo(asset);
        if (!info)
        {
            NIT_CHECK_MSG(false, "Trying to get the asset info from non registered asset type");
        }
        return info;
    }

    AssetHandle DeserializeAssetFromString(const String& asset_str)
    {
        AssetHandle result;
        YAML::Node node = YAML::Load(asset_str);

        if (YAML::Node asset_info_node = node["AssetInfo"])
        {
            AssetInfo asset_info;
            asset_info.type_name = asset_info_node["type_name"].as<String>();
            asset_info.name = asset_info_node["name"].as<String>();
            asset_info.path = asset_info_node["path"].as<String>();
            asset_info.id = asset_info_node["id"].as<ID>();
            asset_info.version = asset_info_node["version"].as<u32>();
            
            if (asset_info.version < GetLastAssetVersion(asset_info.type_name))
            {
                NIT_CHECK_MSG(false, "Trying to load an outdated asset, please upgrade the current asset version!!");
                return result;
            }
            
            if (YAML::Node asset_node = node[asset_info.type_name])
            {
                AssetPool* pool = GetAssetPool(GetType(asset_info.type_name));
                
                NIT_CHECK_MSG(pool, "Trying to deserialize an unregistered type of asset!");
                
                const bool created = IsValid(&pool->data_pool, asset_info.id);
                
                if (!created)
                {
                    InsertDataWithID(&pool->data_pool, asset_info.id);
                    PushAssetInfo(asset_info, IndexOf(&pool->data_pool, asset_info.id), false);
                }
                
                void* data = GetDataRaw(&pool->data_pool, asset_info.id);
                Deserialize(pool->data_pool.type, data, asset_node);

                result = { asset_info.name, GetType(asset_info.type_name), asset_info.id };
                
                if (created)
                {
                    Broadcast<const AssetCreatedArgs&>(GetAssetRegistryInstance()->asset_created_event, {result});
                }
                
                if (IsAssetLoaded(result))
                {
                    LoadAsset(result, true);
                }
            }
        }
        
        return result;
    }

    AssetHandle DeserializeAssetFromFile(const String& file_path)
    {
        InputFile input_file(file_path);

        if (input_file.is_open())
        {
            StringStream stream;
            stream << input_file.rdbuf();
            return DeserializeAssetFromString(stream.str());
        }
        
        NIT_CHECK_MSG(false, "Cannot open file!");
        return {};
    }

    void SerializeAssetToString(const AssetHandle& asset, String& result)
    {
        AssetPool* pool = GetAssetPoolSafe(asset);
        AssetInfo* info = GetAssetInfoSafe(asset);

        YAML::Emitter emitter;
            
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "AssetInfo" << YAML::Value << YAML::BeginMap;
        emitter << YAML::Key << "type_name" << YAML::Value << info->type_name;
        emitter << YAML::Key << "name"      << YAML::Value << info->name;
        emitter << YAML::Key << "path"      << YAML::Value << info->path;
        emitter << YAML::Key << "id"        << YAML::Value << info->id;
        emitter << YAML::Key << "version"   << YAML::Value << info->version;
        emitter << YAML::EndMap;
        
        emitter << YAML::Key << info->type_name << YAML::Value << YAML::BeginMap;
        
        Serialize(pool->data_pool.type, GetDataRaw(&pool->data_pool, info->id), emitter);

        emitter << YAML::EndMap;

        result = emitter.c_str();
    }

    void SerializeAssetToFile(const AssetHandle& asset)
    {
        AssetInfo* info = GetAssetInfoSafe(asset);
        
        OutputFile file(info->path);
        
        if (file.is_open())
        {
            String asset_string;
            SerializeAssetToString(asset, asset_string);
            file << asset_string;
            file.flush();
            file.close();
            return;
        }
        
        NIT_CHECK_MSG(false, "Cannot open file!");
    }

    void InitAssetRegistry()
    {
        NIT_CHECK_ASSET_REGISTRY_CREATED
        
        for (const auto& dir_entry : RecursiveDirectoryIterator(GetWorkingDirectory()))
        {
            const Path& dir_path = dir_entry.path();
            
            if (dir_entry.is_directory() || dir_path.extension().string() != asset_registry->extension)
            {
                continue;
            }

            DeserializeAssetFromFile(dir_path.string());
        }
    }

    u32 GetLastAssetVersion(Type* type)
    {
        return GetAssetPoolSafe(type)->version;
    }

    u32 GetLastAssetVersion(const String& type_name)
    {
        return GetLastAssetVersion(GetType(type_name));
    }

    void FindAssetsByName(const String& name, Array<AssetHandle>& assets)
    {
        NIT_CHECK_ASSET_REGISTRY_CREATED

        for (AssetPool& asset_pool : asset_registry->asset_pools)
        {
            for (u32 i = 0; i < asset_pool.data_pool.sparse_set.count; ++i)
            {
                AssetInfo* asset_info = &asset_pool.asset_infos[i];
                if (asset_info->name == name)
                {
                    assets.push_back({ asset_info->name, GetType(asset_info->type_name), asset_info->id });
                }
            }
        }
    }

    AssetHandle FindAssetByName(const String& name)
    {
        NIT_CHECK_ASSET_REGISTRY_CREATED

        for (AssetPool& asset_pool : asset_registry->asset_pools)
        {
            for (u32 i = 0; i < asset_pool.data_pool.sparse_set.count; ++i)
            {
                AssetInfo* asset_info = &asset_pool.asset_infos[i];
                if (asset_info->name == name)
                {
                    return { asset_info->name, GetType(asset_info->type_name), asset_info->id };
                }
            }
        }
        
        return {};
    }

    bool IsAssetValid(const AssetHandle& asset)
    {
        AssetPool* pool = GetAssetPool(asset.type);
        if (!pool)
        {
            return false;
        }
        return IsValid(&pool->data_pool, asset.id);
    }
    
    bool IsAssetLoaded(const AssetHandle& asset)
    {
        if (!IsAssetValid(asset))
        {
            return false;
        }
        
        return GetAssetInfoSafe(asset)->loaded;
    }

    void DestroyAsset(AssetHandle& asset)
    {
        //TODO: Delete file?

        AssetPool* pool = GetAssetPoolSafe(asset);
        AssetInfo* info = GetAssetInfoSafe(asset);
        
        FreeAsset(asset);

        AssetDestroyedArgs args;
        args.asset_handle.id   = info->id;
        args.asset_handle.type = pool->data_pool.type;
        args.asset_handle.name = info->name;
        Broadcast<const AssetDestroyedArgs&>(asset_registry->asset_destroyed_event, args);
        
        EraseAssetInfo(*info, DeleteData(&pool->data_pool, info->id));
    }

    void LoadAsset(AssetHandle& asset, bool force_reload)
    {
        AssetPool* pool = GetAssetPoolSafe(asset);
        AssetInfo* info = GetAssetInfoSafe(asset);
        
        if (info->loaded)
        {
            if (force_reload)
            {
                // In this case we should keep the reference count.
                u32 reference_count = info->reference_count;
                FreeAsset(asset);
                info->reference_count = reference_count;
            }
            else
            {
                return;
            }
        }
        
        info->loaded = true;
        Load(asset.type, GetDataRaw(&pool->data_pool, asset.id));
    }

    void FreeAsset(AssetHandle& asset)
    {
        AssetPool* pool = GetAssetPoolSafe(asset);
        AssetInfo* info = GetAssetInfoSafe(asset);
        info->reference_count = 0;
        Free(asset.type, GetDataRaw(&pool->data_pool, asset.id));
        asset = {};
    }

    void RetainAsset(AssetHandle& asset)
    {
        if (!IsAssetValid(asset))
        {
            return;
        }

        AssetInfo* info = GetAssetInfoSafe(asset);

        if (!info->loaded)
        {
            LoadAsset(asset);
        }

        ++info->reference_count;
    }

    void ReleaseAsset(AssetHandle& asset, bool force_free)
    {
        if (!IsAssetValid(asset))
        {
            return;
        }
        
        AssetInfo* info = GetAssetInfoSafe(asset);

        if (!info->loaded)
        {
            return;
        }

        // A loaded asset with reference_count of 0 is a valid case.
        if (force_free || info->reference_count <= 1)
        {
            FreeAsset(asset);
            info->reference_count = 0;
            return;
        }
        
        --info->reference_count;
    }
}