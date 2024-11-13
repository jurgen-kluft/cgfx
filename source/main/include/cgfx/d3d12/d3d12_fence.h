#ifndef __CGFX_D3D12_FENCE_H__
#define __CGFX_D3D12_FENCE_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
    #pragma once
#endif

#include "cgfx/d3d12/d3d12_header.h"
#include "cgfx/gfx_fence.h"

namespace ncore
{
    namespace ngfx
    {
        class D3D12Device;

        class D3D12Fence : public IGfxFence
        {
        public:
            D3D12Fence(D3D12Device* pDevice, const char* name);
            ~D3D12Fence();

            virtual void* GetHandle() const override { return m_pFence; }
            virtual void  Wait(u64 value) override;
            virtual void  Signal(u64 value) override;

            bool Create();

        private:
            ID3D12Fence* m_pFence = nullptr;
            HANDLE       m_hEvent = nullptr;
        };
    }  // namespace ngfx
}  // namespace ncore
#endif
