# Resources

## IGfxDevice

There is only one specific device, depending on the OS you are running on. For example, on Windows, there is a `D3D12Device` class that is derived from `IGfxDevice`. 
Currently you can have multiple devices, but only one is active at a time, we should change this to only have one device at compile time.


# GfxResource

Is a base class.

Properties:

- GfxDevice* (Not sure if this is needed since we can only have one backend active at a time)
- Name

## GfxBuffer 

Is a GfxResource, and is used as a base class in the back-ends.

Properties:

- GfxBufferDesc

## GfxCommandList

Is a GfxResource and has a LOT of functions and is used as a base class in the back-ends.

Properties:

- GfxCommandQueue (type)

## GfxDescriptor

Is a GfxResource, and is used as a base class in the back-ends.


## GfxDevice

Is a base class and has a LOT of functions, and is used as a base class in the back-ends.

Properties:

- GfxDeviceDesc desc
- GfxVendor     vendor
- u64           frameID

## GfxFence

Is a GfxResource, and is used as a base class in the back-ends.

## GfxHeap

Is a GfxResource, and is used as a base class in the back-ends.

Properties:

- GfxHeapDesc

## GfxPipelineState

Is a GfxResource, and is used as a base class in the back-ends.

Properties:

- GfxPipelineType type

## GfxRaytracingBLAS

Is a GfxResource, and is used as a base class in the back-ends.

Properties:

- GfxRaytracingBLASDesc

## GfxRaytracingTLAS

Is a GfxResource, and is used as a base class in the back-ends.

Properties:

- GfxRaytracingTLASDesc

## GfxShader

Is a GfxResource, and is used as a base class in the back-ends.

- GfxShaderDesc desc
- u64           hash

## GfxSwapChain

Is a GfxResource, and is used as a base class in the back-ends.

Properties:

- GfxSwapChainDesc

## GfxTexture

Is a GfxResource, and is used as a base class in the back-ends.

Properties:

- GfxTextureDesc


## IGfxDescriptor

This is used in the back-ends as a base class. For example, in DX12, there is a class `D3D12ShaderResourceView` that is derived from `IGfxDescriptor`. 
This is used to create a shader resource view.

There are only 4 type of descriptors:

- Shader Resource View
- Unordered Access View
- Constant Buffer View
- Sampler

They all share the following properties:

- A name (for debugging)
- A handle (GetHandle())
- A heap index (GetHeapIndex())
- One or more specific descriptor structure
