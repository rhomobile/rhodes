/*
 *  rhodes
 *
 *  Copyright (C) 2008 Lars Burgess. All rights reserved.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
package com.rho.sync;

/*
 * Test class which simulates creates, updates and deletes a source
 */
public class SyncTest {

	public static void runAllTests() {
		testCreate();
		testUpdate();
		testDelete();
	}

	public static void testCreate() {
		SyncObject objName = new SyncObject("name", 1, "83242348",
				"lars test from bby", "create");
		SyncObject objIndustry = new SyncObject("industry", 1, "83242348",
				"Electronics", "create");
		objName.dehydrate();
		objIndustry.dehydrate();
	}

	public static void testUpdate() {
		SyncObject objIndustry = new SyncObject("industry", 1,
				"881be9cb-3879-9660-46be-48fceb82ea5f", "Electronics", "update");
		objIndustry.dehydrate();
	}

	public static void testDelete() {
		SyncObject obj = new SyncObject(null, 1,
				"b1495bf5-91a8-dac7-32ce-491b325d8202", null, "delete");
		obj.dehydrate();
	}
}
