#include "cgfx/c_handle.h"
#include "cgfx/c_enums.h"

// https://github.com/nvpro-samples/nvpro_core/tree/master/nvvk
// https://www.onlineocr.net/

namespace ncore
{
    namespace ngfx
    {
        class shader_t;
        class bindgroup_t;
        class texture_t;
        class buffer_t;

        struct shaderdesc_t;
        struct bindgroupdesc_t;
        struct texturedesc_t;
        struct bufferdesc_t;

        class resourcemanager_t
        {
        public:
            static resourcemanager_t* ptr;

            virtual ~resourcemanager_t() = 0;

            virtual handle_t<shader_t>    createShader(shaderdesc_t& desc)       = 0;
            virtual handle_t<bindgroup_t> createBindgroup(bindgroupdesc_t& desc) = 0;
            virtual handle_t<texture_t>   createTexture(texturedesc_t& desc)     = 0;
            virtual handle_t<buffer_t>    createBuffer(bufferdesc_t& desc)       = 0;

            virtual void deleteShader(handle_t<shader_t> handle)       = 0;
            virtual void deleteBindgroup(handle_t<bindgroup_t> handle) = 0;
            virtual void deleteTexture(handle_t<texture_t> handle)     = 0;
            virtual void deleteBuffer(handle_t<buffer_t> handle)       = 0;
        };

    } // namespace ngfx
} // namespace ncore