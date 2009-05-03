package rhomobile.sync;

import j2me.util.ArrayList;

import java.io.InputStream;
import java.io.IOException;
import com.rho.SimpleFile;
import com.rho.RhoClassFactory;

import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;

import rhomobile.URI;
import com.rho.db.*;
import com.xruby.runtime.builtin.RubyArray;

public class SyncBlob {
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("SyncBlob");
	
/*
	public static final int SYNC_STAGE_NONE = 0;
	public static final int SYNC_STAGE = 1;
	public static final int POSTSYNC_STAGE = 2;
	public static final int REQUEST_STAGE = 3;
*/
	public static String makeBlobFolderName()throws Exception{
		String fName = RhoClassFactory.createFile().getDirPath("blobs");
		
		return fName;
	}
/*	
	private static String makeFileName(SyncObject syncObj)throws IOException{
		
		String fName = makeBlobFolderName();
		
		String strExt = ".bin";
		URI uri = new URI(syncObj.getValue());
		int nDot = uri.getPath().lastIndexOf('.');
		if ( nDot >= 0 )
			strExt = uri.getPath().substring(nDot);
		else{
			int nExt = uri.getQueryString().indexOf("extension=");
			if ( nExt >= 0 ){
				int nExtEnd = uri.getQueryString().indexOf("&", nExt);
				if (nExtEnd < 0 )
					nExtEnd = uri.getQueryString().length();
				
				strExt = uri.getQueryString().substring(nExt+10, nExtEnd);
			}
		}
		
		fName += "id_" + Integer.toHexString(syncObj.getPrimaryKey()) + strExt;
		
		return fName;
	}
	
	private static final int bufferedRead(byte[] a, InputStream in) throws IOException {
		int bytesRead = 0;
		while (bytesRead < (a.length)) {
			int read = in.read();// a, 0, a.length );
			if (read < 0) {
				break;
			}
			a[bytesRead] = (byte)read;
			bytesRead ++;
		}
		return bytesRead;
	}
	
	static void insertOp(SyncObject syncObj, String client_id, int nStage){
		String type = syncObj.get_type();
		if ( type == null || !type.startsWith("blob.url") )
			return;
		
		InputStream is = null;
    	Jsr75File file = new Jsr75File();
		
		try{
			String fName = makeFileName( syncObj );
			String url = syncObj.getValue();
			int nQuest = url.lastIndexOf('?');
			if ( nQuest > 0 )
				url += "&";
			else
				url += "?";
			url += "client_id=" + client_id;

    		file.open(fName, false, false);
			
			is = SyncManager.openRemoteStream( url, "", false );

			if ( is != null)
			{
				synchronized (SyncUtil.m_byteBuffer) {			
					int nRead = 0;
		    		do{
		    			nRead = bufferedRead(SyncUtil.m_byteBuffer,is);
		    			if ( nRead > 0 )
		    				file.getOutStream().write(SyncUtil.m_byteBuffer, 0, nRead);
		    		}while( nRead > 0 );
				}
				
				syncObj.set_type("blob.file");
				syncObj.setValue(fName);
				
				//TODO: Send ack to sync server that we recieve blob
			}
		}catch(IOException exc){
			
		}finally{
			
			try{
				file.close();
			}catch(StorageError exc){}
			
			try{
				if ( is != null )
					is.close();
			}catch(IOException exc){}
			
			SyncManager.closeConnection();
		}
		
	}*/

	public static ArrayList extractBlobs(ArrayList list){
		ArrayList listBlobs = new ArrayList();
		
		for (int i = 0; i < list.size(); ) {
			SyncOperation op = (SyncOperation) list.get(i);
			if ( op.get_operation() != SyncConstants.UPDATE_TYPE_DELETE )
			{
				SyncObject obj = op.get_object();
				if ( obj != null && obj.get_type().equals("blob.file")){
					listBlobs.add(op);
					list.remove(i);
					
					continue;
				}
			}
			
			i++;
		}
		
		return listBlobs;
	}
	
	public static int pushRemoteBlobs(SyncSource source, ArrayList list, String clientId) {
		
		int success = SyncConstants.SYNC_PUSH_CHANGES_OK;
		for (int i = 0; i < list.size() && success != SyncConstants.SYNC_PUSH_CHANGES_ERROR; i++) {
			SyncOperation op = (SyncOperation) list.get(i);
			SyncObject obj = op.get_object();
			
			SimpleFile file = null;
			try {
				file = RhoClassFactory.createFile();
				
				file.open(obj.getValue(), true, true);
				
				if ( file.length() > 0 )
				{
					// Construct the post url
					String url = source.get_sourceUrl() + "/" + op.get_operation()
							+ "?client_id=" + clientId + "&" + op.get_postBody();// + "&type=blob";
					
					String session = SyncUtil.get_session(source);
					success = SyncManager.pushRemoteData(url, file.getInputStream(), session, true,
							"multipart/form-data" );
				}
			} catch (IOException e) {
				LOG.ERROR("There was an error pushing changes", e);
				success = SyncConstants.SYNC_PUSH_CHANGES_ERROR;
			} catch ( Exception e){
				LOG.ERROR("There was an error open file: " + obj.getValue(), e);
				success = SyncConstants.SYNC_PUSH_CHANGES_ERROR;
			}finally{
				if ( file != null )
					try{ file.close(); }catch(IOException exc2){}
					
				file = null;
			}
		}
		
		return success;
	}
	
	public static class DBCallback implements IDBCallback{

		public void OnDeleteAll() {
			OnDeleteAllFromTable(SyncConstants.OBJECTS_TABLE);
		}
		
		public void OnDeleteAllFromTable(String tableName) {
			if ( !tableName.equals(SyncConstants.OBJECTS_TABLE) )
				return;

			try{
				String fName = makeBlobFolderName();
				RhoClassFactory.createFile().delete(fName);
			}catch(Exception exc){
				LOG.ERROR("DBCallback.OnDeleteAllFromTable: Error delete files from table: " + tableName, exc);				
			}
		}

		public void OnDeleteFromTable(String tableName, IDBResult rows2Delete) 
		{
			if ( !SyncConstants.OBJECTS_TABLE.equalsIgnoreCase(tableName) )
				return;
			
			if ( ! rows2Delete.getString(0, SyncConstants.COL_ATTRIBTYPE).equals("blob.file") )
				return;

			String strUpdateType = rows2Delete.getString(0, SyncConstants.COL_UPDATETYPE);
			if ( strUpdateType.equals("create") || strUpdateType.equals("update") )
				return;

			String url = rows2Delete.getString(0, SyncConstants.COL_VALUE);
			if ( url == null || url.length() == 0 )
				return;
			
			try{
			    SimpleFile oFile = RhoClassFactory.createFile();
			    oFile.delete(url);
			}catch(Exception exc){
				LOG.ERROR("DBCallback.OnDeleteFromTable: Error delete file: " + url, exc);				
			}
		}
		
	}
	
}
