#ifndef __CGFX_D3D12_BUFFER_H__
#define __CGFX_D3D12_BUFFER_H__

#include "cgfx/d3d12/d3d12_header.h"
#include "cgfx/gfx_buffer.h"

namespace ncore
{
    namespace ngfx
    {
        class D3D12Device;
        class D3D12Heap;

        namespace D3D12MA
        {
            class Allocation;
        }

        class D3D12Buffer : public IGfxBuffer
        {
        public:
            D3D12Buffer(D3D12Device* pDevice, const GfxBufferDesc& desc, const eastl::string& name);
            ~D3D12Buffer();

            virtual void* GetHandle() const override { return m_pBuffer; }
            virtual void* GetCpuAddress() override;
            virtual u64   GetGpuAddress() override;
            virtual u32   GetRequiredStagingBufferSize() const override;

            bool Create();

        private:
            ID3D12Resource*      m_pBuffer     = nullptr;
            D3D12MA::Allocation* m_pAllocation = nullptr;
            void*                m_pCpuAddress = nullptr;
        };
    }  // namespace ngfx
}  // namespace ncore
#endif
