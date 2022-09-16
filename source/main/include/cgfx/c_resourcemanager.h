#include "cgfx/c_handle.h"
#include "cgfx/c_enums.h"

// https://github.com/nvpro-samples/nvpro_core/tree/master/nvvk
// https://www.onlineocr.net/



class Shader;
class BindGroup;
class Texture;
class Buffer;

struct ShaderDesc;
struct BindGroupDesc;
struct TextureDesc;
struct BufferDesc;

class ResourceManager
{
public:
    static ResourceManager* ptr;

    virtual ~ResourceManager() = 0;

    virtual Handle<Shader>    createShader(ShaderDesc& desc)       = 0;
    virtual Handle<BindGroup> createBindGroup(BindGroupDesc& desc) = 0;
    virtual Handle<Texture>   createTexture(TextureDesc& desc)     = 0;
    virtual Handle<Buffer>    createBuffer(BufferDesc& desc)       = 0;

    virtual void deleteShader(Handle<Shader> handle)       = 0;
    virtual void deleteBindGroup(Handle<BindGroup> handle) = 0;
    virtual void deleteTexture(Handle<Texture> handle)     = 0;
    virtual void deleteBuffer(Handle<Buffer> handle)       = 0;
};
