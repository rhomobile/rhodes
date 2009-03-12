/*
 ============================================================================
 Name		 : ruby_geolocation.cpp
 Author	     : Anton Antonov
 Version	 : 1.0
 Copyright   : Copyright (C) 2008 Rhomobile. All rights reserved.

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

#include <e32def.h>

extern "C" {

TReal g_geo_latitude = 0.0;
TReal g_geo_longitude = 0.0;

double _geo_latitude() {
	return g_geo_latitude;
}

double _geo_longitude() {
	return g_geo_longitude;
}

}
