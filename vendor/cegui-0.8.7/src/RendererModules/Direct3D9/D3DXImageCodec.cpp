/***********************************************************************
    created:    2026
    author:     MTA:SA Team
    purpose:    Direct3D9 D3DX based static ImageCodec module for CEGUI
*************************************************************************/
#include "CEGUI/ImageCodec.h"
#include "CEGUI/System.h"
#include "CEGUI/RendererModules/Direct3D9/Renderer.h"
#include "CEGUI/RendererModules/Direct3D9/Texture.h"
#include <d3dx9.h>

namespace CEGUI
{
class D3DXImageCodec : public ImageCodec
{
public:
    D3DXImageCodec()
        : ImageCodec("CEGUI::D3DXImageCodec - Direct3D9 D3DX based image codec")
    {
        d_supportedFormat = "png tga jpg jpeg bmp dds dib hdr pfm";
    }

    Texture* load(const RawDataContainer& data, Texture* result) override
    {
        if (!result || data.getSize() == 0)
            return nullptr;

        Direct3D9Renderer* pRenderer = static_cast<Direct3D9Renderer*>(System::getSingleton().getRenderer());
        if (!pRenderer)
            return nullptr;

        LPDIRECT3DDEVICE9 pDevice = pRenderer->getDevice();
        if (!pDevice)
            return nullptr;

        LPDIRECT3DTEXTURE9 pTexture = nullptr;
        D3DXIMAGE_INFO     info;

        HRESULT hr = D3DXCreateTextureFromFileInMemoryEx(
            pDevice,
            data.getDataPtr(),
            data.getSize(),
            D3DX_DEFAULT,
            D3DX_DEFAULT,
            1,
            0,
            D3DFMT_UNKNOWN,
            D3DPOOL_MANAGED,
            D3DX_FILTER_NONE,
            D3DX_DEFAULT,
            0,
            &info,
            nullptr,
            &pTexture
        );

        if (FAILED(hr) || !pTexture)
            return nullptr;

        Direct3D9Texture* d3dTex = static_cast<Direct3D9Texture*>(result);
        d3dTex->setDirect3D9Texture(pTexture);
        d3dTex->setOriginalDataSize(Sizef(static_cast<float>(info.Width), static_cast<float>(info.Height)));
        pTexture->Release();

        return result;
    }
};

} // namespace CEGUI

extern "C"
{
CEGUI::ImageCodec* createImageCodec(void)
{
    return CEGUI_NEW_AO CEGUI::D3DXImageCodec();
}

void destroyImageCodec(CEGUI::ImageCodec* imageCodec)
{
    CEGUI_DELETE_AO imageCodec;
}
}
