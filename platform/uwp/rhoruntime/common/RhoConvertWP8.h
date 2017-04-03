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

#pragma once

#include "common/RhoStd.h"

namespace rho
{
namespace common
{

extern "C" {
::Platform::String^ convertStringCToWP8(const char* str);
::Platform::String^ convertStringToWP8(const rho::String& str);
::Platform::String^ convertStringWToWP8(const rho::StringW& str);
::Windows::Foundation::Collections::IVectorView<Platform::String^>^ convertArrayToWP8(const rho::Vector<rho::String>& arr);
::Windows::Foundation::Collections::IMapView<Platform::String^, Platform::String^>^ convertHashToWP8(const rho::Hashtable<rho::String, rho::String>& hash);
}

rho::String convertStringAFromWP8(::Platform::String^ str);
rho::StringW convertStringWFromWP8(::Platform::String^ str);
rho::Vector<rho::String> convertArrayFromWP8(::Windows::Foundation::Collections::IVectorView<Platform::String^>^ arr);
rho::Vector<rho::Hashtable<rho::String, rho::String> > convertArrayOfHashesFromWP8(::Windows::Foundation::Collections::IVectorView<::Windows::Foundation::Collections::IMapView<Platform::String^, Platform::String^>^>^ arr);
rho::Hashtable<rho::String, rho::String> convertHashFromWP8(::Windows::Foundation::Collections::IMapView<Platform::String^, Platform::String^>^ hash);

rho::Hashtable<rho::String, rho::Vector<rho::String> > convertHashOfArraysFromWP8(::Windows::Foundation::Collections::IMapView<Platform::String^, ::Windows::Foundation::Collections::IVectorView<Platform::String^>^>^ hash);
rho::Hashtable<rho::String, rho::Hashtable<rho::String, rho::String> > convertHashOfHashesFromWP8(::Windows::Foundation::Collections::IMapView<Platform::String^, ::Windows::Foundation::Collections::IMapView<Platform::String^, Platform::String^>^>^ hash);
}
}
