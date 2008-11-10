package com.rho.sync;

import java.io.IOException;
import java.util.ArrayList;

import com.rho.db.PerstLiteAdapter;
import com.xruby.runtime.builtin.ObjectFactory;
import com.xruby.runtime.builtin.RubyArray;
import com.xruby.runtime.builtin.RubyHash;
import com.xruby.runtime.builtin.RubyInteger;
import com.xruby.runtime.builtin.RubyString;

public class SyncUtil {

	public static PerstLiteAdapter adapter = null;

	public static int fetchRemoteChanges(String url, int id) {
		int count = 0;
		String data = null;
		try {
			data = SyncManager.fetchRemoteData(url + SyncConstants.SYNC_FORMAT);
		} catch (IOException e) {
			System.out
					.println("There was an error fetching data from the sync source: "
							+ e.getMessage());
		}
		if (data != null) {
			ArrayList list = SyncJSONParser.parseJSONList(data);
			count = list.size();
			if (count > 0) {
				SyncObject.deleteFromDatabaseBySource(id);
				for (int i = 0; i < count; i++) {
					int success = ((SyncObject) list.get(i)).dehydrate();
					if (success != SyncConstants.SYNC_OBJECT_SUCCESS) {
						System.out
								.println("There was an error saving records.");
					}
				}
			}
		}
		return count;
	}

	public static int pushRemoteChanges(SyncOperation[] list) {
		int success = 0;

		return success == SyncConstants.SYNC_PUSH_CHANGES_OK ? SyncConstants.SYNC_PUSH_CHANGES_OK
				: SyncConstants.SYNC_PUSH_CHANGES_ERROR;
	}

	public static RubyArray getSourceList() {
		RubyArray arr = createArray();
		arr.add(createString("sources"));
		arr.add(createString("*"));
		return (RubyArray) adapter.selectFromTable(arr);
	}

	public static RubyArray getObjectValueList(int id) {
		RubyArray arr = createArray();
		RubyHash where = createHash();
		arr.add(createString("object_values"));
		arr.add(createString("*"));
		where.add(createString("source_id"), createInteger(id));
		arr.add(where);
		return (RubyArray) adapter.selectFromTable(arr);
	}

	public static RubyString createString(String val) {
		return ObjectFactory.createString(val);
	}

	public static RubyInteger createInteger(long val) {
		return ObjectFactory.createInteger(val);
	}

	public static RubyHash createHash() {
		return ObjectFactory.createHash();
	}

	public static RubyArray createArray() {
		return new RubyArray();
	}

	public static void printResults(RubyArray objects) {
		// Debug code to print results
		for (int j = 0; j < objects.size(); j++) {
			RubyHash objectElement = (RubyHash) objects.at(SyncUtil
					.createInteger(j));
			String value = objectElement.get(SyncUtil.createString("value"))
					.toString();
			System.out.println("value[" + j + "]: " + value);
		}
	}
}
