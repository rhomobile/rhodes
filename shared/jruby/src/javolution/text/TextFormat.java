/*
 * Javolution - Java(TM) Solution for Real-Time and Embedded Systems
 * Copyright (C) 2006 - Javolution (http://javolution.org/)
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software is
 * freely granted, provided that this notice is preserved.
 */
package javolution.text;

import j2me.lang.CharSequence;
import j2me.text.ParsePosition;
import javolution.Javolution;
import javolution.context.ObjectFactory;
import javolution.lang.ClassInitializer;
import javolution.lang.Reflection;
import javolution.util.FastMap;

import java.io.IOException;

/**
 * <p> This class represents the base format for text parsing and formatting; 
 *     it supports {@link CharSequence} and {@link javolution.text.Appendable} 
 *     interfaces for greater flexibility.</p>
 * 
 * <p> It is possible to retrieve the format for any class for which the 
 *     format has been registered (typically during class initialization).
 *     For example:[code]
 *     public class Complex extends RealtimeObject {
 *         private static final TextFormat<Complex> CARTESIAN = ...;
 *         static { // Sets default format to cartesian, users may change it later (e.g. polar).
 *             TextFormat.setInstance(Complex.class, CARTESIAN);
 *         }
 *         public Complex valueOf(CharSequence csq) {
 *             return TextFormat.getInstance(Complex.class).parse(csq);
 *         }
 *         public Text toText() {
 *             return TextFormat.getInstance(Complex.class).format(this);
 *         }
 *     }[/code]</p>
 *     
 * <p> For parsing/formatting of primitive types, the {@link TypeFormat}
 *     utility class is recommended.</p>
 *     
 * <p> <i>Note:</i> The format behavior may depend upon  
 *     {@link javolution.context.LocalContext local} settings in which 
 *     case concurrent threads may parse differently!</p>
 *     
 * @author <a href="mailto:jean-marie@dautelle.com">Jean-Marie Dautelle </a>
 * @version 5.1, July 4, 2007
 */
public abstract class TextFormat/*<T>*/{

    /**
     * Holds the class to format mapping.
     */
    private static final FastMap FORMATS = new FastMap().setShared(true);

    /**
     * Default constructor.
     */
    protected TextFormat() {
    }

    /**
     * Returns the text format for instances of specified type (class or 
     * interface). The following types are always recognized:<code><ul>
     *    <li>java.lang.Boolean</li>
     *    <li>java.lang.Character</li>
     *    <li>java.lang.Byte</li>
     *    <li>java.lang.Short</li>
     *    <li>java.lang.Integer</li>
     *    <li>java.lang.Long</li>
     *    <li>java.lang.Float</li>
     *    <li>java.lang.Double</li>
     *    <li>java.lang.Class</li>
     * </ul></code>
     * Users may register additional types using the {@link #setInstance 
     * TextFormat.setInstance(Class, TextFormat)} static method. 
     * For example:[code]
     * TextFormat<Font> fontFormat = new TextFormat() {
     *     public Appendable format(Font font, Appendable dest) throws IOException {
     *         return dest.append(font.getName());
     *     }
     *     public Font parse(CharSequence csq, Cursor cursor) {
     *         CharSequence fontName = csq.subSequence(cursor.getIndex(), cursor.getEndIndex());
     *         cursor.increment(fontName.length());
     *         return Font.decode(fontName.toString());
     *     }
     * });
     * TextFormat.setInstance(Font.class, fontFormat); // Registers format for java.awt.Font
     * [/code]
     * 
     * @param  cls the class for which the default format is returned.
     * @return the format for instances of the specified class or 
     *         <code>null</code> if unkown.
     */
    public static/*<T>*/TextFormat/*<T>*/getInstance(Class/*<T>*/cls) {
        TextFormat format = (TextFormat) FORMATS.get(cls);
        return (format != null) ? format : searchFormat(cls);
    }

    private static TextFormat searchFormat(Class cls) {
        if (cls == null)
            return null;
        ClassInitializer.initialize(cls); // Ensures class static initializer are run.
        TextFormat format = (TextFormat) FORMATS.get(cls);
        return (format != null) ? format : searchFormat(superclassOf(cls));
    }

    private static Class superclassOf(Class cls) {
        /*@JVM-1.4+@
         if (true) return cls.getSuperclass();
         /**/
        return null;
    }

    /**
     * Associates the specified format to the specified type (class or 
     * interface).
     * 
     * @param  cls the class for which the default format is returned.
     * @param format the format for instances of the specified calss class.
     */
    public static/*<T>*/void setInstance(Class/*<T>*/cls,
            TextFormat/*<T>*/format) {

        // The specified class is initialized prior to setting 
        // the format to ensure that the default format (typically in the 
        // class static initializer) does not override the new format.
        ClassInitializer.initialize(cls);

        FORMATS.put(cls, format); // Thread-safe (shared map).
    }

    /**
     * Formats the specified object into an <code>Appendable</code> 
     * 
     * @param obj the object to format.
     * @param dest the appendable destination.
     * @return the specified <code>Appendable</code>.
     * @throws IOException if an I/O exception occurs.
     */
    public abstract Appendable format(Object/*{T}*/obj, Appendable dest)
            throws IOException;

    /**
     * Parses a portion of the specified <code>CharSequence</code> from the
     * specified position to produce an object. If parsing succeeds, then the
     * index of the <code>cursor</code> argument is updated to the index after
     * the last character used. 
     * 
     * @param csq the <code>CharSequence</code> to parse.
     * @param cursor the cursor holding the current parsing index.
     * @return the object parsed from the specified character sub-sequence.
     * @throws RuntimeException if any problem occurs while parsing the 
     *         specified character sequence (e.g. illegal syntax).
     */
    public abstract Object/*{T}*/parse(CharSequence csq, Cursor cursor);

    /**
     * Formats the specified object into a {@link TextBuilder} (convenience 
     * method which does not raise IOException). 
     * 
     * @param obj the object to format.
     * @param dest the text builder destination.
     * @return the specified text builder.
     */
    public final Appendable format(Object/*{T}*/obj, TextBuilder dest) {
        try {
            return format(obj, (Appendable) dest);
        } catch (IOException e) {
            throw new Error(); // Cannot happen.
        }
    }

    /**
     * Formats the specified object to a {@link Text} instance
     * (convenience method).
     * 
     * @param obj the object being formated.
     * @return the text representing the specified object.
     */
    public final Text format(Object/*{T}*/obj) {
        TextBuilder tb = TextBuilder.newInstance();
        format(obj, tb);
        Text txt = tb.toText();
        TextBuilder.recycle(tb);
        return txt;
    }

    /**
     * Parses a whole character sequence from the beginning to produce an object
     * (convenience method). 
     * 
     * @param csq the whole character sequence to parse.
     * @return the corresponding object.
     * @throws IllegalArgumentException if the specified character sequence 
     *        cannot be fully parsed.
     */
    public final Object/*{T}*/parse(CharSequence csq) {
        Cursor cursor = Cursor.newInstance(0, csq.length());
        Object/*{T}*/obj = parse(csq, cursor);
        if (cursor.hasNext())
            throw new IllegalArgumentException("Incomplete Parsing");
        Cursor.recycle(cursor);
        return obj;
    }

    /**
     * This class represents a parsing cursor over a character sequence
     * (or subsequence). A cursor location may start and end at any predefined
     * location within the character sequence iterated over (equivalent to
     * parsing a subsequence of the character sequence input). 
     */
    public static class Cursor extends ParsePosition {

        /**
         * Holds the cursor factory.
         */
        private static final ObjectFactory FACTORY = new ObjectFactory() {
            public Object create() {
                return new Cursor();
            }
        };

        /**
         * Holds the cursor index.
         */
        private int _index;

        /**
         * Holds the start index.
         */
        private int _start;

        /**
         * Holds the end index.
         */
        private int _end;

        /**
         * Default constructor.
         */
        private Cursor() {
            super(0);
        }

        /**
         * Returns a new, preallocated or {@link #recycle recycled} cursor
         * instance (on the stack when executing in a {@link 
         * javolution.context.StackContext StackContext}).
         * 
         * @param start the start index.
         * @param end the end index (index after the last character to be read).
         * @return a new or recycled cursor instance.
         */
        public static Cursor newInstance(int start, int end) {
            Cursor cursor = (Cursor) FACTORY.object();
            cursor._start = cursor._index = start;
            cursor._end = end;
            cursor.setErrorIndex(-1);
            return cursor;
        }

        /**
         * Recycles a cursor {@link #newInstance instance} immediately
         * (on the stack when executing in a {@link 
         * javolution.context.StackContext StackContext}).
         * 
         * @param instance the cursor instance being recycled.
         */
        public static void recycle(Cursor instance) {
            FACTORY.recycle(instance);
        }

        /**
         * Returns this cursor index.
         * 
         * @return the index of the next character to parse.
         */
        public final int getIndex() {
            return _index;
        }

        /**
         * Returns this cursor start index.
         * 
         * @return the start index.
         */
        public final int getStartIndex() {
            return _start;
        }

        /**
         * Returns this cursor end index.
         * 
         * @return the end index.
         */
        public final int getEndIndex() {
            return _end;
        }

        /**
         * Returns the error index of this cursor if 
         * {@link #setErrorIndex set}; otherwise returns the current 
         * {@link #getIndex index}.
         * 
         * @return the error index.
         */
        public final int getErrorIndex() {
            int errorIndex = this.getErrorIndex();
            return errorIndex >= 0 ? errorIndex : _index;
        }

        /**
         * Sets the cursor current index.
         * 
         * @param i the index of the next character to parse.
         * @throws IllegalArgumentException 
         *        if <code>((i < getStartIndex()) || (i > getEndIndex()))</code>
         */
        public final void setIndex(int i) {
            if ((i < _start) || (i > _end))
                throw new IllegalArgumentException();
            _index = i;
        }

        /**
         * Sets this cursor start index.
         * 
         * @param start the start index.
         */
        public final void setStartIndex(int start) {
            _start = start;
        }

        /**
         * Sets this cursor end index.
         * 
         * @param end the end index.
         */
        public final void setEndIndex(int end) {
            _end = end;
        }

        /**
         * Sets this cursor error index.
         * 
         * @param errorIndex the error index.
         */
        public final void setErrorIndex(int errorIndex) {
            super.setErrorIndex(errorIndex);
        }

        /**
         * Indicates if this cursor has not reached the end index.
         * 
         * @return <code>this.getIndex() &lt; this.getEndIndex()</code>
         */
        public final boolean hasNext() {
            return _index < _end;
        }

        /**
         * Returns the next character at the cursor position in the specified 
         * character sequence and increments the cursor position by one.
         * For example:[code]
         *    for (char c=cursor.next(csq); c != 0; c = cursor.next(csq)) {
         *        ...
         *    }
         *    }[/code]
         * 
         * @param csq the character sequence iterated by this cursor.
         * @return the character at the current cursor position in the
         *         specified character sequence or <code>'&#92;u0000'</code>
         *         if the end index has already been reached.
         */
        public final char next(CharSequence csq) {
            return (_index < _end) ? csq.charAt(_index++) : 0;
        }

        /**
         * Indicates if this cursor points to the specified character 
         * in the specified character sequence.
         *  
         * @param c the character.
         * @param csq the character sequence iterated by this cursor.
         * @return <code>true</code> if the cursor next character is the 
         *         one specified; <code>false</code> otherwise.
         */
        public final boolean at(char c, CharSequence csq) {
            return (_index < _end) && (csq.charAt(_index) == c);
        }

        /**
         * Indicates if this cursor points to one of the specified character.
         *  
         * @param charSet the character set
         * @param csq the character sequence iterated by this cursor.
         * @return <code>true</code> if the cursor next character is one 
         *         of the character contained by the character set; 
         *         <code>false</code> otherwise.
         */
        public final boolean at(CharSet charSet, CharSequence csq) {
            return (_index < _end) && (charSet.contains(csq.charAt(_index)));
        }

        /**
         * Indicates if this cursor points to the specified characters 
         * in the specified character sequence.
         *  
         * @param pattern the characters searched for.
         * @param csq the character sequence iterated by this cursor.
         * @return <code>true</code> if the cursor next character are the 
         *         one specified in the pattern; <code>false</code> otherwise.
         */
        public final boolean at(String pattern, CharSequence csq) {
            return (_index < _end) && (csq.charAt(_index) == pattern.charAt(0)) ? match(
                    pattern, csq)
                    : false;
        }

        private final boolean match(String pattern, CharSequence csq) {
            for (int i = 1, j = _index + 1, n = pattern.length(), m = _end; i < n;) {
                if ((j >= m) || (csq.charAt(j++) != pattern.charAt(i++)))
                    return false;
            }
            return true;
        }

        /**
         * Moves this cursor forward until it points to a character 
         * different from the character specified. 
         * 
         * @param c the character to skip.
         * @param csq the character sequence iterated by this cursor.
         * @return <code>true</code> if this cursor points to a character 
         *         different from the ones specified; <code>false</code> 
         *         otherwise (e.g. end of sequence reached). 
         */
        public final boolean skip(char c, CharSequence csq) {
            while ((_index < _end) && (csq.charAt(_index) == c)) {
                _index++;
            }
            return _index < _end;
        }

        /**
         * Moves this cursor forward until it points to a character 
         * different from any of the character in the specified set. 
         * For example: [code]
         *  // Reads numbers separated by tabulations or spaces.
         *  FastTable<Integer> numbers = new FastTable<Integer>();
         *  while (cursor.skip(CharSet.SPACE_OR_TAB, csq)) {
         *      numbers.add(TypeFormat.parseInt(csq, cursor));
         *  }[/code]   
         * 
         * @param charSet the character to skip.
         * @param csq the character sequence iterated by this cursor.
         * @return <code>true</code> if this cursor points to a character 
         *         different from the ones specified; <code>false</code> 
         *         otherwise (e.g. end of sequence reached). 
         */
        public final boolean skip(CharSet charSet, CharSequence csq) {
            while ((_index < _end) && (charSet.contains(csq.charAt(_index)))) {
                _index++;
            }
            return _index < _end;
        }

        /**
         * Increments the cursor index by one.
         *
         * @return <code>this</code>
         */
        public final Cursor increment() {
            _index++;
            return this;
        }

        /**
         * Increments the cursor index by the specified value.
         * 
         * @param i the increment value.
         * @return <code>this</code>
         */
        public final Cursor increment(int i) {
            _index += i;
            return this;
        }

        /**
         * Returns the string representation of this cursor.
         * 
         * @return the index value as a string.
         */
        public String toString() {
            return String.valueOf(_index);
        }

        /**
         * Indicates if this cursor is equals to the specified object.
         * 
         * @return <code>true</code> if the specified object is a cursor 
         *         at the same index; <code>false</code> otherwise.
         */
        public boolean equals(Object obj) {
            if (obj == null)
                return false;
            if (!(obj instanceof Cursor))
                return false;
            return _index == ((Cursor) obj)._index;
        }

        /**
         * Returns the hash code for this cursor.
         * 
         * @return the hash code value for this object
         */
        public int hashCode() {
            return _index;
        }

    }

    // Predefined formats.
    static {
        FORMATS.put(new Boolean(true).getClass(), new TextFormat() {

            public Appendable format(Object obj, Appendable dest)
                    throws IOException {
                return TypeFormat.format(((Boolean) obj).booleanValue(), dest);
            }

            public Object parse(CharSequence csq, Cursor cursor) {
                return new Boolean(TypeFormat.parseBoolean(csq, cursor));
            }

        });
        FORMATS.put(new Character(' ').getClass(), new TextFormat() {

            public Appendable format(Object obj, Appendable dest)
                    throws IOException {
                return dest.append(((Character) obj).charValue());
            }

            public Object parse(CharSequence csq, Cursor cursor) {
                return new Character(cursor.next(csq));
            }

        });
        FORMATS.put(new Byte((byte)0).getClass(), new TextFormat() {

            public Appendable format(Object obj, Appendable dest)
                    throws IOException {
                return TypeFormat.format(((Byte) obj).byteValue(), dest);
            }

            public Object parse(CharSequence csq, Cursor cursor) {
                return new Byte(TypeFormat.parseByte(csq, 10, cursor));
            }

        });
        FORMATS.put(new Short((short)0).getClass(), new TextFormat() {

            public Appendable format(Object obj, Appendable dest)
                    throws IOException {
                return TypeFormat.format(((Short) obj).shortValue(), dest);
            }

            public Object parse(CharSequence csq, Cursor cursor) {
                return new Short(TypeFormat.parseShort(csq, 10, cursor));
            }

        });
        FORMATS.put(new Integer(0).getClass(), new TextFormat() {

            public Appendable format(Object obj, Appendable dest)
                    throws IOException {
                return TypeFormat.format(((Integer) obj).intValue(), dest);
            }

            public Object parse(CharSequence csq, Cursor cursor) {
                return new Integer(TypeFormat.parseInt(csq, 10, cursor));
            }

        });
        FORMATS.put(new Long(0).getClass(), new TextFormat() {

            public Appendable format(Object obj, Appendable dest)
                    throws IOException {
                return TypeFormat.format(((Long) obj).longValue(), dest);
            }

            public Object parse(CharSequence csq, Cursor cursor) {
                return new Long(TypeFormat.parseLong(csq, 10, cursor));
            }

        });
        FORMATS.put("".getClass().getClass(), new TextFormat() {

            public Appendable format(Object obj, Appendable dest)
                    throws IOException {
                return dest.append(Javolution.j2meToCharSeq(((Class) obj)
                        .getName()));
            }

            public Object parse(CharSequence csq, Cursor cursor) {
                Text txt = Text.valueOf(csq.subSequence(cursor.getIndex(),
                        cursor.getEndIndex()));
                int index = txt.indexOfAny(CharSet.WHITESPACES);
                int length = index < 0 ? txt.length() : index;
                Text className = txt.subtext(0, length);
                Class cls;
                try {
                    cls = Reflection.getClass(className);
                } catch (ClassNotFoundException e) {
                    throw new IllegalArgumentException("Class " + className
                            + " Not Found");
                }
                cursor.increment(length);
                return cls;
            }

        });

        /*@JVM-1.1+@
         FORMATS.put(new Float(0).getClass(), new TextFormat() {

         public Appendable format(Object obj, Appendable dest)
         throws IOException {
         return TypeFormat.format(((Float) obj).floatValue(), dest);
         }

         public Object parse(CharSequence csq, Cursor cursor) {
         return new Float(TypeFormat.parseFloat(csq, cursor));
         }

         });
         FORMATS.put(new Double(0).getClass(), new TextFormat() {

         public Appendable format(Object obj, Appendable dest)
         throws IOException {
         return TypeFormat.format(((Double) obj).doubleValue(), dest);
         }

         public Object parse(CharSequence csq, Cursor cursor) {
         return new Double(TypeFormat.parseDouble(csq, cursor));
         }

         });
         /**/

    }
}