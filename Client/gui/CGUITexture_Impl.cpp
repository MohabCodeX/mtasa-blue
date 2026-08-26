/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto v1.0
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        gui/CGUITexture_Impl.cpp
 *  PURPOSE:     Texture handling class
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/

#include "StdInc.h"

#ifdef MTA_USE_CEGUI_NEXT
    #include <CEGUI/RendererModules/Direct3D9/Texture.h>
#else
    #include <renderers/directx9GUIRenderer/d3d9texture.h>
#endif

CGUITexture_Impl::CGUITexture_Impl(CGUI_Impl* pGUI)
{
    // Save the renderer
    m_pRenderer = pGUI->GetRenderer();

#ifdef MTA_USE_CEGUI_NEXT
    char szUnique[CGUI_CHAR_SIZE];
    do
    {
        pGUI->GetUniqueName(szUnique);
    } while (m_pRenderer->isTextureDefined(szUnique));
    m_pTexture = &m_pRenderer->createTexture(szUnique);
#else
    // Create the texture
    m_pTexture = m_pRenderer->createTexture();
#endif
}

CGUITexture_Impl::~CGUITexture_Impl()
{
    if (m_pTexture)
    {
#ifdef MTA_USE_CEGUI_NEXT
        m_pRenderer->destroyTexture(*m_pTexture);
#else
        m_pRenderer->destroyTexture(m_pTexture);
#endif
    }
}

bool CGUITexture_Impl::LoadFromFile(const char* szFilename)
{
    // Try to load it
    try
    {
        m_pTexture->loadFromFile(szFilename, "");
    }
    catch (CEGUI::Exception)
    {
        return false;
    }
    return true;
}

void CGUITexture_Impl::LoadFromMemory(const void* pBuffer, unsigned int uiWidth, unsigned int uiHeight)
{
#ifdef MTA_USE_CEGUI_NEXT
    m_pTexture->loadFromMemory(pBuffer, CEGUI::Sizef(static_cast<float>(uiWidth), static_cast<float>(uiHeight)), CEGUI::Texture::PixelFormat::PF_RGBA);
#else
    m_pTexture->loadFromMemory(pBuffer, uiWidth, uiHeight);
#endif
}

void CGUITexture_Impl::Clear()
{
#ifdef MTA_USE_CEGUI_NEXT
    if (m_pTexture)
    {
        CEGUI::String strName = m_pTexture->getName();
        m_pRenderer->destroyTexture(*m_pTexture);
        m_pTexture = &m_pRenderer->createTexture(strName);
    }
#else
    // Destroy the previous texture and recreate it (empty)
    m_pRenderer->destroyTexture(m_pTexture);
    m_pTexture = m_pRenderer->createTexture();
#endif
}

CEGUI::Texture* CGUITexture_Impl::GetTexture()
{
    return m_pTexture;
}

void CGUITexture_Impl::SetTexture(CEGUI::Texture* pTexture)
{
    m_pTexture = pTexture;
}

LPDIRECT3DTEXTURE9 CGUITexture_Impl::GetD3DTexture()
{
#ifdef MTA_USE_CEGUI_NEXT
    return static_cast<CEGUI::Direct3D9Texture*>(m_pTexture)->getDirect3D9Texture();
#else
    return reinterpret_cast<CEGUI::DirectX9Texture*>(m_pTexture)->getD3DTexture();
#endif
}

void CGUITexture_Impl::CreateTexture(unsigned int width, unsigned int height)
{
#ifndef MTA_USE_CEGUI_NEXT
    return reinterpret_cast<CEGUI::DirectX9Texture*>(m_pTexture)->createRenderTarget(width, height);
#endif
}
