/**
 * Copyright 2005-2007 Xue Yong Zhi, Yu Zhang
 * Distributed under the BSD License
 */

package com.xruby.runtime.builtin;

import j2me.util.*;
//import java.util.Vector;
import j2me.util.concurrent.locks.ReentrantLock;
import j2me.lang.ThreadLocal;
import j2me.util.LinkedList;

import com.xruby.runtime.lang.*;
//import com.xruby.runtime.lang.annotation.RubyLevelClass;
//import com.xruby.runtime.lang.annotation.RubyLevelMethod;

/**
 * Implement ruby thread in native thread,not green thread.As you
 * know,YARV will make use of native thread too.
 *
 * Current implementation is not accurate.If you find any question,
 * please don't hesitate to inform us.
 */

//@RubyLevelClass(name="Thread")
public class RubyThread extends RubyBasic {

    private final Thread thread_;
    private ThreadLocal/*<HashMap<String, RubyValue>>*/ thread_local_variables_ =
            new ThreadLocal/*<HashMap<String, RubyValue>>*/() {
                protected synchronized Object/*HashMap<String, RubyValue>*/ initialValue() {
                    return new HashMap/*<String, RubyValue>*/();
                }
            };
    private RubyThreadGroup threadGroup;
    private static List/*<RubyThread>*/ activeThreads = new LinkedList/*<RubyThread>*/();
    private static Map/*<Thread, RubyThread>*/ threadMapper = new HashMap/*<Thread, RubyThread>*/();
    private static RubyThread mainThread;

    private ReentrantLock lock = new ReentrantLock();
    private volatile boolean killed = false;
    private static volatile boolean stoped = false;

    private static final Object stopLock = new Object();
    private static final ReentrantLock criticalLock = new ReentrantLock();

    private RubyValue result = RubyConstant.QNIL;

    RubyThread(final RubyBlock block) {
        super(RubyRuntime.ThreadClass);
        thread_ = new Thread(new Runnable() {
            public void run() {
                RubyThreadGroup.defaultThreadGroup.add(RubyThread.this);
                activeThreads.add(RubyThread.this);
                result = block.invoke(RubyConstant.QNIL);
                activeThreads.remove(RubyThread.this);
                RubyThreadGroup.defaultThreadGroup.add(RubyThread.this);
                threadMapper.remove(RubyThread.this.thread_);
            }
        });
        //RHO_COMMENT: thread_.setDaemon(true)
        //thread_.setDaemon(true);
        thread_.start();
        threadMapper.put(thread_, this);
    }

    private RubyThread(Thread thread) {
        super(RubyRuntime.ThreadClass);
        thread_ = thread;
        threadMapper.put(thread_, this);
    }

    public RubyValue clone(){
    	RubyThread cl = new RubyThread(thread_);
    	cl.thread_local_variables_ = thread_local_variables_;
    	cl.threadGroup = threadGroup;
    	cl.result = result; 
    	
    	cl.doClone(this);
    	return cl;
    }
    
    //Only called from <code>ThreadClassBuilder</code> to add main thread.
    public static void init(){
        //Assume that this method is called from main thread
        mainThread = new RubyThread(Thread.currentThread());
        activeThreads.add(mainThread);
        RubyThreadGroup.defaultThreadGroup.add(mainThread);
    }

    //c ruby does not have this method, but since we wrapped java's Thread, can no longer use java's == operator
    //@RubyLevelMethod(name="==")
    public RubyValue equal(RubyValue arg) {
        return ObjectFactory.createBoolean(equals(arg));
    }



    /*
     * It is different from ruby impl.Default value is not zero,and it
     * depends on OS.
     */
    //@RubyLevelMethod(name="priority")
    public RubyValue priority() {
        return ObjectFactory.createFixnum(thread_.getPriority());
    }

    //@RubyLevelMethod(name="priority=")
    public RubyValue set_priority(RubyValue arg) {
        int val = arg.toInt();
        if (val < Thread.MIN_PRIORITY) {
            val = Thread.MIN_PRIORITY;
        } else if (val > Thread.MAX_PRIORITY) {
            val = Thread.MAX_PRIORITY;
        }
        thread_.setPriority(val);
        return this;
    }

    //@RubyLevelMethod(name="alive?")
    public RubyValue alive() {
        return ObjectFactory.createBoolean(thread_.isAlive());
    }

    //@RubyLevelMethod(name="join")
    public RubyValue join(RubyArray args, RubyBlock block) {
        long timeout = 0;
        if(args != null){
            if(args.size() == 1)
                timeout = RubyTypesUtil.convertToJavaLong(args.get(0))*1000;
        }

        if(this.isCurrentThread()){
            throw new RubyException(RubyRuntime.ThreadErrorClass,"Thread tried to join itself!");
        }
        if(RubyThread.criticalLock.isHeldByCurrentThread()){
            thread_.interrupt();
        }
        join(timeout);
        return this;
    }

    //@RubyLevelMethod(name="current", singleton=true)
    public static RubyThread current(RubyValue receiver) {
        return (RubyThread)threadMapper.get(Thread.currentThread());
    }

    //@RubyLevelMethod(name="new", alias={"fork", "start"}, singleton=true)
    public static RubyValue newThread(RubyValue receiver, RubyArray args, RubyBlock block) {
        return ObjectFactory.createThread(block);
    }

    //@RubyLevelMethod(name="[]")
    public RubyValue getVariable(RubyValue name) {
        RubyValue v = (RubyValue)((HashMap)thread_local_variables_.get()).get(name.toStr());
        if (null == v) {
            return RubyConstant.QNIL;
        }

        return v;
    }

    //@RubyLevelMethod(name="[]=")
    public RubyValue setVariable(RubyValue arg1, RubyValue arg2) {
        ((HashMap)thread_local_variables_.get()).put(arg1.toStr(), arg2);
        return arg2;
    }

    //@RubyLevelMethod(name="inspect")
    public RubyValue rubyInspect() {
        StringBuffer sb = new StringBuffer();
        sb.append("#<");
        sb.append(getRubyClass().getRealClass().getName());
        sb.append(":0x");
        int hash = hashCode();
        sb.append(Integer.toHexString(hash));

        sb.append(" ");
        if(thread_.isAlive()){
            if(getKilled()){
                sb.append("dead");
            }else{
                sb.append("run");
            }
        }else{
            sb.append("dead");
        }
        sb.append(">");
        return ObjectFactory.createString(sb.toString());
    }



    //@RubyLevelMethod(name="group")
    public RubyValue getThreadGroup(){
        if(threadGroup == null)
            return RubyConstant.QNIL;
        else
            return threadGroup;
    }

    /**
     * Directly invoke native thread's destory method is not
     * valid because this method is not implemented in JDK.
     *
     * Killing one thread leave its locks and resources in an
     * unpredictable state is not advisable.It would be deadlock-prone.
     * If the target thread held a lock protecting a critical system
     * resource when it was killed, no thread could ever access this
     * resource again.If another thread ever attempted to lock this
     * resource, deadlock would result.
     *
     * Implements this by setting a kill flag on the target thread and
     * waiting for it to die.Charles Nutter has discussed this question.
     */
    //@RubyLevelMethod(name="kill", alias="exit")
    public RubyValue kill() {
        setKilled();
        //make the thread escaping from wait or block status
        thread_.interrupt();
        try {
            thread_.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        return this;
    }

    //@RubyLevelMethod(name="pass", singleton=true)
    public static RubyValue pass(RubyValue receiver) {
        boolean critical = RubyThread.criticalLock.isHeldByCurrentThread();
        RubyThread.setCritical(false);
        Thread.yield();
        RubyThread.setCritical(critical);
        return RubyConstant.QNIL;
    }

    //@RubyLevelMethod(name="main", singleton=true)
    public static RubyValue getMainThread(RubyValue receiver){
        return mainThread;
    }

    //RHO
    public static boolean isMainThread(){
        return mainThread.thread_ == Thread.currentThread();
    }
    //RHO
    
    //@RubyLevelMethod(name="stop?")
    public RubyValue getStoped(){
        return ObjectFactory.createBoolean(stoped);
    }

    //@RubyLevelMethod(name="run")
    public RubyValue run() {
        synchronized(RubyThread.stopLock){
            RubyThread.setStoped(false);
            RubyThread.stopLock.notifyAll();
        }
        return this;
    }

    //@RubyLevelMethod(name="status")
    public RubyValue status() {
        if (getStoped() == RubyConstant.QTRUE){
            return ObjectFactory.createString("sleep");
        } else if(getKilled()){
            return RubyConstant.QFALSE;
        } else{
            return ObjectFactory.createString("run");
        }
    }

    //@RubyLevelMethod(name="wakup")
    public RubyValue wakup() {
        synchronized(RubyThread.stopLock){
            RubyThread.setStoped(false);
            RubyThread.stopLock.notifyAll();
        }
        return this;
    }

    //@RubyLevelMethod(name="value")
    public RubyValue value() {
        join(0);
        return getResult();
    }

    //@RubyLevelMethod(name="key?")
    public RubyValue is_key(RubyValue arg) {
        String key = arg.toStr();
        return ObjectFactory.createBoolean(getVariables().contains(key));
    }

    //@RubyLevelMethod(name="keys")
    public RubyValue keys() {
        Set set = getVariables();
        Iterator iter = set.iterator();
        RubyArray result = new RubyArray(set.size());
        while(iter.hasNext()){
            result.add(ObjectFactory.createString((String)iter.next()));
        }
        return result;
    }

    //@RubyLevelMethod(name="list", singleton=true)
    public static RubyValue list(RubyValue receiver) {
        List list = RubyThread.getThreadList();
        Iterator iter = list.iterator();
        RubyArray result = new RubyArray(0);
        while(iter.hasNext()){
            RubyThread rt = (RubyThread)iter.next();
            if(rt.thread_.isAlive())
                result.add(rt);
        }
        return result;
    }

    //@RubyLevelMethod(name="stop", singleton=true)
    public static RubyValue stop(RubyValue receiver) {
        synchronized(RubyThread.stopLock){
            RubyThread.setStoped(true);
            try {
                RubyThread.setCritical(false);
                RubyThread.stopLock.wait();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        RubyThread.setStoped(false);
        return RubyConstant.QNIL;
    }

    //@RubyLevelMethod(name="critical", singleton=true)
    public static RubyValue critical(RubyValue receiver, RubyValue arg) {
        return ObjectFactory.createBoolean(RubyThread.criticalLock.isHeldByCurrentThread());
    }

    //@RubyLevelMethod(name="critical=", singleton=true)
    public static RubyValue set_critical(RubyValue receiver, RubyValue arg) {
        boolean critical = false;
        if(arg == RubyConstant.QTRUE){
            critical = true;
        }
        RubyThread.setCritical(critical);
        return arg;
    }

    public void setThreadGroup(RubyThreadGroup group){
        threadGroup = group;
    }

    private static List getThreadList(){
        return activeThreads;
    }

    private static void setCritical(boolean critical){
        if (RubyThread.criticalLock.isHeldByCurrentThread()) {
            if (!critical) {
                RubyThread.criticalLock.unlock();
            }
        } else {
            if (critical) {
                RubyThread.criticalLock.lock();
            }
        }
    }

    public boolean equals(Object o) {
        if (null == o) {
            return false;
        } else if (o instanceof RubyThread) {
            return thread_ == ((RubyThread) o).thread_;
        } else {
            return false;
        }
    }

    //Invoked by KernelModuleBuilder's sleep.
    public static void sleep(long milliseconds){
        synchronized(stopLock){
            stoped = true;
            try {
                stopLock.wait(milliseconds);
            } catch (InterruptedException e) {
                e.printStackTrace();
            } finally{
                stoped = false;
            }
        }
    }

    private Set getVariables(){
        return ((HashMap)thread_local_variables_.get()).keySet();
    }

    private RubyValue getResult(){
        return result;
    }

    private void join(long timeout) {
        try {
        	//RHO_COMMENT: join(/*timeout*/)
            thread_.join(/*timeout*/);
        } catch (InterruptedException e) {
            throw new Error(e.toString());
        }
    }

    private boolean isCurrentThread(){
        return thread_ == Thread.currentThread();
    }

    private void setKilled(){
        lock.lock();
        try{
            killed = true;
        }finally{
            lock.unlock();
        }
    }

    private boolean getKilled(){
        return killed;
    }

    private static void setStoped(boolean value){
        stoped = value;
    }

}
