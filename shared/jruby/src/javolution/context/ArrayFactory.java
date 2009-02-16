/*
 * Javolution - Java(TM) Solution for Real-Time and Embedded Systems
 * Copyright (C) 2007 - Javolution (http://javolution.org/)
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software is
 * freely granted, provided that this notice is preserved.
 */
package javolution.context;

/**
 * <p> This class holds factories to produces arrays of variable length.
 *     It allows for object recycling, pre-allocation and {@link StackContext
 *     stack} allocations:[code]
 *     // Primitive types.
 *     char[] buffer = ArrayFactory.CHARS_FACTORY.array(1024); // Possibly recycled.
 *     for (int i = reader.read(buffer, 0, buffer.length); i > 0;) {
 *         ...
 *     }
 *     ArrayFactory.CHARS_FACTORY.recycle(buffer); //  
 *
 *     // Custom types.
 *     static ArrayFactory<Vertex[]> VERTICES_FACTORY = new ArrayFactory<Vertex[]> {
 *         protected Vertex[] create(int size) {
 *             return new Vertex[size];
 *         }
 *     };
 *     ...
 *     Vertex[] vertices = VERTICES_FACTORY.array(256);
 *     [/code]</p>
 *          
 * @author  <a href="mailto:jean-marie@dautelle.com">Jean-Marie Dautelle</a>
 * @version 5.0, May 5, 2007
 */
public abstract class ArrayFactory/*<T>*/{

    /**
     * Holds factory for <code>boolean</code> arrays.
     */
    public static final ArrayFactory/*<boolean[]>*/BOOLEANS_FACTORY = new ArrayFactory() {
        protected Object create(int size) {
            return new boolean[size];
        }

        public void recycle(Object array) {
            recycle(array, ((boolean[]) array).length);
        }
    };

    /**
     * Holds factory for <code>byte</code> arrays.
     */
    public static final ArrayFactory/*<byte[]>*/BYTES_FACTORY = new ArrayFactory() {
        protected Object create(int size) {
            return new byte[size];
        }

        public void recycle(Object array) {
            recycle(array, ((byte[]) array).length);
        }
    };

    /**
     * Holds factory for <code>char</code> arrays.
     */
    public static final ArrayFactory/*<char[]>*/CHARS_FACTORY = new ArrayFactory() {
        protected Object create(int size) {
            return new char[size];
        }

        public void recycle(Object array) {
            recycle(array, ((char[]) array).length);
        }
    };

    /**
     * Holds factory for <code>short</code> arrays.
     */
    public static final ArrayFactory/*<short[]>*/SHORTS_FACTORY = new ArrayFactory() {
        protected Object create(int size) {
            return new short[size];
        }

        public void recycle(Object array) {
            recycle(array, ((short[]) array).length);
        }
    };

    /**
     * Holds factory for <code>int</code> arrays.
     */
    public static final ArrayFactory/*<int[]>*/INTS_FACTORY = new ArrayFactory() {
        protected Object create(int size) {
            return new int[size];
        }

        public void recycle(Object array) {
            recycle(array, ((int[]) array).length);
        }
    };

    /**
     * Holds factory for <code>long</code> arrays.
     */
    public static final ArrayFactory/*<long[]>*/LONGS_FACTORY = new ArrayFactory() {
        protected Object create(int size) {
            return new long[size];
        }

        public void recycle(Object array) {
            recycle(array, ((long[]) array).length);
        }
    };

    /**
     * Holds factory for <code>float</code> arrays.
     */
    /*@JVM-1.5+@
     public static final ArrayFactory
     /*<float[]>*/
    /*@JVM-1.5+@
     FLOATS_FACTORY = new ArrayFactory() {
     protected Object create(int size) {
     return new float[size];
     }
     public void recycle(Object array) {
     recycle(array, ((float[])array).length);
     }        
     };
     /**/

    /**
     * Holds factory for <code>double</code> arrays.
     */
    /*@JVM-1.5+@
     public static final ArrayFactory
     /*<double[]>*/
    /*@JVM-1.5+@
     DOUBLES_FACTORY = new ArrayFactory() {
     protected Object create(int size) {
     return new double[size];
     }
     public void recycle(Object array) {
     recycle(array, ((double[])array).length);
     }        
     };
     /**/

    /**
     * Holds factory for generic <code>Object</code> arrays.
     */
    public static final ArrayFactory/*<Object[]>*/OBJECTS_FACTORY = new ArrayFactory() {
        protected Object create(int size) {
            return new Object[size];
        }

        public void recycle(Object array) {
            recycle(array, ((Object[]) array).length);
        }
    };

    /**
     * Holds factory for arrays up to size 4.
     */
    private final ObjectFactory _factory4 = new ObjectFactory() {
        protected Object create() {
            return ArrayFactory.this.create(4);
        }
    };

    /**
     * Holds factory for arrays up to size 8.
     */
    private final ObjectFactory _factory8 = new ObjectFactory() {
        protected Object create() {
            return ArrayFactory.this.create(8);
        }
    };

    /**
     * Holds factory for arrays up to size 16.
     */
    private final ObjectFactory _factory16 = new ObjectFactory() {
        protected Object create() {
            return ArrayFactory.this.create(16);
        }
    };

    /**
     * Holds factory for arrays up to size 32.
     */
    private final ObjectFactory _factory32 = new ObjectFactory() {
        protected Object create() {
            return ArrayFactory.this.create(32);
        }
    };

    /**
     * Holds factory for arrays up to size 64.
     */
    private final ObjectFactory _factory64 = new ObjectFactory() {
        protected Object create() {
            return ArrayFactory.this.create(64);
        }
    };

    /**
     * Holds factory for arrays up to size 128.
     */
    private final ObjectFactory _factory128 = new ObjectFactory() {
        protected Object create() {
            return ArrayFactory.this.create(128);
        }
    };

    /**
     * Holds factory for arrays up to size 256.
     */
    private final ObjectFactory _factory256 = new ObjectFactory() {
        protected Object create() {
            return ArrayFactory.this.create(256);
        }
    };

    /**
     * Holds factory for arrays up to size 512.
     */
    private final ObjectFactory _factory512 = new ObjectFactory() {
        protected Object create() {
            return ArrayFactory.this.create(512);
        }
    };

    /**
     * Holds factory for arrays up to size 1024.
     */
    private final ObjectFactory _factory1024 = new ObjectFactory() {
        protected Object create() {
            return ArrayFactory.this.create(1024);
        }
    };

    /**
     * Holds factory for arrays up to size 2048.
     */
    private final ObjectFactory _factory2048 = new ObjectFactory() {
        protected Object create() {
            return ArrayFactory.this.create(2048);
        }
    };

    /**
     * Holds factory for arrays up to size 4096.
     */
    private final ObjectFactory _factory4096 = new ObjectFactory() {
        protected Object create() {
            return ArrayFactory.this.create(4096);
        }
    };

    /**
     * Holds factory for arrays up to size 8192.
     */
    private final ObjectFactory _factory8192 = new ObjectFactory() {
        protected Object create() {
            return ArrayFactory.this.create(8192);
        }
    };

    /**
     * Holds factory for arrays up to size 16384.
     */
    private final ObjectFactory _factory16384 = new ObjectFactory() {
        protected Object create() {
            return ArrayFactory.this.create(16384);
        }
    };

    /**
     * Holds factory for arrays up to size 32768.
     */
    private final ObjectFactory _factory32768 = new ObjectFactory() {
        protected Object create() {
            return ArrayFactory.this.create(32768);
        }
    };

    /**
     * Holds factory for arrays up to size 65536.
     */
    private final ObjectFactory _factory65536 = new ObjectFactory() {
        protected Object create() {
            return ArrayFactory.this.create(65536);
        }
    };

    // Above 65536 we use the heap exclusively. 

    /**
     * Default constructor.
     */
    public ArrayFactory() {
    }

    /**
     * Returns an array possibly recycled or preallocated of specified 
     * minimum size.
     * 
     * @param capacity the minimum size of the array to be returned.
     * @return a recycled, pre-allocated or new factory array.
     */
    public final Object/*{T}*/array(int capacity) {
        return (capacity <= 4) ? (Object/*{T}*/) _factory4.object()
                : largeArray(capacity);
    }

    private final Object/*{T}*/largeArray(int capacity) {
        if (capacity <= 8)
            return (Object/*{T}*/) _factory8.object();
        if (capacity <= 16)
            return (Object/*{T}*/) _factory16.object();
        if (capacity <= 32)
            return (Object/*{T}*/) _factory32.object();
        if (capacity <= 64)
            return (Object/*{T}*/) _factory64.object();
        if (capacity <= 128)
            return (Object/*{T}*/) _factory128.object();
        if (capacity <= 256)
            return (Object/*{T}*/) _factory256.object();
        if (capacity <= 512)
            return (Object/*{T}*/) _factory512.object();
        if (capacity <= 1024)
            return (Object/*{T}*/) _factory1024.object();
        if (capacity <= 2048)
            return (Object/*{T}*/) _factory2048.object();
        if (capacity <= 4096)
            return (Object/*{T}*/) _factory4096.object();
        if (capacity <= 8192)
            return (Object/*{T}*/) _factory8192.object();
        if (capacity <= 16384)
            return (Object/*{T}*/) _factory16384.object();
        if (capacity <= 32768)
            return (Object/*{T}*/) _factory32768.object();
        if (capacity <= 65536)
            return (Object/*{T}*/) _factory65536.object();
        return create(capacity);
    }

    /**
     * Recycles the specified arrays.
     * 
     * @param array the array to be recycled.
     */
    public void recycle(Object/*{T}*/array) {
        recycle(array, ((Object[]) array).length);
    }

    final void recycle(Object array, int length) {
        if (length <= 4) {
            _factory4.recycle(array);
        } else if (length <= 8) {
            _factory8.recycle(array);
        } else if (length <= 16) {
            _factory16.recycle(array);
        } else if (length <= 32) {
            _factory32.recycle(array);
        } else if (length <= 64) {
            _factory64.recycle(array);
        } else if (length <= 128) {
            _factory128.recycle(array);
        } else if (length <= 256) {
            _factory256.recycle(array);
        } else if (length <= 512) {
            _factory512.recycle(array);
        } else if (length <= 1024) {
            _factory1024.recycle(array);
        } else if (length <= 2048) {
            _factory2048.recycle(array);
        } else if (length <= 4096) {
            _factory4096.recycle(array);
        } else if (length <= 8192) {
            _factory8192.recycle(array);
        } else if (length <= 16384) {
            _factory16384.recycle(array);
        } else if (length <= 32768) {
            _factory32768.recycle(array);
        } else if (length <= 65536) {
            _factory65536.recycle(array);
        }
    }

    /**
     * Constructs a new array of specified size from this factory 
     * (using the <code>new</code> keyword).
     *
     * @param size the size of the array.
     * @return a new factory array.
     */
    protected abstract Object/*{T}*/create(int size);

}