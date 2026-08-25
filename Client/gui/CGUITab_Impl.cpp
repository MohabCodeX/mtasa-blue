/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto v1.0
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        gui/CGUITab_Impl.cpp
 *  PURPOSE:     Tab widget class
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/

#include "StdInc.h"

CGUITab_Impl::CGUITab_Impl(CGUI_Impl* pGUI, CGUIElement_Impl* pParent, const char* szCaption)
{
    SetManager(pGUI);

    // Get an unique identifier for CEGUI
    char szUnique[CGUI_CHAR_SIZE];
    pGUI->GetUniqueName(szUnique);

    // Create the window and set default settings
    m_pWindow = pGUI->GetWindowManager()->createWindow("DefaultWindow", szUnique);
    m_pWindow->setDestroyedByParent(false);

    m_pWindow->setText(CGUI_Impl::GetUTFString(szCaption));

    // Store the pointer to this CGUI element in the CEGUI element
    m_pWindow->setUserData(reinterpret_cast<void*>(this));

    AddEvents();

    // If a parent is specified, add it to it's children list, if not, add it as a child to the pManager
    if (pParent)
    {
        SetParent(pParent);

        // Adjust the tab button (pParent should be a TabControl!)
#ifdef MTA_USE_CEGUI_NEXT
        reinterpret_cast<CEGUI::TabControl*>(((CGUITabPanel_Impl*)pParent)->m_pWindow)->setTabHeight(CEGUI::UDim(0.0f, 24.0f));
#else
        reinterpret_cast<CEGUI::TabControl*>(((CGUITabPanel_Impl*)pParent)->m_pWindow)->setAbsoluteTabHeight(24.0f);
#endif
    }
    else
    {
        pGUI->AddChild(this);
        SetParent(NULL);
    }
}

CGUITab_Impl::~CGUITab_Impl()
{
    CGUIElement_Impl*  pParent = static_cast<CGUIElement_Impl*>(m_pParent);
    CEGUI::TabControl* pControl = reinterpret_cast<CEGUI::TabControl*>(((CGUITabPanel_Impl*)pParent)->m_pWindow);
    pControl->removeTab(this->GetWindow()->getName());

    DestroyElement();
}

void CGUITab_Impl::SetCaption(const char* szCaption)
{
    m_pWindow->setText(CGUI_Impl::GetUTFString(szCaption));
}

#ifdef MTA_USE_CEGUI_NEXT
class CGUITabControlHelper : public CEGUI::TabControl
{
public:
    using CEGUI::TabControl::getButtonForTabContents;
    using CEGUI::TabControl::performChildWindowLayout;
};
#endif

void CGUITab_Impl::SetVisible(bool bVisible)
{
    CGUIElement_Impl*  pParent = static_cast<CGUIElement_Impl*>(m_pParent);
    CEGUI::TabControl* pControl = reinterpret_cast<CEGUI::TabControl*>(((CGUITabPanel_Impl*)pParent)->m_pWindow);
#ifdef MTA_USE_CEGUI_NEXT
    static_cast<CGUITabControlHelper*>(pControl)->getButtonForTabContents(m_pWindow)->setVisible(bVisible);
    static_cast<CGUITabControlHelper*>(pControl)->performChildWindowLayout();
#else
    pControl->getButtonForTabContents(m_pWindow)->setVisible(bVisible);
    pControl->requestChildWindowLayout();
#endif
}

bool CGUITab_Impl::IsVisible()
{
    CGUIElement_Impl*  pParent = static_cast<CGUIElement_Impl*>(m_pParent);
    CEGUI::TabControl* pControl = reinterpret_cast<CEGUI::TabControl*>(((CGUITabPanel_Impl*)pParent)->m_pWindow);
#ifdef MTA_USE_CEGUI_NEXT
    return static_cast<CGUITabControlHelper*>(pControl)->getButtonForTabContents(m_pWindow)->isVisible();
#else
    return pControl->getButtonForTabContents(m_pWindow)->isVisible();
#endif
}

void CGUITab_Impl::SetEnabled(bool bEnabled)
{
    CGUIElement_Impl*  pParent = static_cast<CGUIElement_Impl*>(m_pParent);
    CEGUI::TabControl* pControl = reinterpret_cast<CEGUI::TabControl*>(((CGUITabPanel_Impl*)pParent)->m_pWindow);
#ifdef MTA_USE_CEGUI_NEXT
    static_cast<CGUITabControlHelper*>(pControl)->getButtonForTabContents(m_pWindow)->setEnabled(bEnabled);
#else
    pControl->getButtonForTabContents(m_pWindow)->setEnabled(bEnabled);
#endif
}

bool CGUITab_Impl::IsEnabled()
{
    CGUIElement_Impl*  pParent = static_cast<CGUIElement_Impl*>(m_pParent);
    CEGUI::TabControl* pControl = reinterpret_cast<CEGUI::TabControl*>(((CGUITabPanel_Impl*)pParent)->m_pWindow);
#ifdef MTA_USE_CEGUI_NEXT
    return !static_cast<CGUITabControlHelper*>(pControl)->getButtonForTabContents(m_pWindow)->isDisabled();
#else
    return !pControl->getButtonForTabContents(m_pWindow)->isDisabled();
#endif
}
