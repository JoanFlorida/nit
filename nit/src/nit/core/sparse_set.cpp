#include "sparse_set.h"

namespace nit
{
    bool sparse::is_valid(SparseSet* sparse_set)
    {
        return sparse_set && sparse_set->max != 0;
    }

    bool sparse::is_empty(SparseSet* sparse_set)
    {
        if (!sparse::is_valid(sparse_set))
        {
            NIT_DEBUGBREAK();
            return false;
        }
        
        return sparse_set->count == 0;
    }

    bool sparse::is_full(SparseSet* sparse_set)
    {
        if (!sparse::is_valid(sparse_set))
        {
            NIT_DEBUGBREAK();
            return false;
        }
        
        return sparse_set->count == sparse_set->max;
    }

    void sparse::load(SparseSet* sparse_set, u32 max)
    {
        if (!sparse_set || max == U32_MAX)
        {
            NIT_DEBUGBREAK();
            return;
        }

        sparse_set->max = max;
        sparse_set->sparse = new u32[max];
        sparse_set->dense  = new u32[max];
        memset(sparse_set->sparse, SparseSet::INVALID, sizeof(u32) * max);
    }
    
    u32 sparse::insert(SparseSet* sparse_set, u32 element)
    {
        if (!sparse::is_valid(sparse_set) || element == SparseSet::INVALID || sparse::test(sparse_set, element))
        {
            NIT_DEBUGBREAK();
            return SparseSet::INVALID;
        }
        
        if (sparse::is_full(sparse_set))
        {
            sparse::resize(sparse_set, sparse_set->max * 2);
        }
        
        u32 next_slot = sparse_set->count;
        sparse_set->sparse[element] = next_slot;
        sparse_set->dense[next_slot] = element;
        ++sparse_set->count;
        return next_slot;
    }

    bool sparse::test(SparseSet* sparse_set, u32 element)
    {
        if (!sparse::is_valid(sparse_set))
        {
            NIT_DEBUGBREAK();
            return false;
        }
        
        return element < sparse_set->max && sparse_set->sparse[element] != SparseSet::INVALID;
    }

    u32 sparse::search(SparseSet* sparse_set, u32 element)
    {
        if (!sparse::is_valid(sparse_set) || element >= sparse_set->max)
        {
            NIT_DEBUGBREAK();
            return SparseSet::INVALID;
        }

        u32 dense_index = sparse_set->sparse[element];
        
        if (dense_index == SparseSet::INVALID)
        {
            return SparseSet::INVALID;
        }
        
        return dense_index;
    }
    
    SparseSetDeletion sparse::remove(SparseSet* sparse_set, u32 element)
    {
        if (!sparse::is_valid(sparse_set) || sparse_set->count == 0 || !sparse::test(sparse_set, element))
        {
            NIT_DEBUGBREAK();
            return { false };
        }
        
        u32 deleted_slot = sparse_set->sparse[element];
        u32 last_slot = sparse_set->count - 1;
        
        sparse_set->sparse[element] = SparseSet::INVALID;
        --sparse_set->count;

        if (deleted_slot == last_slot)
        {
            return { true, deleted_slot, last_slot };
        }
        
        u32 last_element = sparse_set->dense[last_slot];
        sparse_set->dense[deleted_slot] = last_element;
        
        sparse_set->sparse[last_element] = deleted_slot;
        return { true, deleted_slot, last_slot };
    }

    void sparse::resize(SparseSet* sparse_set, u32 new_max)
    {
        if (!sparse::is_valid(sparse_set) || new_max > sparse_set->max)
        {
            NIT_DEBUGBREAK();
            return;
        }
        
        u32* new_sparse = new u32[new_max];
        u32* new_dense  = new u32[new_max];
    
        std::copy_n(sparse_set->sparse, sparse_set->max, new_sparse);
        memset(new_sparse + sparse_set->max, SparseSet::INVALID, (new_max - sparse_set->max) * sizeof(u32));
        
        std::copy_n(sparse_set->dense, sparse_set->max, new_dense);
    
        delete[] sparse_set->sparse;
        delete[] sparse_set->dense;
    
        sparse_set->sparse = new_sparse;
        sparse_set->dense  = new_dense;
        sparse_set->max    = new_max;
    }

    void sparse::release(SparseSet* sparse_set)
    {
        if (!sparse::is_valid(sparse_set))
        {
            NIT_DEBUGBREAK();
            return;
        }
        
        if (sparse_set->sparse)
        {
            delete[] sparse_set->sparse;
            sparse_set->sparse = nullptr;
        }
        if (sparse_set->dense)
        {
            delete[] sparse_set->dense;
            sparse_set->dense = nullptr;
        }
        
        sparse_set->count = sparse_set->max = 0;
    }
}