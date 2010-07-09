/**
 * Copyright 2005-2007 Xue Yong Zhi, Jie Li
 * Distributed under the BSD License
 */

package com.xruby.runtime.builtin;

import java.io.InputStream;

import com.rho.RhoRuby;
import com.xruby.runtime.lang.*;
//import com.xruby.runtime.lang.annotation.RubyLevelClass;
//import com.xruby.runtime.lang.annotation.RubyLevelMethod;

import j2me.io.File;
import j2me.math.HugeInt;

import j2me.lang.StringMe;

////@RubyLevelClass(name="File", superclass="IO")
public class RubyFile extends RubyIO {
    public RubyFile(String filename, String mode) {
        super(new RubyIOFileExecutor(filename, mode), RubyRuntime.FileClass);
    }
    public RubyFile() {
        super(null, RubyRuntime.FileClass);
    }

    public RubyFile(RubyIOExecutor executor, RubyClass klass) {
        super(executor, klass);
    }

    ////@RubyLevelMethod(name="file?", singleton=true)
    public static RubyValue file_question(RubyValue receiver, RubyValue arg) {
        String fileName = arg.toStr();
        File file = new File(fileName);
        return ObjectFactory.createBoolean(file.isFile());
    }

    ////@RubyLevelMethod(name="writable?", singleton=true)
    public static RubyValue writable_question(RubyValue receiver, RubyValue arg) {
        String fileName = arg.toStr();
        File file = new File(fileName);
        return ObjectFactory.createBoolean(file.canWrite());
    }

    ////@RubyLevelMethod(name="readable?", singleton=true)
    public static RubyValue readable_question(RubyValue receiver, RubyValue arg) {
        String fileName = arg.toStr();
        File file = new File(fileName);
        return ObjectFactory.createBoolean(file.canRead());
    }

    ////@RubyLevelMethod(name="exist?", alias="exists?", singleton=true)
    public static RubyValue exist_question(RubyValue receiver, RubyValue arg) {
        String fileName = arg.toStr();
        boolean bExist = false;
        if ( fileName.startsWith("/apps"))
        	bExist = RhoSupport.findClass(fileName)!=null;

        if ( !bExist )
        	bExist = RhoRuby.resourceFileExists(fileName);
        
        if ( !bExist )
        {
        	File file = new File(fileName);
        	bExist = file.exists();
        }
        
    	return ObjectFactory.createBoolean(bExist);
        
    }

    public static RubyValue existinjar_question(RubyValue receiver, RubyValue arg) 
    {
        String fileName = arg.toStr();
        boolean bExist = RhoSupport.findClass(fileName)!=null;
    	return ObjectFactory.createBoolean(bExist);
        
    }
    
    ////@RubyLevelMethod(name="directory?", singleton=true)
    public static RubyValue directory_question(RubyValue receiver, RubyValue arg) {
        String fileName = arg.toStr();
        File file = new File(fileName);
        return ObjectFactory.createBoolean(!file.isFile());
    }

    //@RubyLevelMethod(name="executable?", singleton=true)
    public static RubyValue executable_question(RubyValue receiver, RubyValue arg) {
        return RubyConstant.QTRUE;
    }

    //@RubyLevelMethod(name="expand_path", singleton=true)
    public static RubyValue expand_path(RubyValue receiver) {
        throw new RubyException(RubyRuntime.ArgumentErrorClass, "wrong number of arguments (0 for 1)");
    }

    //@RubyLevelMethod(name="expand_path", singleton=true)
    public static RubyValue expand_path(RubyValue receiver, RubyArray args) {
        String file_name = args.get(0).toStr();

        if (file_name.equals(".")) {
            file_name = "";
        }

        if (args.size() == 1) {
            File file = new File(file_name);
            return ObjectFactory.createString(file.getAbsolutePath().replace('\\', '/'));
        } else {
            if (file_name.startsWith("/")) {
                file_name = ".." + file_name;
            }
            StringBuffer dir_string = new StringBuffer(args.get(1).toStr());
            if (dir_string.toString().equals("/")) {
                dir_string = new StringBuffer((new File("/")).getAbsolutePath().replace('\\', '/'));
            }

            while (file_name.startsWith("../")) {
                int i = dir_string.toString().lastIndexOf('/');
                if (i >= 0) {
                    dir_string.delete(i, dir_string.length());
                }
                file_name = file_name.substring(3, file_name.length());
            }
            if (file_name.length() > 0 || dir_string.toString().endsWith(":")) {
                if (!dir_string.toString().endsWith("/")) {
                    dir_string.append("/");
                }
                dir_string.append(file_name);
            }
            return ObjectFactory.createString(dir_string.toString().replace('\\', '/'));
        }
    }

    //@RubyLevelMethod(name="dirname", singleton=true)
    public static RubyValue dirname(RubyValue receiver, RubyValue arg) {
        String fileName = arg.toStr();
        File file = new File(fileName);
        String parent = file.getParent();
        if (parent == null) {
            return ObjectFactory.createString(StringMe.matches(fileName, "\\/+") ? "/" : ".");
        }

        //Java's File.getParent() always converts '/' to '\\' on windows. This is not
        //what we want, so here we hack the result with replace().
        return ObjectFactory.createString(parent.replace('\\', '/'));
    }

    //@RubyLevelMethod(name="mtime", singleton=true)
    public static RubyValue mtime(RubyValue receiver, RubyValue arg) {
        String fileName = arg.toStr();
        File file = new File(fileName);
        if (!file.isFile() && !file.isDirectory()) {
            throw new RubyException(RubyRuntime.RuntimeErrorClass, "No such file or directory - " + fileName);
        }
        return ObjectFactory.createTime(file.lastModified());
    }

    //@RubyLevelMethod(name="size", singleton=true)
    public static RubyValue size(RubyValue receiver, RubyValue arg) {
        String fileName = arg.toStr();
    	InputStream stream = null;
        
        if ( fileName.startsWith("/apps"))
        {
			try {
				stream = RhoRuby.loadFile("/" + fileName);
				if ( stream != null )
				{
		        	RubyValue res = ObjectFactory.createInteger(stream.available());
		        	
		        	return res;
				}
			} catch (Exception e) {
				e.printStackTrace();
			}finally
			{
				try{ if ( stream != null ) stream.close(); }catch(java.io.IOException exc){}
			}
        }
        
        File file = new File(fileName);
        if (!file.isFile() && !file.isDirectory()) {
            throw new RubyException(RubyRuntime.RuntimeErrorClass, "No such file or directory - " + fileName);
        }
        return RubyBignum.bignorm(HugeInt.valueOf(file.length()));
    }

    //@RubyLevelMethod(name="rename", singleton=true)
    public static RubyValue rename(RubyValue receiver, RubyValue arg1, RubyValue arg2) {
        String file1 = arg1.toStr();
        String file2 = arg2.toStr();
        File file = new File(file1);
        if (!file.isFile() && !file.isDirectory()) {
            throw new RubyException(RubyRuntime.RuntimeErrorClass, "No such file or directory - " + file1);
        }
        return ObjectFactory.createBoolean(file.renameTo(new File(file2)));
    }

    //@RubyLevelMethod(name="utime", singleton=true)
    public static RubyValue utime(RubyValue receiver, RubyArray args) {
        if (args.size() < 3) {
            throw new RubyException(RubyRuntime.ArgumentErrorClass, "wrong number of arguments (" + args.length() + " for 2)");
        }

        //RubyTime atime = (RubyTime)args.get(0);
        RubyTime mtime = (RubyTime)args.get(1);
        for (int i = 2; i < args.size(); ++i) {
            String filename = args.get(i).toStr();
            File f = new File(filename);
            f.setLastModified(mtime.getTime());
        }

        return ObjectFactory.createFixnum(args.size() - 2);
    }

    //@RubyLevelMethod(name="truncate")
    public RubyValue truncate(RubyValue arg) {
        truncate(arg.toInt());
        return ObjectFactory.FIXNUM0;
    }

    //@RubyLevelMethod(name="separator", singleton=true)
    public static RubyValue separator(RubyValue receiver) {
        return ObjectFactory.createString(File.separator);
    }

    //@RubyLevelMethod(name="delete", alias="unlink", singleton=true)
    public static RubyValue delete(RubyValue receiver, RubyArray args) {
        int deleted = 0;
        if (args != null) {
            for (int i = 0; i < args.size(); ++i) {
                String fileName = args.get(i).toStr();
                File file = new File(fileName);
                if (file.isDirectory()) {
                    throw new RubyException(RubyRuntime.RuntimeErrorClass, "Is a directory - " + fileName);
                } else if (file.isFile()) {
                    file.delete();
                    ++deleted;
                } else {
                    throw new RubyException(RubyRuntime.RuntimeErrorClass, "No such file or directory - " + fileName);
                }
            }
        }
        return ObjectFactory.createFixnum(deleted);
    }

    //@RubyLevelMethod(name="basename", singleton=true)
    public static RubyValue basename(RubyValue receiver, RubyArray args) {
        String fileName = args.get(0).toStr();
        String basename;
        if ("".equals(fileName)) {
            basename = "";
        } else {
            basename = new File(fileName).getName();
            if (0 == basename.length()) {
                basename = "/";
            }
        }

        if (args.size() == 1) {
            return ObjectFactory.createString(basename);
        }

        String suffix = args.get(1).toStr();
        if (suffix.equals(".*")) {
            int dot_position = basename.lastIndexOf('.');
            if (dot_position < 0) {
                return ObjectFactory.createString(basename);
            } else {
                return ObjectFactory.createString(basename.substring(0, dot_position));
            }
        }
        if (basename.endsWith(suffix)) {
            return ObjectFactory.createString(basename.substring(0, basename.length() - suffix.length()));
        } else {
            return ObjectFactory.createString(basename);
        }
    }

    //@RubyLevelMethod(name="open", singleton=true)
    public static RubyValue open(RubyValue receiver, RubyArray args, RubyBlock block) {
        return com.xruby.runtime.lang.RubyKernelModule.open(receiver, args, block);
    }

    //@RubyLevelMethod(name="initialize")
    public RubyValue initialize(RubyArray args, RubyBlock block) 
    {
    	RubyValue val = com.xruby.runtime.lang.RubyKernelModule.open(this, args, block);
    	if( val instanceof RubyIO)
    	{
    		initIO((RubyIO)val);
    	}
        return this;
    }
    
    //@RubyAllocMethod
    public static RubyValue alloc(RubyValue receiver) {
        RubyFile h = new RubyFile();
        h.setRubyClass((RubyClass) receiver);
        return h;
    }
    
    //@RubyLevelMethod(name="extname", singleton=true)
    public static RubyValue extname(RubyValue receiver, RubyValue arg) {
        String filename = arg.toStr();
        String ext = "";

        int dotIndex = filename.lastIndexOf('.');
        if (dotIndex > 0 && dotIndex != (filename.length() - 1)) {
        	ext = filename.substring(dotIndex);
        }

        return ObjectFactory.createString(ext);
    }
    
}
