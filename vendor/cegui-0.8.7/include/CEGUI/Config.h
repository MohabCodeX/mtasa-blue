/***********************************************************************
    created:    Mon Jan 10 2011
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2015 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUIConfig_h_
#define _CEGUIConfig_h_

#if !defined(NDEBUG) && !defined(DEBUG)
#   define DEBUG 1
#endif

// String class configurations
#define CEGUI_STRING_CLASS_UNICODE 1
#define CEGUI_STRING_CLASS_STD 2
#define CEGUI_STRING_CLASS_STD_AO 3

#define CEGUI_STRING_CLASS CEGUI_STRING_CLASS_UNICODE

// Default XMLParser
#ifndef CEGUI_DEFAULT_XMLPARSER
#   define CEGUI_DEFAULT_XMLPARSER "TinyXMLParser"
#endif

// FreeType font support
#define CEGUI_HAS_FREETYPE 1

// Default Logger
#define CEGUI_HAS_DEFAULT_LOGGER 1

// Bidirectional text support (Arabic / Hebrew)
#define CEGUI_BIDI_SUPPORT 1
#define CEGUI_USE_MINIBIDI 1

// Static library configuration
#define CEGUI_STATIC 1

#endif  // _CEGUIConfig_h_
