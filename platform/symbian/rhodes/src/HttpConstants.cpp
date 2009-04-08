/*
 ============================================================================
 Name		: HttpClient.cpp
 Author	  : Anton Antonov
 Version	 : 1.0
 Copyright   :   Copyright (C) 2008 Rhomobile. All rights reserved.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ============================================================================
 */

#include "HttpConstants.h"

const char* CHttpConstants::KGetCommand = "GET";
const char* CHttpConstants::KPostCommand = "POST";

_DEFINE_LIT(CHttpConstants, KHttpIN, "\\data\\rho\\http\\in\\");
_DEFINE_LIT(CHttpConstants, KHttpOUT, "\\data\\rho\\http\\out\\");

_DEFINE_LIT(CHttpConstants, KFileIncompletePrefix, "___");
_DEFINE_LIT(CHttpConstants, KFileSystemRoot,"\\");
_DEFINE_LIT(CHttpConstants, KDateFormat,"%D%M%Y%/0%1%/1%2%/2%3%/3 %:0%H%:1%T%:2%S.%C%:3");

_DEFINE_LIT(CHttpConstants, KImagesDir, "\\data\\rho\\images\\");
