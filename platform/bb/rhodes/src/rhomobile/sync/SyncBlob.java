package rhomobile.sync;

import j2me.util.ArrayList;

import java.io.InputStream;
import java.io.IOException;
import org.garret.perst.impl.Jsr75File;
import org.garret.perst.StorageError;

import rhomobile.URI;
import rhomobile.db.PerstLiteAdapter;
import rhomobile.db.PerstLiteAdapter.Table_base1;

public class SyncBlob {

	public static final int SYNC_NONE = 0;
	public static final int SYNC_STAGE = 1;
	public static final int POSTSYNC_STAGE = 2;
	public static final int REQUEST_STAGE = 3;

	private static String makeBlobFolderName()throws IOException{
		String fName = Jsr75File.getDirPath("blobs");
		
		return fName;
	}
	
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
		
	}

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
		
		int success = 0;
		for (int i = 0; i < list.size() && success != SyncConstants.SYNC_PUSH_CHANGES_ERROR; i++) {
			SyncOperation op = (SyncOperation) list.get(i);
			SyncObject obj = op.get_object();
			
			Jsr75File file = new Jsr75File();
			
			try {
				file.open(obj.getValue(), true, true);
				
				// Construct the post url
				String url = source.get_sourceUrl() + "/" + op.get_operation()
						+ "?client_id=" + clientId + "&" + op.get_postBody();// + "&type=blob";
				
				String session = SyncUtil.get_session(source);
				success = SyncManager.pushRemoteData(url, file.getInputStream(), session, true,
						"application/octet-stream" );
			} catch (IOException e) {
				System.out.println("There was an error pushing changes: "
						+ e.getMessage());
				success = SyncConstants.SYNC_PUSH_CHANGES_ERROR;
			} catch ( StorageError e){
				System.out.println("There was an error open file: " + obj.getValue() + "; Error:"
						+ e.getMessage());
				success = SyncConstants.SYNC_PUSH_CHANGES_ERROR;
			}finally{
				if ( file != null )
					file.close();
				file = null;
			}
		}
		
		return success;
	}
	
	public static class DBCallback implements PerstLiteAdapter.IDbCallback{

		public void OnDeleteAllFromTable(String tableName) {
			if ( !tableName.equals(PerstLiteAdapter.Table_object_values.name()) )
				return;

			try{
				String fName = makeBlobFolderName();
				Jsr75File.delete(fName);
			}catch(StorageError exc){
			}catch(IOException exc){
				
			}
		}

		public void OnDeleteFromTable(String tableName, Table_base1 item) {
			if ( !(item instanceof PerstLiteAdapter.Table_object_values))
				return;
			
			PerstLiteAdapter.Table_object_values obj = (PerstLiteAdapter.Table_object_values)item;
			if ( !obj.getTypeField().equals("blob.file") )
				return;
			
			if ( obj.getUpdateTypeField().equals("create") || obj.getUpdateTypeField().equals("update") )
				return;
			
			String url = obj.getValueField();
			if ( url == null || url.length() == 0 )
				return;
			
			try{
				Jsr75File.delete(url);
			}catch(StorageError exc){
				
			}
		}
		
	}
	
}
