package com.rho.db;

import org.garret.perst.IInputStream;
import org.garret.perst.IOutputStream;
import org.garret.perst.Index;
import org.garret.perst.Iterator;
import org.garret.perst.Persistent;
import org.garret.perst.Storage;
import org.garret.perst.StorageError;
import org.garret.perst.StorageFactory;
import org.garret.perst.Types;
import org.garret.perst.Key;

import com.xruby.runtime.builtin.*;
import com.xruby.runtime.lang.*;

//@RubyLevelClass(name="DbAdapter")
public class PerstLiteAdapter  extends RubyBasic {
	
	public static final RubyString ID = ObjectFactory.createString("id");
	public static final RubyString SOURCE_ID = ObjectFactory.createString("source_id");
	public static final RubyString URL = ObjectFactory.createString("source_url");
	public static final RubyString ALL = ObjectFactory.createString("*");
	
	private static final String DB_FILENAME = "syncdb_5.dbs";

	public static class Table_base extends Persistent{
	    int id = 0;
	    int source_id=0;
	    String url = "";
		
	    // Serialize the object
	    public void writeObject(IOutputStream out) {
	    	out.writeInt(id);
	        out.writeInt(source_id);
	        out.writeString(url);
	    }

	    // Deserialize the object
	    public void readObject(IInputStream in) {
	    	id = in.readInt();
	    	source_id = in.readInt();
	    	url = in.readString();
	    }
		
	    RubyHash getValueByName(RubyString name){
	    	RubyHash res = ObjectFactory.createHash();
	    	boolean bAll = name.equals(ALL);
	    	
	    	if ( bAll || name.equals(SOURCE_ID) )
	    		res.add( SOURCE_ID, ObjectFactory.createInteger(source_id) );
	    	if ( bAll || name.equals(URL) )
	    		res.add( URL, ObjectFactory.createString(url) );
	    	if ( bAll || name.equals(ID) )
	    		res.add( ID, ObjectFactory.createInteger(id) );
	    	
	    	return res;
	    }
	    public void addOrdered( RubyArray res, RubyValue attrib, 
	    		RubyValue orderBy, RubyHash distinctMap ){
	    	RubyHash hash = getValueByName((RubyString)attrib);
	    	if ( distinctMap != null && hash.size().toInt() > 0 ){
	    		RubyValue val = hash.get(attrib);
	    		if ( val != RubyConstant.QNIL ){
	    			if ( distinctMap.has_key(val) == RubyConstant.QTRUE )
	    				return;
	    			distinctMap.add(val, val);
	    		}
	    	}
	    	
	    	if ( orderBy == null )
	    		res.add( hash );
	    	else
	    	{
	    		RubyString val1 = (RubyString)(hash.get(orderBy));
	    		for( int i = 0; i < res.size(); i++ ){
	    			RubyString val2 = (RubyString)(((RubyHash)res.get(i)).get(orderBy));
	    			if ( val1.operator_compare(val2).toInt() <= 0 ){
	    				res.insert(i, hash);
	    				return;
	    			}
	    		}
	    		res.add( hash );
	    	}
	    }
	    
	    public Table_base(){}
	    
	    public void setValues(RubyHash hash){
	        RubyValue val = hash.getValue(ID);
	        if ( val != RubyConstant.QNIL )
	        	id = val.toInt();
	        
	        val = hash.getValue(SOURCE_ID);
	        if ( val != RubyConstant.QNIL )
	        	source_id = val.toInt();
	        val = hash.getValue(URL);
	        if ( val != RubyConstant.QNIL )
	        	url = val.toStr();
	        
	    }
	    
	    public Table_base(Storage db, RubyHash hash){
	        super(db);
	    }
	}
	
	public static abstract class TableRootBase extends Persistent { 

		public TableRootBase(){
		}
		public TableRootBase(Storage st){
			super(st);
		}
	    public abstract void clear();
	    
	    public abstract void put(Table_base item);
	    //public abstract void set(Table_base item);
	    public abstract void remove(Table_base item);
	    
	    public abstract Iterator iterator( RubyHash where);
	}	
	
	public static class Table_object_values extends Table_base { 
	    String attrib="";
	    String object="";
	    String value="";
	    String created_at="";
	    String updated_at="";
	    String update_type="";

		private static final RubyString ATTRIB = ObjectFactory.createString("attrib");
		private static final RubyString OBJECT = ObjectFactory.createString("object");
		private static final RubyString VALUE = ObjectFactory.createString("value");
		private static final RubyString CREATED_AT = ObjectFactory.createString("created_at");
		private static final RubyString UPDATED_AT = ObjectFactory.createString("updated_at");
		private static final RubyString UPDATE_TYPE = ObjectFactory.createString("update_type");
	    
		public static class TableRoot extends TableRootBase { 
		    Index source_idANDupdate_type;
		    Index object_idANDattrib;

		    // Deserialize the object
		    public void readObject(IInputStream in) {
		    	source_idANDupdate_type = (Index)in.readObject();
		    	object_idANDattrib = (Index)in.readObject();
		    }

		    // Serialize the object
		    public void writeObject(IOutputStream out) { 
		        out.writeObject(source_idANDupdate_type);
		        out.writeObject(object_idANDattrib);
		    }

		    public TableRoot() {}
		    public TableRoot(Storage db) { 
		        super(db);
		        
		        source_idANDupdate_type = db.createIndex(
		        		new int[]{Types.Int,Types.String}, false);
		        object_idANDattrib = db.createIndex(
		        		new int[]{Types.String,Types.String}, false);
		    }
		    public void clear(){
		    	Iterator iter = source_idANDupdate_type.iterator();
		    	while(iter.hasNext()){
		    		Table_object_values item = (Table_object_values)iter.next();
		    		iter.remove();
		    	}
		    	source_idANDupdate_type.clear();

		    	iter = object_idANDattrib.iterator();
		    	while(iter.hasNext()){
		    		
		    		Table_object_values item = (Table_object_values)iter.next();
		    		iter.remove();
		    		item.deallocate();
		    	}
		    	object_idANDattrib.clear();
		    }
		    
		    public void put(Table_base itemB){
		    	Table_object_values item = (Table_object_values)itemB;
		    	source_idANDupdate_type.put(new Key( new Object[]{new Integer(item.source_id), item.update_type}),item);
		    	object_idANDattrib.put(new Key( item.object, item.attrib),item);
		    }
		    public void remove(Table_base itemB){
		    	Table_object_values item = (Table_object_values)itemB;
		    	source_idANDupdate_type.remove(new Key( new Object[]{new Integer(item.source_id), item.update_type}),item);
		    	object_idANDattrib.remove(new Key( item.object, item.attrib),item);
		    	item.deallocate();
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
	    	out.writeString(attrib);
	    	out.writeString(object);
	    	out.writeString(value);
	    	out.writeString(created_at);
	    	out.writeString(updated_at);
	    	out.writeString(update_type);	    	
	    }

	    // Deserialize the object
	    public void readObject(IInputStream in) { 
	    	super.readObject(in);
	    	attrib = in.readString();
	    	object = in.readString();
	    	value = in.readString();
	    	created_at = in.readString();
	    	updated_at = in.readString();
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
	    	if ( bAll || name.equals(CREATED_AT) )
	    		res.add( CREATED_AT, ObjectFactory.createString(created_at) );
	    	if ( bAll || name.equals(UPDATED_AT) )
	    		res.add( UPDATED_AT, ObjectFactory.createString(updated_at) );
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
	        val = hash.getValue(CREATED_AT);
	        if ( val != RubyConstant.QNIL )
	        	created_at = val.toStr();	        
	        val = hash.getValue(UPDATED_AT);
	        if ( val != RubyConstant.QNIL )
	        	updated_at = val.toStr();	        
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
		    
		    public void put(Table_base item){
		    	source_id.put(new Key(item.source_id), item);
		    }
		    public void remove(Table_base item){
		    	source_id.remove(new Key(item.source_id),item);
		    	item.deallocate();
		    }
		    
		    public Iterator iterator(RubyHash where){
		    	if(where == null ) 
		    	{ 
		    		return source_id.iterator(); 
		    	}
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
    	createDB();
        return this;
    }
	
	void createDB() {
		if (isClosed() == RubyConstant.QFALSE) {
			return;
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
            
            root.put("sources", new Table_sources.TableRoot(m_storage) );
            root.put("object_values", new Table_object_values.TableRoot(m_storage) );
        }
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
			Table_base item = null;
			if ( strName.equals(Table_sources.name()) )
				item = new Table_sources(m_storage,(RubyHash)mapValues);
			else
				item = new Table_object_values(m_storage,(RubyHash)mapValues);
			
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
		RubyArray res = new RubyArray(); 
		
		RubyValue orderBy = null;
		boolean distinct = false;
		if ( params != null && params != RubyConstant.QNIL ){
			orderBy = ((RubyHash)params).get(ObjectFactory.createString("order by"));
			if ( orderBy == RubyConstant.QNIL )
				orderBy = null;
			distinct = ((RubyHash)params).get(ObjectFactory.createString("distinct")) != 
				RubyConstant.QNIL;
		}
		
		TableRootBase tblRoot = getTableRoot(tableName);
		if ( tblRoot != null ){
			Iterator iter = tblRoot.iterator((RubyHash)where);
			RubyHash distinctMap = distinct ? ObjectFactory.createHash() : null;
			while(iter != null && iter.hasNext()){
				Table_base item = (Table_base)iter.next();
				item.addOrdered( res, attrib, orderBy, distinctMap );
			}
		}
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
			Iterator iter = tblRoot.iterator((RubyHash)where);
			while(iter != null && iter.hasNext()){
				Table_base item = (Table_base)iter.next();
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
				Table_base item = (Table_base)iter.next();
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
