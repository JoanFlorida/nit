﻿#pragma once

namespace nit
{
    struct Pool
    {
        Type*          type                = nullptr;
        void*          elements            = nullptr;
        SparseSet      sparse_set          = {};
        Queue<u32>     available_ids       = {};
        bool           self_id_management  = false;
    };

    namespace pool
    {
        template<typename T>
        void load(Pool* pool, u32 max_element_count, bool self_id_management = true);

        void release(Pool* pool);

        bool is_valid(Pool* pool, u32 element_id);  
    
        bool insert_data_with_id(Pool* pool, u32 element_id, void* data = nullptr);

        bool insert_data(Pool* pool, u32& element_id, void* data = nullptr);
    
        template<typename T>
        T* insert_data_with_id(Pool* pool, u32 element_id, const T& data);
    
        template<typename T>
        T* insert_data(Pool* pool, u32& out_id, const T& data = {});

        u32 index_of(Pool* pool, u32 element_id);
    
        void* get_raw_data(Pool* pool, u32 element_id);
    
        template<typename T>
        T* get_data(Pool* pool, u32 element_id);

        SparseSetDeletion delete_data(Pool* pool, u32 element_id);
    
        void resize(Pool* pool, u32 new_max);
    }
}

#include "nit/core/pool.inl"