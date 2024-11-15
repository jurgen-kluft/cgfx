#ifndef __CGFX_D3D12_SHADER_H__
#define __CGFX_D3D12_SHADER_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
    #pragma once
#endif

#include "cgfx/d3d12/d3d12_header.h"
#include "cgfx/gfx_shader.h"

namespace ncore
{
    namespace ngfx
    {
        class D3D12Device;

        class D3D12Shader : public shader_t
        {
        public:
            D3D12Shader(D3D12Device* pDevice, const shader_desc_t& desc, const char* name);

            virtual void* GetHandle() const override { return nullptr; }
            virtual bool  Create(byte* data_ptr, u32 data_len) override;

            D3D12_SHADER_BYTECODE GetByteCode() const { return {m_data.data, m_data.size}; }

        private:
            template <typename T>
            struct vector_t
            {
				T* data = nullptr;
				u32 size = 0;
				u32 capacity = 0;
			};

            vector_t<u8> m_data;
        };
    }  // namespace ngfx
}  // namespace ncore
#endif
