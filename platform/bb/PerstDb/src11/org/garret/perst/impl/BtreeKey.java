package org.garret.perst.impl;
import  org.garret.perst.*;

class BtreeKey { 
    Key key;
    int oid;
    int oldOid;

    BtreeKey(Key key, int oid) { 
        this.key = key;
        this.oid = oid;
    }

    final void getStr(Page pg, int i) { 
        int len = BtreePage.getKeyStrSize(pg, i);
        int offs = BtreePage.firstKeyOffs + BtreePage.getKeyStrOffs(pg, i);
        char[] sval = new char[len];
        for (int j = 0; j < len; j++) { 
            sval[j] = (char)Bytes.unpack2(pg.data, offs);
            offs += 2;
        }
        key = new Key(sval);
    }

    final void getByteArray(Page pg, int i) { 
        int len = BtreePage.getKeyStrSize(pg, i);
        int offs = BtreePage.firstKeyOffs + BtreePage.getKeyStrOffs(pg, i);
        byte[] bval = new byte[len];
        System.arraycopy(pg.data, offs, bval, 0, len);
        key = new Key(bval);
    }


    final void extract(Page pg, int offs, int type) { 
        byte[] data = pg.data;

        switch (type) {
          case Types.Boolean:
            key = new Key(data[offs] != 0);
            break;
          case Types.Byte:
            key = new Key(data[offs]);
            break;
          case Types.Short:
            key = new Key(Bytes.unpack2(data, offs));
            break;
          case Types.Char:
            key = new Key((char)Bytes.unpack2(data, offs));
            break;
          case Types.Int:
          case Types.Object:
            key = new Key(Bytes.unpack4(data, offs));
            break;
          case Types.Long:
          case Types.Date:
            key = new Key(Bytes.unpack8(data, offs));
            break;
          case Types.Float:
            key = new Key(Float.intBitsToFloat(Bytes.unpack4(data, offs)));
            break;
          case Types.Double:
            key = new Key(Double.longBitsToDouble(Bytes.unpack8(data, offs)));
            break;
          default:
            Assert.failed("Invalid type");
        }
    } 
    
    final void pack(Page pg, int i) { 
        byte[] dst = pg.data;
        switch (key.type) { 
          case Types.Boolean:
          case Types.Byte:
            dst[BtreePage.firstKeyOffs + i] = (byte)key.ival;
            break;
          case Types.Short:
          case Types.Char:
            Bytes.pack2(dst, BtreePage.firstKeyOffs + i*2, (short)key.ival);
            break;
          case Types.Int:
          case Types.Object:
            Bytes.pack4(dst, BtreePage.firstKeyOffs + i*4, key.ival);
            break;
          case Types.Long:
          case Types.Date:
            Bytes.pack8(dst, BtreePage.firstKeyOffs + i*8,  key.lval);
            break;
          case Types.Float:
            Bytes.pack4(dst, BtreePage.firstKeyOffs + i*4, Float.floatToIntBits((float)key.dval));
            break;
          case Types.Double:
            Bytes.pack8(dst, BtreePage.firstKeyOffs + i*8, Double.doubleToLongBits(key.dval));
            break;
          default:
            Assert.failed("Invalid type");
        }
        Bytes.pack4(dst, BtreePage.firstKeyOffs + (BtreePage.maxItems - i - 1)*4, oid);
    }
}
