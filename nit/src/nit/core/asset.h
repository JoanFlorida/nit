﻿#pragma once

namespace Nit
{
    struct AssetInfo
    {
        String type_name;
        ID id;
        String name;
        String path;
    };
    
    template<typename T>
    struct AssetTypeArgs
    {
        FnLoad<T>        fn_load        = nullptr;
        FnFree<T>        fn_free        = nullptr;
        FnSerialize<T>   fn_serialize   = nullptr;
        FnDeserialize<T> fn_deserialize = nullptr;
        u32              max_elements   = DEFAULT_POOL_ELEMENT_COUNT;
    };

    struct AssetRegistry
    {
        Map<String, Pool>  pools;
        Map<String, ID>    name_to_id;
        Map<ID, AssetInfo> id_to_info;
        String             extension = ".nit";
    };
    
    void SetAssetRegistryInstance(AssetRegistry* asset_registry_instance);
    AssetRegistry* GetAssetRegistryInstance();
    
    template<typename T>
    void RegisterAssetType(const AssetTypeArgs<T>& args)
    {
        AssetRegistry* asset_registry = GetAssetRegistryInstance();
        Pool& pool = asset_registry->pools.insert(GetTypeName<T>());

        if (!IsTypeRegistered<T>())
        {
            RegisterType<T>();
        }
        
        Type* type = GetType<T>();
        SetLoadFunction(type, args.fn_load);
        SetFreeFunction(type, args.fn_free);
        SetSerializeFunction(type, args.fn_serialize);
        SetDeserializeFunction(type, args.fn_deserialize);
        
        InitPool<T>(&pool, args.max_elements);
    }

    void PushAssetInfo(ID id, const String& name, const String& path);
    void EraseAssetInfo(ID id);
    
    Pool& GetAssetPool(const String& type_name);

    template<typename T>
    Pool& GetAssetPool()
    {
        return GetAssetPool(GetTypeName<T>());
    }
    
    template<typename T>
    ID CreateAsset(const String& name, const String& path = "", const T& data = {})
    {
        Pool& pool = GetAssetPool<T>();
        ID id; InsertPoolElement(&pool, id, data);
        PushAssetInfo(id, name, path);
        return id;
    }
    
    template<typename T>
    T& GetAssetData(ID id)
    {
        Pool& pool = GetAssetPool<T>();
        return GetPoolElement<T>(&pool, id);
    }

    void DestroyAsset(ID id);
    void DeserializeAssets();
    
    //void DeserializeAsset(ID id);
    //void LoadAssets();
    //void FreeAssets();
}