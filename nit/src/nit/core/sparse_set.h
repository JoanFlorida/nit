#pragma once

namespace Nit
{
    struct SparseSet
    {
        static constexpr u32 INVALID_INDEX = U32_MAX;
        
        u32* sparse = nullptr;
        u32* dense  = nullptr;
        u32  count  = 0;
        u32  max    = 0;
    };
    
    //TODO: Resize
    bool IsValid(SparseSet* sparse_set);
    bool IsEmpty(SparseSet* sparse_set);
    bool IsFull(SparseSet* sparse_set);
    void Load(SparseSet* sparse_set, u32 max);
    void Insert(SparseSet* sparse_set, u32 element);
    u32  Search(SparseSet* sparse_set, u32 element);
    void Delete(SparseSet* sparse_set, u32 element);
    void Free(SparseSet* sparse_set);
}