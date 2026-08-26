/***********************************************************************
    created:    Thu Jul 7 2005
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
 *
 *   Permission is hereby granted, free of charge, to any person obtaining
 *   a copy of this software and associated documentation files (the
 *   "Software"), to deal in the Software without restriction, including
 *   without limitation the rights to use, copy, modify, merge, publish,
 *   distribute, sublicense, and/or sell copies of the Software, and to
 *   permit persons to whom the Software is furnished to do so, subject to
 *   the following conditions:
 *
 *   The above copyright notice and this permission notice shall be
 *   included in all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *   OTHER DEALINGS IN THE SOFTWARE.
 ***************************************************************************/
#include "CEGUI/WindowRendererSets/Core/MultiLineEditbox.h"
#include "CEGUI/falagard/WidgetLookManager.h"
#include "CEGUI/falagard/WidgetLookFeel.h"
#include "CEGUI/WindowManager.h"
#include "CEGUI/widgets/Scrollbar.h"
#include "CEGUI/PropertyHelper.h"
#include "CEGUI/Image.h"
#include "CEGUI/BidiVisualMapping.h"
#include "CEGUI/TplWindowRendererProperty.h"

// Start of CEGUI namespace section
namespace CEGUI
{

    const String FalagardMultiLineEditbox::TypeName("Core/MultiLineEditbox");

    const String FalagardMultiLineEditbox::UnselectedTextColourPropertyName("NormalTextColour");
    const String FalagardMultiLineEditbox::SelectedTextColourPropertyName("SelectedTextColour");
    const String FalagardMultiLineEditbox::ActiveSelectionColourPropertyName("ActiveSelectionColour");
    const String FalagardMultiLineEditbox::InactiveSelectionColourPropertyName("InactiveSelectionColour");
    const float  FalagardMultiLineEditbox::DefaultCaretBlinkTimeout(0.66f);

    FalagardMultiLineEditbox::FalagardMultiLineEditbox(const String& type)
        : MultiLineEditboxWindowRenderer(type), d_blinkCaret(false), d_caretBlinkTimeout(DefaultCaretBlinkTimeout), d_caretBlinkElapsed(0.0f), d_showCaret(true)
    {
        CEGUI_DEFINE_WINDOW_RENDERER_PROPERTY(FalagardMultiLineEditbox, bool, "BlinkCaret",
                                              "Property to get/set whether the Editbox caret should blink.  "
                                              "Value is either \"true\" or \"false\".",
                                              &FalagardMultiLineEditbox::setCaretBlinkEnabled, &FalagardMultiLineEditbox::isCaretBlinkEnabled, false);
        CEGUI_DEFINE_WINDOW_RENDERER_PROPERTY(FalagardMultiLineEditbox, float, "BlinkCaretTimeout",
                                              "Property to get/set the caret blink timeout / speed.  "
                                              "Value is a float value indicating the timeout in seconds.",
                                              &FalagardMultiLineEditbox::setCaretBlinkTimeout, &FalagardMultiLineEditbox::getCaretBlinkTimeout, 0.66f);
    }

    Rectf FalagardMultiLineEditbox::getTextRenderArea(void) const
    {
        MultiLineEditbox*     w = (MultiLineEditbox*)d_window;
        const WidgetLookFeel& wlf = getLookNFeel();
        bool                  v_visible = w->getVertScrollbar()->isVisible();
        bool                  h_visible = w->getHorzScrollbar()->isVisible();

        // if either of the scrollbars are visible, we might want to use another text rendering area
        if (v_visible || h_visible)
        {
            String area_name("TextArea");

            if (h_visible)
            {
                area_name += "H";
            }
            if (v_visible)
            {
                area_name += "V";
            }
            area_name += "Scroll";

            if (wlf.isNamedAreaDefined(area_name))
            {
                return wlf.getNamedArea(area_name).getArea().getPixelRect(*w);
            }
        }

        // default to plain TextArea
        if (wlf.isNamedAreaDefined("TextArea"))
            return wlf.getNamedArea("TextArea").getArea().getPixelRect(*w);
        if (wlf.isNamedAreaDefined("WithFrameTextRenderArea"))
            return wlf.getNamedArea("WithFrameTextRenderArea").getArea().getPixelRect(*w);
        if (wlf.isNamedAreaDefined("TextRenderArea"))
            return wlf.getNamedArea("TextRenderArea").getArea().getPixelRect(*w);

        return Rectf(Vector2f(0.0f, 0.0f), w->getPixelSize());
    }

    void FalagardMultiLineEditbox::cacheEditboxBaseImagery()
    {
        MultiLineEditbox* w = (MultiLineEditbox*)d_window;

        // get WidgetLookFeel for the assigned look.
        const WidgetLookFeel& wlf = getLookNFeel();
        String                state_name = w->isEffectiveDisabled() ? "Disabled" : (w->isReadOnly() ? "ReadOnly" : "Enabled");
        if (wlf.isStateImageryPresent(state_name))
            wlf.getStateImagery(state_name).render(*w);
        else if (wlf.isStateImageryPresent("Enabled"))
            wlf.getStateImagery("Enabled").render(*w);
    }

    void FalagardMultiLineEditbox::cacheCaretImagery(const Rectf& textArea)
    {
        MultiLineEditbox* w = (MultiLineEditbox*)d_window;
        const Font*       fnt = w->getFont();

        // require a font so that we can calculate caret position.
        if (fnt)
        {
            // get line that caret is in
            size_t caretLine = w->getLineNumberFromIndex(w->getCaretIndex());

            const MultiLineEditbox::LineList& d_lines = w->getFormattedLines();

            // if caret line is valid.
            if (caretLine < d_lines.size())
            {
                // calculate pixel offsets to where caret should be drawn
                size_t caretLineIdx = w->getCaretIndex() - d_lines[caretLine].d_startIdx;
                float  ypos = caretLine * fnt->getLineSpacing();

                float xpos = 0.0f;
#ifdef CEGUI_BIDI_SUPPORT
                const BidiVisualMapping* bvm = w->getBidiVisualMapping();
                String                   logicalLine = w->getText().substr(d_lines[caretLine].d_startIdx, d_lines[caretLine].d_length);
                while (!logicalLine.empty() && (logicalLine[logicalLine.length() - 1] == '\n' || logicalLine[logicalLine.length() - 1] == '\r'))
                    logicalLine.resize(logicalLine.length() - 1);

                if (bvm && !logicalLine.empty())
                {
                    String                          visualLine;
                    BidiVisualMapping::StrIndexList l2v, v2l;
                    bvm->reorderFromLogicalToVisual(logicalLine, visualLine, l2v, v2l);

                    size_t visualCaretIdx = 0;
                    if (caretLineIdx == 0)
                    {
                        BidiCharType charType = bvm->getBidiCharType(logicalLine[0]);
                        if (charType == BCT_NEUTRAL)
                        {
                            for (size_t nextIdx = 1; nextIdx < logicalLine.length(); ++nextIdx)
                            {
                                BidiCharType nextType = bvm->getBidiCharType(logicalLine[nextIdx]);
                                if (nextType != BCT_NEUTRAL)
                                {
                                    charType = nextType;
                                    break;
                                }
                            }
                        }
                        visualCaretIdx = (charType == BCT_RIGHT_TO_LEFT) ? (l2v.empty() ? visualLine.length() : (l2v[0] + 1)) : (l2v.empty() ? 0 : l2v[0]);
                    }
                    else
                    {
                        size_t prevLogicalIdx = caretLineIdx - 1;
                        if (prevLogicalIdx < l2v.size())
                        {
                            size_t       v = l2v[prevLogicalIdx];
                            BidiCharType charType = bvm->getBidiCharType(logicalLine[prevLogicalIdx]);
                            if (charType == BCT_NEUTRAL)
                            {
                                for (int p = static_cast<int>(prevLogicalIdx) - 1; p >= 0; --p)
                                {
                                    BidiCharType prevStrongType = bvm->getBidiCharType(logicalLine[p]);
                                    if (prevStrongType != BCT_NEUTRAL)
                                    {
                                        charType = prevStrongType;
                                        break;
                                    }
                                }
                            }
                            visualCaretIdx = (charType == BCT_RIGHT_TO_LEFT) ? v : (v + 1);
                        }
                        else
                        {
                            visualCaretIdx = visualLine.length();
                        }
                    }
                    xpos = fnt->getTextAdvance(visualLine.substr(0, visualCaretIdx));
                }
                else
                {
                    xpos = fnt->getTextAdvance(w->getText().substr(d_lines[caretLine].d_startIdx, caretLineIdx));
                }
#else
                xpos = fnt->getTextAdvance(w->getText().substr(d_lines[caretLine].d_startIdx, caretLineIdx));
#endif

                // get WidgetLookFeel for the assigned look.
                const WidgetLookFeel& wlf = getLookNFeel();
                // get caret imagery
                const ImagerySection* caretImagery = nullptr;
                if (wlf.isImagerySectionDefined("Caret"))
                    caretImagery = &wlf.getImagerySection("Caret");
                else if (wlf.isImagerySectionDefined("Carat"))
                    caretImagery = &wlf.getImagerySection("Carat");

                if (caretImagery)
                {
                    // calculate finat destination area for caret
                    Rectf caretArea;
                    caretArea.left(textArea.left() + xpos);
                    caretArea.top(textArea.top() + ypos);
                    caretArea.setWidth(caretImagery->getBoundingRect(*w).getSize().d_width);
                    caretArea.setHeight(fnt->getLineSpacing());
                    caretArea.offset(Vector2f(-w->getHorzScrollbar()->getScrollPosition(), -w->getVertScrollbar()->getScrollPosition()));

                    // cache the caret image for rendering.
                    caretImagery->render(*w, caretArea, 0, &textArea);
                }
            }
        }
    }

    void FalagardMultiLineEditbox::render()
    {
        MultiLineEditbox* w = (MultiLineEditbox*)d_window;
        // render general frame and stuff before we handle the text itself
        cacheEditboxBaseImagery();

        // Render edit box text
        Rectf textarea(getTextRenderArea());
        cacheTextLines(textarea);

        // draw caret
        if ((w->hasInputFocus() && !w->isReadOnly()) && (!d_blinkCaret || d_showCaret))
            cacheCaretImagery(textarea);
    }

    void FalagardMultiLineEditbox::cacheTextLines(const Rectf& dest_area)
    {
        MultiLineEditbox* w = (MultiLineEditbox*)d_window;
        Rectf             drawArea(dest_area);
        float             vertScrollPos = w->getVertScrollbar()->getScrollPosition();
        drawArea.offset(Vector2f(-w->getHorzScrollbar()->getScrollPosition(), -vertScrollPos));

        const Font* fnt = w->getFont();
        if (!fnt)
            return;

        ColourRect  colours;
        const float alpha = w->getEffectiveAlpha();
        ColourRect  normalTextCol;
        setColourRectToUnselectedTextColour(normalTextCol);
        normalTextCol.modulateAlpha(alpha);
        ColourRect selectTextCol;
        setColourRectToSelectedTextColour(selectTextCol);
        selectTextCol.modulateAlpha(alpha);
        ColourRect selectBrushCol;
        w->hasInputFocus() ? setColourRectToActiveSelectionColour(selectBrushCol) : setColourRectToInactiveSelectionColour(selectBrushCol);
        selectBrushCol.modulateAlpha(alpha);

        const MultiLineEditbox::LineList& d_lines = w->getFormattedLines();
        const size_t                      numLines = d_lines.size();

        size_t sidx = static_cast<size_t>(vertScrollPos / fnt->getLineSpacing());
        size_t eidx = 1 + sidx + static_cast<size_t>(dest_area.getHeight() / fnt->getLineSpacing());
        eidx = ceguimin(eidx, numLines);
        drawArea.d_min.d_y += fnt->getLineSpacing() * static_cast<float>(sidx);

        const BidiVisualMapping* bvm = nullptr;
#ifdef CEGUI_BIDI_SUPPORT
        bvm = w->getBidiVisualMapping();
#endif

        const size_t selStart = w->getSelectionStartIndex();
        const size_t selEnd = w->getSelectionEndIndex();
        const bool   hasSelection = (w->getSelectionLength() > 0) && (w->getSelectionBrushImage() != nullptr);

        for (size_t i = sidx; i < eidx; ++i)
        {
            Rectf                             lineRect(drawArea);
            const MultiLineEditbox::LineInfo& currLine = d_lines[i];

            String logicalLine = w->getText().substr(currLine.d_startIdx, currLine.d_length);
            while (!logicalLine.empty() && (logicalLine[logicalLine.length() - 1] == '\n' || logicalLine[logicalLine.length() - 1] == '\r'))
                logicalLine.resize(logicalLine.length() - 1);

            const float old_top = lineRect.top();
            lineRect.d_min.d_y += (fnt->getLineSpacing() - fnt->getFontHeight()) * 0.5f;

#ifdef CEGUI_BIDI_SUPPORT
            if (bvm && !logicalLine.empty())
            {
                String                          visualLine;
                BidiVisualMapping::StrIndexList l2v, v2l;
                bvm->reorderFromLogicalToVisual(logicalLine, visualLine, l2v, v2l);

                float char_x = lineRect.d_min.d_x;
                for (size_t c = 0; c < visualLine.length(); ++c)
                {
                    String currChar = visualLine.substr(c, 1);
                    float  charAdv = fnt->getTextAdvance(currChar);

                    size_t log_in_line = (c < v2l.size()) ? v2l[c] : c;
                    size_t global_log = currLine.d_startIdx + log_in_line;

                    bool highlighted = hasSelection && (global_log >= selStart) && (global_log < selEnd);

                    if (highlighted)
                    {
                        Rectf hlarea(lineRect);
                        hlarea.top(old_top);
                        hlarea.bottom(old_top + fnt->getLineSpacing());
                        hlarea.left(char_x);
                        hlarea.right(char_x + charAdv);
                        w->getSelectionBrushImage()->render(w->getGeometryBuffer(), hlarea, &dest_area, selectBrushCol);

                        colours = selectTextCol;
                    }
                    else
                    {
                        colours = normalTextCol;
                    }

                    Vector2f charPos(char_x, lineRect.d_min.d_y);
                    fnt->drawText(w->getGeometryBuffer(), currChar, charPos, &dest_area, colours);
                    char_x += charAdv;
                }
            }
            else
#endif
            {
                if (!hasSelection || (currLine.d_startIdx >= selEnd) || ((currLine.d_startIdx + currLine.d_length) <= selStart))
                {
                    colours = normalTextCol;
                    fnt->drawText(w->getGeometryBuffer(), logicalLine, lineRect.getPosition(), &dest_area, colours);
                }
                else
                {
                    size_t line_sel_start = (selStart > currLine.d_startIdx) ? (selStart - currLine.d_startIdx) : 0;
                    size_t line_sel_end = (selEnd < currLine.d_startIdx + logicalLine.length()) ? (selEnd - currLine.d_startIdx) : logicalLine.length();

                    float cur_x = lineRect.d_min.d_x;

                    if (line_sel_start > 0)
                    {
                        String pre = logicalLine.substr(0, line_sel_start);
                        colours = normalTextCol;
                        fnt->drawText(w->getGeometryBuffer(), pre, Vector2f(cur_x, lineRect.d_min.d_y), &dest_area, colours);
                        cur_x += fnt->getTextAdvance(pre);
                    }

                    if (line_sel_end > line_sel_start)
                    {
                        String sel = logicalLine.substr(line_sel_start, line_sel_end - line_sel_start);
                        float  selW = fnt->getTextAdvance(sel);

                        Rectf hlarea(lineRect);
                        hlarea.top(old_top);
                        hlarea.bottom(old_top + fnt->getLineSpacing());
                        hlarea.left(cur_x);
                        hlarea.right(cur_x + selW);
                        w->getSelectionBrushImage()->render(w->getGeometryBuffer(), hlarea, &dest_area, selectBrushCol);

                        colours = selectTextCol;
                        fnt->drawText(w->getGeometryBuffer(), sel, Vector2f(cur_x, lineRect.d_min.d_y), &dest_area, colours);
                        cur_x += selW;
                    }

                    if (line_sel_end < logicalLine.length())
                    {
                        String post = logicalLine.substr(line_sel_end);
                        colours = normalTextCol;
                        fnt->drawText(w->getGeometryBuffer(), post, Vector2f(cur_x, lineRect.d_min.d_y), &dest_area, colours);
                    }
                }
            }

            drawArea.d_min.d_y += fnt->getLineSpacing();
        }
    }

    //----------------------------------------------------------------------------//
    void FalagardMultiLineEditbox::setColourRectToUnselectedTextColour(ColourRect& colour_rect) const
    {
        setColourRectToOptionalPropertyColour(UnselectedTextColourPropertyName, colour_rect);
    }

    //----------------------------------------------------------------------------//
    void FalagardMultiLineEditbox::setColourRectToSelectedTextColour(ColourRect& colour_rect) const
    {
        setColourRectToOptionalPropertyColour(SelectedTextColourPropertyName, colour_rect);
    }

    //----------------------------------------------------------------------------//
    void FalagardMultiLineEditbox::setColourRectToActiveSelectionColour(ColourRect& colour_rect) const
    {
        setColourRectToOptionalPropertyColour(ActiveSelectionColourPropertyName, colour_rect);
    }

    //----------------------------------------------------------------------------//
    void FalagardMultiLineEditbox::setColourRectToInactiveSelectionColour(ColourRect& colour_rect) const
    {
        setColourRectToOptionalPropertyColour(InactiveSelectionColourPropertyName, colour_rect);
    }

    //----------------------------------------------------------------------------//
    void FalagardMultiLineEditbox::setColourRectToOptionalPropertyColour(const String& propertyName, ColourRect& colour_rect) const
    {
        if (d_window->isPropertyPresent(propertyName))
            colour_rect = d_window->getProperty<ColourRect>(propertyName);
        else
            colour_rect.setColours(0);
    }

    //----------------------------------------------------------------------------//
    void FalagardMultiLineEditbox::update(float elapsed)
    {
        // do base class stuff
        WindowRenderer::update(elapsed);

        // only do the update if we absolutely have to
        if (d_blinkCaret && !static_cast<MultiLineEditbox*>(d_window)->isReadOnly() && static_cast<MultiLineEditbox*>(d_window)->hasInputFocus())
        {
            d_caretBlinkElapsed += elapsed;

            if (d_caretBlinkElapsed > d_caretBlinkTimeout)
            {
                d_caretBlinkElapsed = 0.0f;
                d_showCaret ^= true;
                // state changed, so need a redraw
                d_window->invalidate();
            }
        }
    }

    //----------------------------------------------------------------------------//
    bool FalagardMultiLineEditbox::isCaretBlinkEnabled() const
    {
        return d_blinkCaret;
    }

    //----------------------------------------------------------------------------//
    float FalagardMultiLineEditbox::getCaretBlinkTimeout() const
    {
        return d_caretBlinkTimeout;
    }

    //----------------------------------------------------------------------------//
    void FalagardMultiLineEditbox::setCaretBlinkEnabled(bool enable)
    {
        d_blinkCaret = enable;
    }

    //----------------------------------------------------------------------------//
    void FalagardMultiLineEditbox::setCaretBlinkTimeout(float seconds)
    {
        d_caretBlinkTimeout = seconds;
    }

    //----------------------------------------------------------------------------//
    bool FalagardMultiLineEditbox::handleFontRenderSizeChange(const Font* const font)
    {
        const bool res = WindowRenderer::handleFontRenderSizeChange(font);

        if (d_window->getFont() == font)
        {
            d_window->invalidate();
            static_cast<MultiLineEditbox*>(d_window)->formatText(true);
            return true;
        }

        return res;
    }

}  // End of  CEGUI namespace section
