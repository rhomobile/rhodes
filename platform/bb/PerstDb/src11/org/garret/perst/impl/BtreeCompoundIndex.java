package org.garret.perst.impl;
import  org.garret.perst.*;

import java.util.*;

class BtreeCompoundIndex extends Btree implements Index { 
    int[]    types;

    public void writeObject(IOutputStream out) {
        super.writeObject(out);
        out.writeArrayOfInt(types);
    }

    public void readObject(IInputStream in) {
        super.readObject(in);
        types = in.readArrayOfInt();
    }

    BtreeCompoundIndex() {}
    
    BtreeCompoundIndex(String[] keyTypes, boolean unique) {
        this.unique = unique;
        type = Types.ArrayOfByte;        
        types = new int[keyTypes.length];
        for (int i = 0; i < keyTypes.length; i++) {
            types[i] = getCompoundKeyComponentType(keyTypes[i]);
        }
    }

    BtreeCompoundIndex(int[] types, boolean unique) {
        type = Types.ArrayOfByte;        
        this.types = types;
        this.unique = unique;
    }

    static int getCompoundKeyComponentType(String c) { 
        if (c.equals("boolean")) { 
            return Types.Boolean;
        } else if (c.equals("byte")) { 
            return Types.Byte;
        } else if (c.equals("char")) { 
            return Types.Char;
        } else if (c.equals("short")) { 
            return Types.Short;
        } else if (c.equals("int")) { 
            return Types.Int;
        } else if (c.equals("long")) { 
            return Types.Long;
        } else if (c.equals("float")) { 
            return Types.Float;
        } else if (c.equals("double")) { 
            return Types.Double;
        } else if (c.equals("java.lang.String")) { 
            return Types.String;
        } else if (c.equals("java.uril.Date")) { 
            return Types.Date;
        } else if (c.equals("byte[]")) { 
            return Types.ArrayOfByte;
        } else {
            return Types.Object;
        }
    }

    int compareByteArrays(byte[] key, byte[] item, int offs, int lengtn) { 
        int o1 = 0;
        int o2 = offs;
        byte[] a1 = key;
        byte[] a2 = item;
        for (int i = 0; i < types.length && o1 < key.length; i++) {
            int diff = 0;
            switch (types[i]) { 
              case Types.Boolean:
              case Types.Byte:
                diff = a1[o1++] - a2[o2++];
                break;
              case Types.Short:
                diff = Bytes.unpack2(a1, o1) - Bytes.unpack2(a2, o2);
                o1 += 2;
                o2 += 2;
                break;
              case Types.Char:
                diff = (char)Bytes.unpack2(a1, o1) - (char)Bytes.unpack2(a2, o2);
                o1 += 2;
                o2 += 2;
                break;
              case Types.Int:
              case Types.Object:
              {
                  int i1 = Bytes.unpack4(a1, o1);
                  int i2 = Bytes.unpack4(a2, o2);
                  diff = i1 < i2 ? -1 : i1 == i2 ? 0 : 1;
                  o1 += 4;
                  o2 += 4;
                  break;
              }
              case Types.Long:
              case Types.Date:
              {
                  long l1 = Bytes.unpack8(a1, o1);
                  long l2 = Bytes.unpack8(a2, o2);
                  diff = l1 < l2 ? -1 : l1 == l2 ? 0 : 1;
                  o1 += 8;
                  o2 += 8;
                  break;
              }
              case Types.Float:
              {
                  float f1 = Float.intBitsToFloat(Bytes.unpack4(a1, o1));
                  float f2 = Float.intBitsToFloat(Bytes.unpack4(a2, o2));
                  diff = f1 < f2 ? -1 : f1 == f2 ? 0 : 1;
                  o1 += 4;
                  o2 += 4;
                  break;
              }
              case Types.Double:
              {
                  double d1 = Double.longBitsToDouble(Bytes.unpack8(a1, o1));
                  double d2 = Double.longBitsToDouble(Bytes.unpack8(a2, o2));
                  diff = d1 < d2 ? -1 : d1 == d2 ? 0 : 1;
                  o1 += 8;
                  o2 += 8;
                  break;
              }
              case Types.String:
              {
                  int len1 = Bytes.unpack2(a1, o1);
                  int len2 = Bytes.unpack2(a2, o2);
                  o1 += 2;
                  o2 += 2;
                  int len = len1 < len2 ? len1 : len2;
                  while (--len >= 0) { 
                      diff = (char)Bytes.unpack2(a1, o1) - (char)Bytes.unpack2(a2, o2);
                      if (diff != 0) { 
                          return diff;
                      }
                      o1 += 2;
                      o2 += 2;
                  }
                  diff = len1 - len2;
                  break;
              }
              case Types.ArrayOfByte:
              {
                  int len1 = Bytes.unpack4(a1, o1);
                  int len2 = Bytes.unpack4(a2, o2);
                  o1 += 4;
                  o2 += 4;
                  int len = len1 < len2 ? len1 : len2;
                  while (--len >= 0) { 
                      diff = a1[o1++] - a2[o2++];
                      if (diff != 0) { 
                          return diff;
                      }
                  }
                  diff = len1 - len2;
                  break;
              }
              default:
                Assert.failed("Invalid type");
            }
            if (diff != 0) { 
                return diff;
            }
        }
        return 0;
    }

    Object unpackByteArrayKey(Page pg, int pos) {
        int offs = BtreePage.firstKeyOffs + BtreePage.getKeyStrOffs(pg, pos);
        byte[] data = pg.data;
        Object values[] = new Object[types.length];

        for (int i = 0; i < types.length; i++) {
            Object v = null;
            switch (types[i]) { 
              case Types.Boolean:
                v = (data[offs++] != 0) ? Boolean.TRUE : Boolean.FALSE;
                break;
              case Types.Byte:
                v = new Byte(data[offs++]);
                break;
              case Types.Short:
                v = new Short(Bytes.unpack2(data, offs));
                offs += 2;
                break;
              case Types.Char:
                v = new Character((char)Bytes.unpack2(data, offs));
                offs += 2;
                break;
              case Types.Int:
                v = new Integer(Bytes.unpack4(data, offs));
                offs += 4;
                break;
              case Types.Object:
              {
                  int oid = Bytes.unpack4(data, offs);
                  v = oid == 0 ? null : ((StorageImpl)getStorage()).lookupObject(oid);
                  offs += 4;
                  break;
              }
              case Types.Long:
                v = new Long(Bytes.unpack8(data, offs));
                offs += 8;
                break;
              case Types.Date:
              {
                  long msec = Bytes.unpack8(data, offs);
                  v = msec == -1 ? null : new Date(msec);
                  offs += 8;
                  break;
              }
              case Types.Float:
                v = new Float(Float.intBitsToFloat(Bytes.unpack4(data, offs)));
                offs += 4;
                break;
              case Types.Double:
                v = new Double(Double.longBitsToDouble(Bytes.unpack8(data, offs)));
                offs += 8;
                break;
              case Types.String:
              {
                  int len = Bytes.unpack2(data, offs);
                  offs += 2;
                  char[] sval = new char[len];
                  for (int j = 0; j < len; j++) { 
                      sval[j] = (char)Bytes.unpack2(data, offs);
                      offs += 2;
                  }
                  v = new String(sval);
                  break;
              }
              case Types.ArrayOfByte:
              {
                  int len = Bytes.unpack4(data, offs);
                  offs += 4;
                  byte[] bval = new byte[len];
                  System.arraycopy(data, offs, bval, 0, len);
                  offs += len;
                  break;
              }
              default:
                Assert.failed("Invalid type");
            }
            values[i] = v;
        }
        return values;
    }
            

    private Key convertKey(Key key) { 
        if (key == null) { 
            return null;
        }
        if (key.type != Types.ArrayOfObject) { 
            throw new StorageError(StorageError.INCOMPATIBLE_KEY_TYPE);
        }
        Object[] values = (Object[])key.oval;
        ByteBuffer buf = new ByteBuffer();
        int dst = 0;
        for (int i = 0; i < values.length; i++) { 
            Object v = values[i];
            switch (types[i]) {
              case Types.Boolean:
                buf.extend(dst+1);
                buf.arr[dst++] = (byte)(((Boolean)v).booleanValue() ? 1 : 0);
                break;
              case Types.Byte:
                buf.extend(dst+1);
                buf.arr[dst++] = ((Byte)v).byteValue();
                break;
              case Types.Short:
                buf.extend(dst+2);
                Bytes.pack2(buf.arr, dst, ((Short)v).shortValue());
                dst += 2;
                break;
              case Types.Char:
                buf.extend(dst+2);
                Bytes.pack2(buf.arr, dst, (short)((Character)v).charValue());
                dst += 2;
                break;
              case Types.Int:
                buf.extend(dst+4);
                Bytes.pack4(buf.arr, dst, ((Integer)v).intValue());
                dst += 4;
                break;
              case Types.Object:
                buf.extend(dst+4);
                Bytes.pack4(buf.arr, dst, v == null ? 0 : ((IPersistent)v).getOid());
                dst += 4;
                break;
              case Types.Long:
                buf.extend(dst+8);
                Bytes.pack8(buf.arr, dst, ((Long)v).longValue());
                dst += 8;
                break;
              case Types.Date:
                buf.extend(dst+8);
                Bytes.pack8(buf.arr, dst, v == null ? -1 : ((Date)v).getTime());
                dst += 8;
                break;
              case Types.Float:
                buf.extend(dst+4);
                Bytes.pack4(buf.arr, dst, Float.floatToIntBits(((Float)v).floatValue()));
                dst += 4;
                break;
              case Types.Double:
                buf.extend(dst+8);
                Bytes.pack8(buf.arr, dst, Double.doubleToLongBits(((Double)v).doubleValue()));
                dst += 8;
                break;
              case Types.String:
              {
                  buf.extend(dst+2);
                  if (v != null) { 
                      String str = (String)v;
                      int len = str.length();
                      Bytes.pack2(buf.arr, dst, (short)len);
                      dst += 2;
                      buf.extend(dst + len*2);
                      for (int j = 0; j < len; j++) { 
                          Bytes.pack2(buf.arr, dst, (short)str.charAt(j));
                          dst += 2;
                      }
                  } else { 
                      Bytes.pack2(buf.arr, dst, (short)0);
                      dst += 2;
                  }
                  break;
              }
              case Types.ArrayOfByte:
              {
                  buf.extend(dst+4);
                  if (v != null) { 
                      byte[] arr = (byte[])v;
                      int len = arr.length;
                      Bytes.pack4(buf.arr, dst, len);
                      dst += 4;                          
                      buf.extend(dst + len);
                      System.arraycopy(arr, 0, buf.arr, dst, len);
                      dst += len;
                  } else { 
                      Bytes.pack4(buf.arr, dst, 0);
                      dst += 4;
                  }
                  break;
              }
              default:
                Assert.failed("Invalid type");
            }
        }
        return new Key(buf.toArray(), key.inclusion != 0);
    }
            
    public IPersistent[] get(Key from, Key till) {
        return super.get(convertKey(from), convertKey(till));
    }

    public IPersistent get(Key key) {
        return super.get(convertKey(key));
    }

    public IPersistent  remove(Key key) { 
        return super.remove(convertKey(key));
    }

    public void remove(Key key, IPersistent obj) { 
        super.remove(convertKey(key), obj);
    }

    public IPersistent set(Key key, IPersistent obj) { 
        return super.set(convertKey(key), obj);
    }

    public boolean put(Key key, IPersistent obj) {
        return super.put(convertKey(key), obj);
    }

    public Iterator iterator(Key from, Key till, int order) {
        return super.iterator(convertKey(from), convertKey(till), order);
    }

    public Iterator entryIterator(Key from, Key till, int order) {
        return super.entryIterator(convertKey(from), convertKey(till), order);
    }
}

