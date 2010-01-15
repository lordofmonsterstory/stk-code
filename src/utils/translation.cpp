//  $Id: transation.cpp 839 2006-10-24 00:01:56Z hiker $
//
//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2006, 2007, 2008 Joerg Henrichs
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 3
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.


// Note: the irrlicht include is only here (and esp. before including 
//       translation.hpp, which contradicts our style rule) to avoid the 
//        warning message  "  'swprintf' : macro redefinition"
//       This happens if libintl.h is included before irrlicht.h (since
//       both files redefine swprintf).

#include "utils/translation.hpp"

#include <assert.h>
#include <locale.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>

#include "irrlicht.h"

#include "io/file_manager.hpp"
#include "utils/constants.hpp"

// set to 1 to debug i18n
#define TRANSLATE_VERBOSE 0

Translations* translations=NULL;
bool remove_bom = false;

// ----------------------------------------------------------------------------
Translations::Translations()
{ 
#ifdef ENABLE_NLS
    // LC_ALL does not work, sscanf will then not always be able
    // to scan for example: s=-1.1,-2.3,-3.3 correctly, which is
    // used in driveline files.
#if defined(WIN32) && !defined(__CYGWIN__)
    // Windows does not have LC_MESSAGES
    setlocale(LC_CTYPE,    "");
#else
    setlocale(LC_MESSAGES, "");
#endif
    bindtextdomain (PACKAGE, file_manager->getTranslationDir().c_str());
    if (sizeof(wchar_t) == 4)
    {
        // FIXME: will probably not work on PPC maccs
        if (IS_LITTLE_ENDIAN) bind_textdomain_codeset(PACKAGE, "UTF-32LE");
        else                  bind_textdomain_codeset(PACKAGE, "UTF-32BE");
    }
    else if (sizeof(wchar_t) == 2)
    {
        bind_textdomain_codeset(PACKAGE, "UTF-16LE");
    }
    else
    {
        fprintf(stderr, "Your wchar_t is neither 2 byte-long nor 4. What now??\n");
        exit(1);
    }
    
    textdomain (PACKAGE);
#endif
        
}   // Translations
// ----------------------------------------------------------------------------
const wchar_t* Translations::w_gettext(const char* original)
{
    if (original[0] == '\0') return L"";
    
#if TRANSLATE_VERBOSE
    #if ENABLE_NLS
    std::cout << "Translating " << original << "\n";
    #else
    std::cout << "NOT Translating " << original << "\n";
    #endif
#endif
    
#if ENABLE_NLS
    const char* original_t = gettext(original);
#else
    m_converted_string = core::stringw(original);
    return m_converted_string.c_str();
#endif

    /*
    std::cout << "--> original_t==original ? " << (original_t==original) << std::endl;
    int zeros = 0;
    for (int n=0;; n+=1)
    {
        std::cout << original_t[n] << " (" << (unsigned)(original_t[n]) << ")\n";
        if (original_t[n] == 0)
        {
            zeros++;
            if (zeros >= 4) break;
        }
        else
        {
            zeros = 0;
        }
    }*/
    
    if(original_t==original)
    {
        m_converted_string = core::stringw(original);
        
#if TRANSLATE_VERBOSE
        std::wcout << L"  translation : " << m_converted_string.c_str() << std::endl;
#endif
        return m_converted_string.c_str();
    }
    
    // print
    //for (int n=0;; n+=4)
    
    wchar_t* out_ptr = (wchar_t*)original_t;
    if (remove_bom) out_ptr++;
#if TRANSLATE_VERBOSE
    std::wcout << L"  translation : " << out_ptr << std::endl;
#endif
    return out_ptr;

}
