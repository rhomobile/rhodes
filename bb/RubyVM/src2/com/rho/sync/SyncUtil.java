package com.rho.sync;

import java.io.IOException;
import java.util.ArrayList;

public class SyncUtil {

	public static int fetchRemoteChanges(String url) {
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
		}
		return count;
	}

	public static int pushRemoteChanges(SyncOperation[] list) {
		int success = 0;

		return success == SyncConstants.SYNC_PUSH_CHANGES_OK ? SyncConstants.SYNC_PUSH_CHANGES_OK
				: SyncConstants.SYNC_PUSH_CHANGES_ERROR;
	}
}
