#include "cgfx/gfx.h"

#include "ccore/c_math.h"

namespace ncore
{
    namespace ngfx
    {
        u32 GetAllocationSize(const texture_desc_t& desc)
        {
            u32 size = 0;

            u32 min_width  = GetFormatBlockWidth(enums::cast<enums::format>(desc.format));
            u32 min_height = GetFormatBlockHeight(enums::cast<enums::format>(desc.format));

            for (u32 layer = 0; layer < desc.array_size; ++layer)
            {
                for (u32 mip = 0; mip < desc.mip_levels; ++mip)
                {
                    u32 width  = math::g_max(desc.width >> mip, min_width);
                    u32 height = math::g_max(desc.height >> mip, min_height);
                    u32 depth  = math::g_max(desc.depth >> mip, 1u);

                    size += GetFormatRowPitch(enums::cast<enums::format>(desc.format), width) * height * depth;
                }
            }

            return size;
        }

        
    }  // namespace ngfx
}  // namespace ncore
