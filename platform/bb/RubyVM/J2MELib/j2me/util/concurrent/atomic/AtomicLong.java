package j2me.util.concurrent.atomic;
public class AtomicLong  {
        private volatile long value;
        
    public AtomicLong(long initialValue) {
        value = initialValue;
    }
    public AtomicLong() {
    }
    public final long get() {
        return value;
    }
    public final void set(long newValue) {
        value = newValue;
    }

    public final long getAndIncrement() {
        while (true) {
            long current = get();
            long next = current + 1;
            set(next);
            //TODO: getAndIncrement
//            if (compareAndSet(current, next))
                return current;
        }
    }
    
}
