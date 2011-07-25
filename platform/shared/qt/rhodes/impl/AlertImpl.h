/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

#ifndef ALERTIMPL_H
#define ALERTIMPL_H

// CAlertParams is a container to pass parameters to UI thread.
class CAlertParams {
public:
   enum CAlertType {DLG_DEFAULT, DLG_CUSTOM, DLG_STATUS};
   struct CAlertButton
   {
       rho::String m_strCaption;
       rho::String m_strID;
       CAlertButton ( const rho::String& strCaption, const rho::String& strID ): m_strCaption(strCaption), m_strID(strID){}
   };

   CAlertParams (rho::String &message)
   {
        m_dlgType  = DLG_DEFAULT;
        m_message  = message;
   }

   CAlertParams (rho::String &title, rho::String &message, rho::String &icon, rho::String &callback, rho::Vector<CAlertButton>& buttons, CAlertType dlgType)
   {
        m_dlgType  = dlgType;
        m_title    = title;
        m_message  = message;
        m_icon     = icon;
        m_callback = callback;
        m_buttons  = buttons;
   }

   int m_dlgType;
   rho::String m_title;
   rho::String m_message;
   rho::String m_icon;
   rho::String m_callback;
   rho::Vector<CAlertButton> m_buttons;
};

#endif // ALERTIMPL_H
