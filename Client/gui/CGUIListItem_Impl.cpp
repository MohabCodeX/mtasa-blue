/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto v1.0
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        gui/CGUIListItem_Impl.cpp
 *  PURPOSE:     List widget item class
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/

#include "StdInc.h"

CGUIListItem_Impl::CGUIListItem_Impl(const char* szText, unsigned int uiType, CGUIStaticImage_Impl* pImage)
{
    ItemType = uiType;

    // Create the requested list item type
    switch (uiType)
    {
        case TextItem:
#ifdef MTA_USE_CEGUI_NEXT
            m_pListItem = new CGUIListboxTextItem(CGUI_Impl::GetUTFString(szText));
#else
            m_pListItem = new CEGUI::ListboxTextItem(CGUI_Impl::GetUTFString(szText));
#endif
            break;
        case ImageItem:
#ifdef MTA_USE_CEGUI_NEXT
            m_pListItem = new CGUIListboxImageItem(pImage ? pImage->GetDirectImage() : NULL);
#else
            m_pListItem = new CEGUI::ListboxImageItem(pImage ? pImage->GetDirectImage() : NULL);
#endif
            break;
        case NumberItem:
#ifdef MTA_USE_CEGUI_NEXT
            m_pListItem = new CGUIListboxNumberItem(CGUI_Impl::GetUTFString(szText));
#else
            m_pListItem = new CEGUI::ListboxNumberItem(CGUI_Impl::GetUTFString(szText));
#endif
            break;
    }

    if (m_pListItem)
    {
        // Set flags and properties
        m_pListItem->setAutoDeleted(false);
#ifdef MTA_USE_CEGUI_NEXT
        m_pListItem->setSelectionBrushImage("CGUI-Images/ListboxSelectionBrush");
#else
        m_pListItem->setSelectionBrushImage("CGUI-Images", "ListboxSelectionBrush");
#endif
    }

    m_pData = NULL;
}

CGUIListItem_Impl::~CGUIListItem_Impl()
{
    if (m_deleteDataCallback)
        m_deleteDataCallback(m_pData);
    delete m_pListItem;
}

void CGUIListItem_Impl::SetDisabled(bool bDisabled)
{
    reinterpret_cast<CEGUI::ListboxItem*>(m_pListItem)->setDisabled(bDisabled);
}

void CGUIListItem_Impl::SetFont(const char* szFontName)
{
    if (szFontName)
        reinterpret_cast<CEGUI::ListboxTextItem*>(m_pListItem)->setFont(CEGUI::String(szFontName));
}

void CGUIListItem_Impl::SetText(const char* pszText, const char* pszSortText)
{
#ifdef MTA_USE_CEGUI_NEXT
    m_pListItem->setText(CGUI_Impl::GetUTFString(pszText));
    if (ItemType == TextItem || ItemType == NumberItem)
    {
        reinterpret_cast<CGUIListboxTextItem*>(m_pListItem)->setSortText(pszSortText ? CGUI_Impl::GetUTFString(pszSortText) : "");
    }
#else
    m_pListItem->setText(CGUI_Impl::GetUTFString(pszText), pszSortText);
#endif
}

void CGUIListItem_Impl::SetData(const char* pszData)
{
    if (pszData)
    {
        m_strData = pszData;
        m_pData = (void*)m_strData.c_str();
    }
    else
    {
        m_pData = NULL;
    }
}

void CGUIListItem_Impl::SetImage(CGUIStaticImage* pImage)
{
    if (ItemType == ImageItem)
    {
        CGUIStaticImage_Impl* pImageImpl = (CGUIStaticImage_Impl*)pImage;
#ifdef MTA_USE_CEGUI_NEXT
        reinterpret_cast<CGUIListboxImageItem*>(m_pListItem)->setImage(pImageImpl ? pImageImpl->GetDirectImage() : NULL);
#else
        reinterpret_cast<CEGUI::ListboxImageItem*>(m_pListItem)->setImage(pImageImpl ? pImageImpl->GetDirectImage() : NULL);
#endif
    }
}

std::string CGUIListItem_Impl::GetText() const
{
    return CGUI_Impl::GetUTFString(m_pListItem->getText().c_str()).c_str();
}

CEGUI::ListboxItem* CGUIListItem_Impl::GetListItem()
{
    return m_pListItem;
}

bool CGUIListItem_Impl::GetSelectedState()
{
    return m_pListItem->isSelected();
}

void CGUIListItem_Impl::SetSelectedState(bool bState)
{
    m_pListItem->setSelected(bState);
}

void CGUIListItem_Impl::SetColor(unsigned char ucRed, unsigned char ucGreen, unsigned char ucBlue, unsigned char ucAlpha)
{
#ifdef MTA_USE_CEGUI_NEXT
    if (ItemType == TextItem || ItemType == NumberItem)
    {
        reinterpret_cast<CEGUI::ListboxTextItem*>(m_pListItem)
            ->setTextColours(CEGUI::Colour((float)ucRed / 255.0f, (float)ucGreen / 255.0f, (float)ucBlue / 255.0f, (float)ucAlpha / 255.0f));
    }
#else
    if (ItemType == TextItem)
    {
        reinterpret_cast<CEGUI::ListboxTextItem*>(m_pListItem)
            ->setTextColours(CEGUI::colour((float)ucRed / 255.0f, (float)ucGreen / 255.0f, (float)ucBlue / 255.0f, (float)ucAlpha / 255.0f));
    }
    else if (ItemType == NumberItem)
    {
        reinterpret_cast<CEGUI::ListboxNumberItem*>(m_pListItem)
            ->setTextColours(CEGUI::colour((float)ucRed / 255.0f, (float)ucGreen / 255.0f, (float)ucBlue / 255.0f, (float)ucAlpha / 255.0f));
    }
#endif
}

bool CGUIListItem_Impl::GetColor(unsigned char& ucRed, unsigned char& ucGreen, unsigned char& ucBlue, unsigned char& ucAlpha)
{
#ifdef MTA_USE_CEGUI_NEXT
    if (ItemType == TextItem || ItemType == NumberItem)
    {
        CEGUI::Colour color = reinterpret_cast<CEGUI::ListboxTextItem*>(m_pListItem)->getTextColours().d_top_left;
        ucRed = static_cast<unsigned char>(color.getRed() * 255);
        ucGreen = static_cast<unsigned char>(color.getGreen() * 255);
        ucBlue = static_cast<unsigned char>(color.getBlue() * 255);
        ucAlpha = static_cast<unsigned char>(color.getAlpha() * 255);
        return true;
    }
#else
    if (ItemType == TextItem)
    {
        CEGUI::colour color = reinterpret_cast<CEGUI::ListboxTextItem*>(m_pListItem)->getTextColours().d_top_left;
        ucRed = static_cast<unsigned char>(color.getRed() * 255);
        ucGreen = static_cast<unsigned char>(color.getGreen() * 255);
        ucBlue = static_cast<unsigned char>(color.getBlue() * 255);
        ucAlpha = static_cast<unsigned char>(color.getAlpha() * 255);
        return true;
    }
    else if (ItemType == NumberItem)
    {
        CEGUI::colour color = reinterpret_cast<CEGUI::ListboxNumberItem*>(m_pListItem)->getTextColours().d_top_left;
        ucRed = static_cast<unsigned char>(color.getRed() * 255);
        ucGreen = static_cast<unsigned char>(color.getGreen() * 255);
        ucBlue = static_cast<unsigned char>(color.getBlue() * 255);
        ucAlpha = static_cast<unsigned char>(color.getAlpha() * 255);
        return true;
    }
#endif
    return false;
}
