/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto v1.0
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        gui/CGUIListItem_Impl.h
 *  PURPOSE:     List widget item class
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/

#pragma once

#include <gui/CGUIListItem.h>

#ifdef MTA_USE_CEGUI_NEXT
    #include <CEGUI/CEGUI.h>
#else
    #include <CEGUI.h>
#endif

class CGUIStaticImage;
class CGUIStaticImage_Impl;

#ifdef MTA_USE_CEGUI_NEXT
class CGUIListboxNumberItem : public CEGUI::ListboxTextItem
{
public:
    using CEGUI::ListboxTextItem::ListboxTextItem;
    bool operator<(const CEGUI::ListboxItem& rhs) const override { return atoi(getText().c_str()) < atoi(rhs.getText().c_str()); }
    bool operator>(const CEGUI::ListboxItem& rhs) const override { return atoi(getText().c_str()) > atoi(rhs.getText().c_str()); }
};

class CGUIListboxImageItem : public CEGUI::ListboxItem
{
public:
    CGUIListboxImageItem(const CEGUI::Image* image, unsigned int item_id = 0, void* item_data = nullptr, bool disabled = false, bool auto_delete = true)
        : CEGUI::ListboxItem("", item_id, item_data, disabled, auto_delete), d_image(image)
    {
    }

    const CEGUI::Image* getImage() const { return d_image; }
    void                setImage(const CEGUI::Image* image) { d_image = image; }

    CEGUI::Sizef getPixelSize() const override { return d_image ? d_image->getRenderedSize() : CEGUI::Sizef(0.0f, 0.0f); }

    void draw(CEGUI::GeometryBuffer& buffer, const CEGUI::Rectf& targetRect, float alpha, const CEGUI::Rectf* clipper) const override
    {
        if (d_selected && d_selectBrush != 0)
            d_selectBrush->render(buffer, targetRect, clipper, getModulateAlphaColourRect(d_selectCols, alpha));

        if (d_image)
        {
            CEGUI::Rectf finalRect(targetRect);
            finalRect.setSize(d_image->getRenderedSize());
            d_image->render(buffer, finalRect, clipper, getModulateAlphaColourRect(CEGUI::ColourRect(0xFFFFFFFF), alpha));
        }
    }

protected:
    const CEGUI::Image* d_image;
};
#endif

class CGUIListItem_Impl : public CGUIListItem
{
public:
    enum Type
    {
        TextItem = 0,
        NumberItem = 1,
        ImageItem = 2
    };

    CGUIListItem_Impl(const char* szText = "", unsigned int uiType = 0, CGUIStaticImage_Impl* pImage = NULL);
    ~CGUIListItem_Impl();

    std::string GetText() const;
    void        SetText(const char* pszText, const char* pszSortText = NULL);

    void* GetData() const { return m_pData; }
    void  SetData(void* pData, CGUICallback<void, void*> deleteDataCallback = NULL)
    {
        m_pData = pData;
        m_deleteDataCallback = deleteDataCallback;
    }
    void SetData(const char* pszData);

    void SetDisabled(bool bDisabled);
    void SetFont(const char* szFontName);
    void SetImage(CGUIStaticImage* Image);

    bool GetSelectedState();
    void SetSelectedState(bool bState);

    bool GetColor(unsigned char& ucRed, unsigned char& ucGreen, unsigned char& ucBlue, unsigned char& ucAlpha);
    void SetColor(unsigned char ucRed, unsigned char ucGreen, unsigned char ucBlue, unsigned char ucAlpha);

    CEGUI::ListboxItem* GetListItem();

    unsigned int ItemType;

private:
    CEGUI::ListboxItem*       m_pListItem;
    void*                     m_pData;
    std::string               m_strData;
    CGUICallback<void, void*> m_deleteDataCallback;
};
