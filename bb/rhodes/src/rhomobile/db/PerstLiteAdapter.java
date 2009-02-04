package rhomobile.db;

import org.garret.perst.IInputStream;
import org.garret.perst.IOutputStream;
import org.garret.perst.Index;
import org.garret.perst.Iterator;
import org.garret.perst.Persistent;
import org.garret.perst.SimpleFile;
import org.garret.perst.Storage;
import org.garret.perst.StorageError;
import org.garret.perst.StorageFactory;
import org.garret.perst.Types;
import org.garret.perst.Key;
import org.garret.perst.impl.FileFactory;
import com.xruby.runtime.builtin.*;
import com.xruby.runtime.lang.*;

//@RubyLevelClass(name="DbAdapter")
public class PerstLiteAdapter  extends RubyBasic {
	
	public static final RubyString ID = ObjectFactory.createString("id");
	public static final RubyString SOURCE_ID = ObjectFactory.createString("source_id");
	public static final RubyString SOURCE_URL = ObjectFactory.createString("source_url");
	public static final RubyString SESSION = ObjectFactory.createString("session");
	
	public static final RubyString ALL = ObjectFactory.createString("*");
	
	private static final String DB_FILENAME = "syncdb.dbs";
	private static final String DB_VERSION_FNAME = "version";

	public static class Table_base1 extends Persistent{
	    public Table_base1(){}
	    public Table_base1(Storage db, RubyHash hash){
	        super(db);
	    }
		
	    RubyHash getValueByName(RubyString name){
	    	return ObjectFactory.createHash();
	    }
	    public void setValues(RubyHash hash){
	    }
	    
	    public boolean addOrdered( RubyArray res, RubyValue attrib, 
	    		RubyValue orderBy, RubyHash distinctMap, boolean count ){
	    	RubyHash hash = getValueByName((RubyString)attrib);
	    	if ( distinctMap != null && hash.size().toInt() > 0 ){
	    		RubyValue val = hash.get(attrib);
	    		if ( val != RubyConstant.QNIL ){
	    			if ( distinctMap.has_key(val) == RubyConstant.QTRUE )
	    				return false;
	    			distinctMap.add(val, val);
	    		}
	    	}
	    	
	    	if ( count ){
	    		if ( !attrib.equals(ALL) && hash.size().toInt() > 0 ){
		    		RubyValue val = hash.get(attrib);
		    		if ( val == RubyConstant.QNIL )
		    			return false;
	    		}
	    		return true;
	    	}
	    	
	    	if ( orderBy == null ){
	    		res.add( hash );
	    		return true;
	    	}
	    	
    		RubyString val1 = (RubyString)(hash.get(orderBy));
    		for( int i = 0; i < res.size(); i++ ){
    			RubyString val2 = (RubyString)(((RubyHash)res.get(i)).get(orderBy));
    			if ( val1.operator_compare(val2).toInt() <= 0 ){
    				res.insert(i, hash);
    				return true;
    			}
    		}
    		res.add( hash );
	    	return true;
	    }
	    
	}
	
	public static class Table_base extends Table_base1{
	    int id = 0;
	    int source_id=0;
		
	    // Serialize the object
	    public void writeObject(IOutputStream out) {
	    	out.writeInt(id);
	        out.writeInt(source_id);
	    }

	    // Deserialize the object
	    public void readObject(IInputStream in) {
	    	id = in.readInt();
	    	source_id = in.readInt();
	    }
		
	    RubyHash getValueByName(RubyString name){
	    	RubyHash res = ObjectFactory.createHash();
	    	boolean bAll = name.equals(ALL);
	    	
	    	if ( bAll || name.equals(SOURCE_ID) )
	    		res.add( SOURCE_ID, ObjectFactory.createInteger(source_id) );
	    	if ( bAll || name.equals(ID) )
	    		res.add( ID, ObjectFactory.createInteger(id) );
	    	
	    	return res;
	    }
	    
	    public Table_base(){}
	    public Table_base(Storage db, RubyHash hash){
	        super(db,hash);
	    }
	    
	    public void setValues(RubyHash hash){
	        RubyValue val = hash.getValue(ID);
	        if ( val != RubyConstant.QNIL )
	        	id = val.toInt();
	        
	        val = hash.getValue(SOURCE_ID);
	        if ( val != RubyConstant.QNIL )
	        	source_id = val.toInt();
	    }
	    
	}
	
	public static abstract class TableRootBase extends Persistent { 

		public TableRootBase(){
		}
		public TableRootBase(Storage st){
			super(st);
		}
	    public abstract void clear();
	    
	    public abstract void put(Table_base1 item);
	    //public abstract void set(Table_base item);
	    public abstract void remove(Table_base1 item);
	    
	    public abstract Iterator iterator( RubyHash where);
	    public abstract Iterator iterator();
	    
	}	
	
	public static class Table_object_values extends Table_base { 
	    String attrib="";
	    String object="";
	    String value="";
	    String update_type="";

		private static final RubyString ATTRIB = ObjectFactory.createString("attrib");
		private static final RubyString OBJECT = ObjectFactory.createString("object");
		private static final RubyString VALUE = ObjectFactory.createString("value");
		private static final RubyString UPDATE_TYPE = ObjectFactory.createString("update_type");
	    
		public static class TableRoot extends TableRootBase { 
		    Index source_idANDupdate_type;
		    Index object_idANDattrib;
		    Index object_idANDupdate_type;
		    
		    // Deserialize the object
		    public void readObject(IInputStream in) {
		    	source_idANDupdate_type = (Index)in.readObject();
		    	object_idANDattrib = (Index)in.readObject();
		    	object_idANDupdate_type = (Index)in.readObject();
		    }

		    // Serialize the object
		    public void writeObject(IOutputStream out) { 
		        out.writeObject(source_idANDupdate_type);
		        out.writeObject(object_idANDattrib);
		        out.writeObject(object_idANDupdate_type);
		    }

		    public TableRoot() {}
		    public TableRoot(Storage db) { 
		        super(db);
		        
		        source_idANDupdate_type = db.createIndex(
		        		new int[]{Types.Int,Types.String}, false);
		        object_idANDattrib = db.createIndex(
		        		new int[]{Types.String,Types.String}, false);
		        object_idANDupdate_type = db.createIndex(
		        		new int[]{Types.String,Types.String}, false);
		    }
		    public void clear(){
		    	Iterator iter = source_idANDupdate_type.iterator();
		    	while(iter.hasNext()){
		    		Table_object_values item = (Table_object_values)iter.next();
		    		iter.remove();
		    	}
		    	source_idANDupdate_type.clear();

		    	iter = object_idANDupdate_type.iterator();
		    	while(iter.hasNext()){
		    		
		    		Table_object_values item = (Table_object_values)iter.next();
		    		iter.remove();
		    	}
		    	object_idANDupdate_type.clear();
		    	
		    	iter = object_idANDattrib.iterator();
		    	while(iter.hasNext()){
		    		
		    		Table_object_values item = (Table_object_values)iter.next();
		    		iter.remove();
		    		item.deallocate();
		    	}
		    	object_idANDattrib.clear();
		    }
		    
		    public void put(Table_base1 itemB){
		    	Table_object_values item = (Table_object_values)itemB;
		    	source_idANDupdate_type.put(new Key( new Object[]{new Integer(item.source_id), item.update_type}),item);
		    	object_idANDattrib.put(new Key( item.object, item.attrib),item);
		    	object_idANDupdate_type.put(new Key( item.object, item.update_type),item);
		    }
		    public void remove(Table_base1 itemB){
		    	Table_object_values item = (Table_object_values)itemB;
		    	source_idANDupdate_type.remove(new Key( new Object[]{new Integer(item.source_id), item.update_type}),item);
		    	object_idANDattrib.remove(new Key( item.object, item.attrib),item);
		    	object_idANDupdate_type.remove(new Key( item.object, item.update_type),item);
		    	item.deallocate();
		    }

		    public Iterator iterator(){
		    	return source_idANDupdate_type.iterator();
		    }
		    
		    public Iterator iterator( RubyHash where){
		    	RubyValue valSourceID = where.get(SOURCE_ID);
		    	RubyValue valObject = where.get(Table_object_values.OBJECT);
		    	RubyValue valUpdatedType = where.get(Table_object_values.UPDATE_TYPE);
		    	RubyValue valAttrib = where.get(Table_object_values.ATTRIB);

		    	if ( valAttrib != RubyConstant.QNIL && valObject != RubyConstant.QNIL ){
		    		Key key = new Key(valObject.toStr(),valAttrib.toStr()); 
		    		return object_idANDattrib.iterator( key, key, Index.ASCENT_ORDER);
		    	}

		    	if ( valUpdatedType != RubyConstant.QNIL && valObject != RubyConstant.QNIL ){
		    		Key key = new Key(valObject.toStr(),valUpdatedType.toStr()); 
		    		return object_idANDupdate_type.iterator( key, key, Index.ASCENT_ORDER);
		    	}
		    	
		    	if ( valObject != RubyConstant.QNIL ){
		    		Key key = new Key(new Object[]{valObject.toStr()}); 
		    		return object_idANDattrib.iterator( key, key, Index.ASCENT_ORDER);
		    	}
		    	
		    	if ( valUpdatedType != RubyConstant.QNIL && valSourceID != RubyConstant.QNIL ){
		    		Key key = new Key(new Object[]{new Integer(valSourceID.toInt()),valUpdatedType.toStr()});
		    		return source_idANDupdate_type.iterator( key, key, Index.ASCENT_ORDER);
		    	}
		    	if ( valSourceID != RubyConstant.QNIL ){
		    		Key key = new Key(new Object[]{new Integer(valSourceID.toInt())});
		    		return source_idANDupdate_type.iterator( key, key, Index.ASCENT_ORDER);
		    	}
		    	
		    	return null;
		    }
	    
		}	
		
	    // Serialize the object
	    public void writeObject(IOutputStream out) { 
	    	super.writeObject(out);
	    	out.writeInt(id);
	    	out.writeString(attrib);
	    	out.writeString(object);
	    	out.writeString(value);
	    	out.writeString(update_type);	    	
	    }

	    // Deserialize the object
	    public void readObject(IInputStream in) { 
	    	super.readObject(in);
	    	id = in.readInt();
	    	attrib = in.readString();
	    	object = in.readString();
	    	value = in.readString();
	    	update_type = in.readString();
	    }

	    RubyHash getValueByName(RubyString name){
	    	RubyHash res = super.getValueByName(name);
	    	boolean bAll = name.equals(ALL);
	    	
	    	if ( bAll || name.equals(ATTRIB) )
	    		res.add( ATTRIB, ObjectFactory.createString(attrib) );
	    	if ( bAll || name.equals(OBJECT) )
	    		res.add( OBJECT, ObjectFactory.createString(object) );
	    	if ( bAll || name.equals(VALUE) )
	    		res.add( VALUE, ObjectFactory.createString(value) );
	    	if ( bAll || name.equals(UPDATE_TYPE) )
	    		res.add( UPDATE_TYPE, ObjectFactory.createString(update_type) );
	    	
	    	return res;
	    }
	    
	    public static String name(){return "object_values";}
	    public void setValues(RubyHash hash){
	    	super.setValues(hash);
	    	
	        RubyValue val = hash.getValue(ATTRIB);
	        if ( val != RubyConstant.QNIL )
	        	attrib = val.toStr();
	        val = hash.getValue(OBJECT);
	        if ( val != RubyConstant.QNIL )
	        	object = val.toStr();
	        val = hash.getValue(VALUE);
	        if ( val != RubyConstant.QNIL )
	        	value = val.toStr();	               
	        val = hash.getValue(UPDATE_TYPE);
	        if ( val != RubyConstant.QNIL )
	        	update_type = val.toStr();	        
	    }
	    
	    public Table_object_values(){}
	    public Table_object_values(Storage db, RubyHash hash){
	        super(db,hash);
	        setValues(hash);
	    }
	};

	public static class Table_sources extends Table_base { 
		
	    String source_url = "";
	    String session = "";
	    int last_updated = 0;
	    int last_inserted_size = 0;
	    int last_deleted_size = 0;
	    int last_sync_duration = 0;
	    int last_sync_success = 0;
	    
	    public static final RubyString LAST_UPDATED = ObjectFactory.createString("last_updated");
	    public static final RubyString LAST_INSERTED_SIZE = ObjectFactory.createString("last_inserted_size");
	    public static final RubyString LAST_DELETED_SIZE = ObjectFactory.createString("last_deleted_size");
	    public static final RubyString LAST_SYNC_DURATION = ObjectFactory.createString("last_sync_duration");
	    public static final RubyString LAST_SYNC_SUCCESS = ObjectFactory.createString("last_sync_success");
	    
		public static class TableRoot extends TableRootBase { 
		    Index source_id;

		    // Deserialize the object
		    public void readObject(IInputStream in) { 
		    	source_id = (Index)in.readObject();
		    }

		    // Serialize the object
		    public void writeObject(IOutputStream out) { 
		        out.writeObject(source_id);
		    }

		    public TableRoot() {}
		    public TableRoot(Storage db) { 
		        super(db);
		        
		        source_id = db.createIndex(Types.Int, true);
		    }
		    
		    public void clear(){
		    	Iterator iter = source_id.iterator();
		    	while(iter.hasNext()){
		    		Table_base item = (Table_base)iter.next();
		    		source_id.remove(new Key(item.source_id),item);
		    		item.deallocate();
		    	}
		    	source_id.clear();
		    }
		    
		    public void put(Table_base1 itemB){
		    	Table_sources item = (Table_sources)itemB; 
		    	source_id.put(new Key(item.source_id), item);
		    }
		    public void remove(Table_base1 itemB){
		    	Table_sources item = (Table_sources)itemB;
		    	source_id.remove(new Key(item.source_id),item);
		    	item.deallocate();
		    }

		    public Iterator iterator(){
	    		return source_id.iterator(); 
		    }
		    
		    public Iterator iterator(RubyHash where){
		    	RubyValue val = where.get(SOURCE_ID);
		    	if ( val == RubyConstant.QNIL )
		    		return null;
		    	return source_id.iterator(new Key(val.toInt()), new Key(val.toInt()), Index.ASCENT_ORDER);
		    }
		}	
		
	    public static String name(){return "sources";}
	    
	    public Table_sources() {}
	    public Table_sources(Storage db, RubyHash hash){
	        super(db,hash);
	        setValues(hash);
	    }
	    
	    // Serialize the object
	    public void writeObject(IOutputStream out) { 
	    	super.writeObject(out);
	    	out.writeString(source_url);
	    	out.writeString(session);
	    	out.writeInt(last_updated);
	    	out.writeInt(last_inserted_size);
	    	out.writeInt(last_deleted_size);
	    	out.writeInt(last_sync_duration);
	    	out.writeInt(last_sync_success);
	    }

	    // Deserialize the object
	    public void readObject(IInputStream in) { 
	    	super.readObject(in);
	    	source_url = in.readString();
	    	session = in.readString();
	    	last_updated = in.readInt();
	    	last_inserted_size = in.readInt();
	    	last_deleted_size = in.readInt();
	    	last_sync_duration = in.readInt();
	    	last_sync_success = in.readInt();
	    }
	    
	    RubyHash getValueByName(RubyString name){
	    	RubyHash res = super.getValueByName(name);
	    	boolean bAll = name.equals(ALL);
	    	
	    	if ( bAll || name.equals(SOURCE_URL) )
	    		res.add( SOURCE_URL, ObjectFactory.createString(source_url) );
	    	if ( bAll || name.equals(SESSION) )
	    		res.add( SESSION, ObjectFactory.createString(session) );
	    	if ( bAll || name.equals(LAST_UPDATED) )
	    		res.add( LAST_UPDATED, ObjectFactory.createInteger(last_updated) );
	    	if ( bAll || name.equals(LAST_INSERTED_SIZE) )
	    		res.add( LAST_INSERTED_SIZE, ObjectFactory.createInteger(last_inserted_size) );
	    	if ( bAll || name.equals(LAST_DELETED_SIZE) )
	    		res.add( LAST_DELETED_SIZE, ObjectFactory.createInteger(last_deleted_size) );
	    	if ( bAll || name.equals(LAST_SYNC_DURATION) )
	    		res.add( LAST_SYNC_DURATION, ObjectFactory.createInteger(last_sync_duration) );
	    	if ( bAll || name.equals(LAST_SYNC_SUCCESS) )
	    		res.add( LAST_SYNC_SUCCESS, ObjectFactory.createInteger(last_sync_success) );
	    	
	    	return res;
	    }
	    
	    public void setValues(RubyHash hash){
	    	super.setValues(hash);
	    	
	        RubyValue val = hash.getValue(SOURCE_URL);
	        if ( val != RubyConstant.QNIL )
	        	source_url = val.toStr();
	        val = hash.getValue(SESSION);
	        if ( val != RubyConstant.QNIL )
	        	session = val.toStr();
	        val = hash.getValue(LAST_UPDATED);
	        if ( val != RubyConstant.QNIL )
	        	last_updated = val.toInt();
	        val = hash.getValue(LAST_INSERTED_SIZE);
	        if ( val != RubyConstant.QNIL )
	        	last_inserted_size = val.toInt();
	        val = hash.getValue(LAST_DELETED_SIZE);
	        if ( val != RubyConstant.QNIL )
	        	last_deleted_size = val.toInt();
	        val = hash.getValue(LAST_SYNC_DURATION);
	        if ( val != RubyConstant.QNIL )
	        	last_sync_duration = val.toInt();
	        val = hash.getValue(LAST_SYNC_SUCCESS);
	        if ( val != RubyConstant.QNIL )
	        	last_sync_success = val.toInt();
	    }
	    
	};

	public static class Table_client_info extends Table_base1 { 
		
	    String client_id = "";
		
		public static final RubyString CLIENT_ID = ObjectFactory.createString("client_id");
	    
		public static class TableRoot extends TableRootBase { 
		    Index client_id;

		    // Deserialize the object
		    public void readObject(IInputStream in) { 
		    	client_id = (Index)in.readObject();
		    }

		    // Serialize the object
		    public void writeObject(IOutputStream out) { 
		        out.writeObject(client_id);
		    }

		    public TableRoot() {}
		    public TableRoot(Storage db) { 
		        super(db);
		        
		        client_id = db.createIndex(Types.String, true);
		    }
		    
		    public void clear(){
		    	Iterator iter = client_id.iterator();
		    	while(iter.hasNext()){
		    		Table_client_info item = (Table_client_info)iter.next();
		    		client_id.remove(new Key(item.client_id),item);
		    		item.deallocate();
		    	}
		    	client_id.clear();
		    }
		    
		    public void put(Table_base1 itemB){
		    	Table_client_info item = (Table_client_info)itemB; 
		    	client_id.put(new Key( item.client_id),item);
		    }
		    public void remove(Table_base1 itemB){
		    	Table_client_info item = (Table_client_info)itemB;
		    	client_id.remove(new Key( item.client_id),item);
		    	item.deallocate();
		    }

		    public Iterator iterator(){
	    		return client_id.iterator(); 
		    }
		    
		    public Iterator iterator(RubyHash where){
		    	RubyValue val = where.get(CLIENT_ID);
		    	if ( val == RubyConstant.QNIL )
		    		return null;
		    	return client_id.iterator(new Key(val.toString()), new Key(val.toString()), Index.ASCENT_ORDER);
		    }
		}	
		
	    public static String name(){return "client_info";}
	    
	    public Table_client_info() {}
	    public Table_client_info(Storage db, RubyHash hash){
	        super(db,hash);
	        setValues(hash);
	    }
	    
	    // Serialize the object
	    public void writeObject(IOutputStream out) { 
	    	super.writeObject(out);
	    	out.writeString(client_id);
	    }

	    // Deserialize the object
	    public void readObject(IInputStream in) { 
	    	super.readObject(in);
	    	client_id = in.readString();
	    }
	    
	    RubyHash getValueByName(RubyString name){
	    	RubyHash res = super.getValueByName(name);
	    	boolean bAll = name.equals(ALL);
	    	
	    	if ( bAll || name.equals(CLIENT_ID) )
	    		res.add( CLIENT_ID, ObjectFactory.createString(client_id) );
	    	
	    	return res;
	    }
	    
	    public void setValues(RubyHash hash){
	    	super.setValues(hash);
	    	
	        RubyValue val = hash.getValue(CLIENT_ID);
	        if ( val != RubyConstant.QNIL )
	        	client_id = val.toStr();
	    }
	    
	};
	
	Storage m_storage;
	private static PerstLiteAdapter adapter = null;
	PerstLiteAdapter(RubyClass c) {
		super(c);
	}

    //@RubyAllocMethod
    public static PerstLiteAdapter alloc(RubyValue receiver) {
    	if(adapter == null) {
    		adapter = new PerstLiteAdapter(RubyRuntime.DBAdapterClass);
    	} 
    	return adapter;
    }
	
    //@RubyLevelMethod(name="initialize")
    public PerstLiteAdapter initialize(RubyValue v) {
    	createDB(v !=null && v != RubyConstant.QNIL ? v.toString() : "");
        return this;
    }
	
	void createDB(String strVer) {
		if (isClosed() == RubyConstant.QFALSE) {
			return;
		}

    	//Check version
		if ( strVer != null && strVer.length() > 0 ){
        	String dbVer = readDBVersion();
			if ( dbVer == null || !dbVer.equalsIgnoreCase(strVer) ){
				org.garret.perst.impl.Jsr75File.delete(DB_FILENAME);
	            writeDBVersion(strVer);
			}
        }
		
		m_storage = StorageFactory.getInstance().createStorage();
		
        try { 
        	m_storage.open(DB_FILENAME);
        } catch (StorageError x) { 
            throw new RubyException("Failed to create or open database:" + DB_FILENAME + ".Error:" + x.getMessage());
        }
        Index root = (Index)m_storage.getRoot();
        if (root == null) { // if root object was not specified, then storage is not yet initialized
            // Perform initialization:
            // ... create root object
            root = m_storage.createIndex(Types.String, true);
            // ... register new root object
            m_storage.setRoot(root);
            
            root.put(Table_sources.name(), new Table_sources.TableRoot(m_storage) );
            root.put(Table_object_values.name(), new Table_object_values.TableRoot(m_storage) );
            root.put(Table_client_info.name(), new Table_client_info.TableRoot(m_storage) );
        }
	}
	
	String readDBVersion(){
        SimpleFile file = FileFactory.createFile();
        file.open(DB_VERSION_FNAME, true, true);
        byte buf[] = new byte[20];
		int len = file.read(0, buf);
		file.close();
		return new String(buf,0,len);
	}
	
	void writeDBVersion(String ver){
        SimpleFile file = FileFactory.createFile();
        file.open(DB_VERSION_FNAME, false, false);
        file.write(0, ver.getBytes());
        file.close();
	}
	
	TableRootBase getTableRoot( RubyValue tableName ){
		Index root = (Index)m_storage.getRoot();
		if ( root != null )
		{
			return (TableRootBase)(root.get(tableName.toStr()));
		}
		
		return null;
	}
	
	//@RubyLevelMethod(name="insertIntoTable")
	public synchronized RubyValue insertIntoTable(RubyValue tableName, RubyValue mapValues) {
		TableRootBase tblRoot = getTableRoot(tableName);
		if ( tblRoot != null ){
			String strName = tableName.toStr();
			Table_base1 item = null;
			if ( strName.equals(Table_sources.name()) )
				item = new Table_sources(m_storage,(RubyHash)mapValues);
			else if ( strName.equals(Table_object_values.name()) )
				item = new Table_object_values(m_storage,(RubyHash)mapValues);
			else if ( strName.equals(Table_client_info.name()) )
				item = new Table_client_info(m_storage,(RubyHash)mapValues);
			else
				throw new RubyException("Unknown database name:" + strName);
			
			tblRoot.put(item);
			m_storage.commit();
		}
			
		return new RubyArray();
	}
	
	//@RubyLevelMethod(name="selectFromTable")
	public synchronized RubyValue selectFromTable(RubyArray args ){
		if ( args.size() < 2 ){
			new RubyException("selectFromTable has 3 paramaters: tableName, attrib, where");
			return RubyConstant.QNIL;
		}
		return 	selectFromTable(args.get(0), args.get(1), (args.size() > 2 ? args.get(2) : null),
				(args.size() > 3 ? args.get(3) : null ) );
	}
	
	RubyValue selectFromTable(RubyValue tableName, RubyValue attrib, 
			RubyValue where, RubyValue params){
		RubyValue orderBy = null;
		boolean distinct = false;
		boolean count = false;
		if ( params != null && params != RubyConstant.QNIL ){
			orderBy = ((RubyHash)params).get(ObjectFactory.createString("order by"));
			if ( orderBy == RubyConstant.QNIL )
				orderBy = null;
			distinct = ((RubyHash)params).get(ObjectFactory.createString("distinct")) != 
				RubyConstant.QNIL;
			count = ((RubyHash)params).get(ObjectFactory.createString("count")) != 
				RubyConstant.QNIL;
		}

		RubyArray res = new RubyArray(); 
		int nCount = 0;
		TableRootBase tblRoot = getTableRoot(tableName);
		if ( tblRoot != null ){
			Iterator iter = (where != null && where != RubyConstant.QNIL) ? 
					tblRoot.iterator((RubyHash)where) :
					tblRoot.iterator();
					
			RubyHash distinctMap = distinct ? ObjectFactory.createHash() : null;
			while(iter != null && iter.hasNext()){
				Table_base1 item = (Table_base1)iter.next();
				nCount += item.addOrdered( res, attrib, orderBy, distinctMap, count ) ? 1:0;
			}
		}
		
		if ( count )
			return ObjectFactory.createInteger(nCount);
		
		return res;
	}

	//@RubyLevelMethod(name="updateIntoTable")
	public synchronized RubyValue updateIntoTable(RubyArray args ){
		if ( args.size() != 3 ){
			new RubyException("updateIntoTable has 3 paramaters: tableName, mapValues, where");
			return RubyConstant.QNIL;
		}
		return 	updateIntoTable(args.get(0), args.get(1), args.get(2));
	}
	
	public synchronized RubyValue updateIntoTable(RubyValue tableName, RubyValue mapValues, RubyValue where ) {
		TableRootBase tblRoot = getTableRoot(tableName);
		if ( tblRoot != null ){
//			Iterator iter = tblRoot.iterator((RubyHash)where);
			Iterator iter = (where != null && where != RubyConstant.QNIL) ? 
					tblRoot.iterator((RubyHash)where) :
					tblRoot.iterator();
			
			while(iter != null && iter.hasNext()){
				Table_base1 item = (Table_base1)iter.next();
				if ( item != null ){
					item.setValues((RubyHash)mapValues);
					item.modify();
					//tblRoot.set(item);
				}
			}
			m_storage.commit();
		}
		return new RubyArray();
	}
	
	//@RubyLevelMethod(name="deleteAllFromTable")
	public synchronized RubyValue deleteAllFromTable(RubyValue tableName) {
		TableRootBase tblRoot = getTableRoot(tableName);
		if ( tblRoot != null ){
			m_storage.setProperty("perst.concurrent.iterator",Boolean.TRUE);
			tblRoot.clear();
			m_storage.commit();
			m_storage.setProperty("perst.concurrent.iterator",Boolean.FALSE);
		}
			
		return new RubyArray();
	}
	
	//@RubyLevelMethod(name="deleteFromTable")
	public synchronized RubyValue deleteFromTable(RubyValue tableName, RubyValue where){
		TableRootBase tblRoot = getTableRoot(tableName);
		if ( tblRoot != null ){
			m_storage.setProperty("perst.concurrent.iterator",Boolean.TRUE);
			Iterator iter = tblRoot.iterator((RubyHash)where);
			while(iter != null && iter.hasNext()){
				Table_base1 item = (Table_base1)iter.next();
				if ( item != null ){
					tblRoot.remove(item);
				}
			}
			m_storage.commit();
			m_storage.setProperty("perst.concurrent.iterator",Boolean.FALSE);
		}
		return new RubyArray();
	}
	
	//@RubyLevelMethod(name="close")
	public RubyValue close(){
		if ( m_storage != null ){
			m_storage.close();
			m_storage = null;
		}
		
		return RubyConstant.QNIL;
	}

	//@RubyLevelMethod(name="closed?")
	RubyValue isClosed(){
		if ( m_storage != null && m_storage.isOpened() )
			return RubyConstant.QFALSE;
		
		return RubyConstant.QTRUE;
	}
	
	public static void initMethods( RubyClass klass){
		klass.defineMethod( "initialize", new RubyOneArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
				return ((PerstLiteAdapter)receiver).initialize(arg);}
		});
		
		klass.defineMethod( "deleteAllFromTable", new RubyOneArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
				return ((PerstLiteAdapter)receiver).deleteAllFromTable(arg);}
		});
		
		klass.defineMethod( "insertIntoTable", new RubyTwoArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block ){
				return ((PerstLiteAdapter)receiver).insertIntoTable(arg0, arg1);}
		});
		klass.defineMethod( "selectFromTable", new RubyVarArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
				return ((PerstLiteAdapter)receiver).selectFromTable(args);}
		});
		klass.defineMethod( "updateIntoTable", new RubyVarArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
				return ((PerstLiteAdapter)receiver).updateIntoTable(args);}
		});
		klass.defineMethod( "deleteFromTable", new RubyTwoArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block ){
				return ((PerstLiteAdapter)receiver).deleteFromTable(arg0, arg1);}
		});
		klass.defineMethod( "close", new RubyNoArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyBlock block ){
				return ((PerstLiteAdapter)receiver).close();}
		});
		klass.defineMethod( "closed?", new RubyNoArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyBlock block ){
				return ((PerstLiteAdapter)receiver).isClosed();}
		});
		
		klass.defineAllocMethod(new RubyNoArgMethod(){
			protected RubyValue run(RubyValue receiver, RubyBlock block )	{
				return PerstLiteAdapter.alloc(receiver);}
		});
		
	}
}
