/**
 * Copyright 2005-2007 Yu Zhang
 * Distributed under the BSD License
 */

package com.xruby.runtime.builtin;

import java.util.Vector;

import j2me.io.File;
import j2me.util.ArrayList;
import j2me.util.Iterator;
import j2me.util.List;
import j2me.lang.SystemMe;
//import org.apache.oro.io.GlobFilenameFilter;

import com.xruby.runtime.lang.RhoSupport;
import com.xruby.runtime.lang.RubyBasic;
import com.xruby.runtime.lang.RubyBlock;
import com.xruby.runtime.lang.RubyClass;
import com.xruby.runtime.lang.RubyConstant;
import com.xruby.runtime.lang.RubyException;
import com.xruby.runtime.lang.RubyRuntime;
import com.xruby.runtime.lang.RubyValue;

////@RubyLevelClass(name="Dir", modules="Enumerable")
public class RubyDir extends RubyBasic {
    private File dir_;
    private boolean isOpen = true;
    private int curPos;
    private String path;
    private List/*<String>*/ list = new ArrayList/*<String>*/();

    public RubyDir(String path){
        super(RubyRuntime.DirClass);
        this.path = path;
        dir_ = new File(path);

        list.add(".");
        list.add("..");
        Vector contents = dir_.list();
        if(contents != null)
        {
            for(int i=0;i<contents.size();i++)
            {
                list.add(contents.elementAt(i));
            }
        }
        curPos = 0;
    }

    public RubyValue clone(){
    	RubyDir cl = new RubyDir(path);
    	cl.doClone(this);
    	return cl;
    }
    
    private boolean isDirectory(){
        return dir_.isDirectory();
    }

    private String read_dir(){
        if(curPos >= list.size())
            return null;

        String tmp = (String)list.get(curPos);
        curPos++;
        return tmp;
    }

    private void setPos(int pos){
        curPos = pos;
    }

    private static RubyValue glob(RubyValue receiver, RubyValue arg) {
        /*String s = arg.toStr();
        String dir;
        String pattern;
        int index = s.lastIndexOf('/');
        if (index < 0) {
            dir = ".";
            pattern = s;

        } else {
            dir = s.substring(0, index);
            pattern = s.substring(index + 1);
        }

        RubyArray a = new RubyArray();
        File file = new File(dir);
        String[] files = file.list(new GlobFilenameFilter(pattern));
        if (null != files) {
//            for (String f : files) {
            for( int i = 0; i < files.length; i++ ){
                a.add(ObjectFactory.createString(dir + "/" + files[i]));
            }
        }
        return a;*/
    	
    	throw new RuntimeException("glob is not implemented");
    }

    private static RubyValue glob_with_block(RubyValue receiver, RubyValue arg, RubyBlock block) {
        /*String s = arg.toStr();
        String dir;
        String pattern;
        int index = s.lastIndexOf('/');
        if (index < 0) {
            dir = ".";
            pattern = s;

        } else {
            dir = s.substring(0, index);
            pattern = s.substring(index + 1);
        }

        File file = new File(dir);
        String[] files = file.list(new GlobFilenameFilter(pattern));
        if (null != files) {
//            for (String f : files) {
        	for( int i = 0; i < files.length; i++ ){        	
                block.invoke(receiver, ObjectFactory.createString(dir + "/" + files[i]));
            }
        }
        return RubyConstant.QNIL;*/
    	throw new RuntimeException("glob_with_block is not implemented");
    }

    ////@RubyLevelMethod(name="close")
    public RubyValue close(){
        isOpen = false;
        return RubyConstant.QNIL;
    }

    ////@RubyLevelMethod(name="rewind")
    public RubyValue rewind() {
        setPos(0);
        return RubyConstant.QNIL;
    }

    //@RubyLevelMethod(name="pos", alias="tell")
    public RubyFixnum pos(){
        return ObjectFactory.createFixnum(curPos);
    }

    //@RubyLevelMethod(name="path")
    public RubyString getPath(){
        return ObjectFactory.createString(path);
    }

    //@RubyLevelMethod(name="each")
    public RubyValue each(RubyBlock block){
//        for (String item : list) {
        for (Iterator iter = list.iterator(); iter.hasNext();) {
        	String item = (String)iter.next();
            RubyValue v = block.invoke(this, ObjectFactory.createString(item));
            if (block.breakedOrReturned()) {
                return v;
            }
        }
        return this;
    }

    //@RubyLevelMethod(name="seek")
    public RubyValue seek(RubyValue arg) {
        int pos = arg.toInt();
        setPos(pos);
        return this;
    }

    //@RubyLevelMethod(name="pos=")
    public RubyValue setPos(RubyValue arg) {
        int pos = arg.toInt();
        setPos(pos);
        return arg;
    }

    //@RubyLevelMethod(name="read")
    public RubyValue read() {
        if(!isOpen){
            throw new RubyException(RubyRuntime.IOErrorClass,"closed directory");
        }

        String str = read_dir();
        if(str == null)
            return RubyConstant.QNIL;
        else
            return ObjectFactory.createString(str);
    }

    //@RubyLevelMethod(name="new", singleton=true)
    public static RubyValue dirNew(RubyValue receiver, RubyValue arg) {
        String path = arg.toStr();
        RubyDir dir = new RubyDir(path);
        if (!dir.isDirectory()) {
            throw new RubyException((RubyClass)RubyRuntime.ErrnoModule.getConstant("ENOENT"), "No such directory - " + path);
        }
        dir.setRubyClass((RubyClass)receiver);
        return dir;
    }

    //@RubyLevelMethod(name="foreach", singleton=true)
    public static RubyValue foreach(RubyValue receiver, RubyValue arg, RubyBlock block) {
        String path = arg.toStr();
        RubyDir dir = new RubyDir(path);
        return dir.each(block);
    }

    //@RubyLevelMethod(name="chdir", singleton=true)
    public static RubyValue chdir(RubyValue receiver, RubyValue arg) {
        String dir = arg.toStr();
        File file = new File(dir);
        if (!file.isDirectory()) {
            throw new RubyException(RubyRuntime.RuntimeErrorClass, "No a directory - " + dir);
        }
        SystemMe.setProperty("user.dir", file.getAbsolutePath());
        return ObjectFactory.FIXNUM0;
    }

    //@RubyLevelMethod(name="getwd", alias="pwd", singleton=true)
    public static RubyValue getwd(RubyValue receiver) {
        return ObjectFactory.createString(System.getProperty("user.dir"));
    }

    //@RubyLevelMethod(name="mkdir", singleton=true)
    public static RubyValue mkdir(RubyValue receiver, RubyValue arg) {
        String dir = arg.toStr();
        File file = new File(dir, "rw");
        if (file.isDirectory() || file.mkdir()) {
            return ObjectFactory.FIXNUM0;
        }
        throw new RubyException(RubyRuntime.RuntimeErrorClass, "Can't create directory - " + dir);
    }

    //@RubyLevelMethod(name="rmdir", alias={"delete", "unlink"}, singleton=true)
    public static RubyValue rmdir(RubyValue receiver, RubyValue arg) {
        String dir = arg.toStr();
        File file = new File(dir);
        if (!file.isDirectory()) {
            throw new RubyException((RubyClass)RubyRuntime.ErrnoModule.getConstant("ENOENT"), "Not a directory - " + dir);
        }
        if (file.delete()) {
            return ObjectFactory.FIXNUM0;
        }
        throw new RubyException((RubyClass)RubyRuntime.ErrnoModule.getConstant("ENOENT"), "Can't delete directory - " + dir);
    }

    //@RubyLevelMethod(name="entries", singleton=true)
    public static RubyValue entries(RubyValue receiver, RubyValue arg, RubyBlock block) {
        String dir = arg.toStr();
        File file = new File(dir);
        if (!file.isDirectory()) {
            throw new RubyException(RubyRuntime.RuntimeErrorClass, "Not a directory - " + dir);
        }

        RubyArray files = new RubyArray();
        files.add(ObjectFactory.createString("."));
        files.add(ObjectFactory.createString(".."));
        Vector arFiles = file.list();
        for( int i = 0; i < arFiles.size(); i++ )
        {        
            files.add(ObjectFactory.createString((String)arFiles.elementAt(i)));
        }

        return files;
    }

    //@RubyLevelMethod(name="open", singleton=true)
    public static RubyValue open(RubyValue receiver, RubyValue arg, RubyBlock block) {
        String path = arg.toStr();
        RubyDir dir = new RubyDir(path);
        if (!dir.isDirectory()) {
            throw new RubyException((RubyClass)RubyRuntime.ErrnoModule.getConstant("ENOENT"), "No such directory - " + path);
        }
        dir.setRubyClass((RubyClass)receiver);

        if (null == block) {
            return dir;
        } else {
            RubyValue v = block.invoke(receiver, dir);
            dir.close();
            return v;
        }
    }

    //@RubyLevelMethod(name="glob", singleton=true)
    public static RubyValue glob(RubyValue receiver, RubyValue arg, RubyBlock block) {
        if (null == block) {
            return glob(receiver, arg);
        } else {
            return glob_with_block(receiver, arg, block);
        }
    }

    //@RubyLevelMethod(name="[]", singleton=true)
    public static RubyValue array_access(RubyValue receiver, RubyValue arg) {
        return glob(receiver, arg);
    }

    ////@RubyLevelMethod(name="exist?", alias="exists?", singleton=true)
    public static RubyValue exist_question(RubyValue receiver, RubyValue arg) {
        String fileName = arg.toStr();
        
    	File file = new File(fileName);
    	boolean bExist = file.isDirectory();
        
    	return ObjectFactory.createBoolean(bExist);
    }
    
}
