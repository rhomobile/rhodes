import java.util.*;
import java.io.*;

class JavaField { 
    static final int tpVoid           = 0;
    static final int tpByte           = 1;
    static final int tpBoolean        = 2;
    static final int tpShort          = 3;
    static final int tpChar           = 4;
    static final int tpInt            = 5;
    static final int tpLong           = 6;
    static final int tpFloat          = 7;
    static final int tpDouble         = 8;
    static final int tpDate           = 9;
    static final int tpString         = 10;
    static final int tpObject         = 11;
    static final int tpLink           = 12;
    static final int tpArrayOfByte    = 13;
    static final int tpArrayOfBoolean = 14;
    static final int tpArrayOfShort   = 15;
    static final int tpArrayOfChar    = 16;
    static final int tpArrayOfInt     = 17;
    static final int tpArrayOfLong    = 18;
    static final int tpArrayOfFloat   = 19;
    static final int tpArrayOfDouble  = 20;
    static final int tpArrayOfDate    = 21;
    static final int tpArrayOfString  = 22;
    static final int tpArrayOfObject  = 23;

    static final String[] typeMnem = {
        "Void",
        "Byte",
        "Boolean",
        "Short",
        "Char",
        "Int",
        "Long",
        "Float",
        "Double",
        "Date",
        "String",
        "Object",
        "Link",
        "ArrayOfByte",
        "ArrayOfBoolean",
        "ArrayOfShort",
        "ArrayOfChar",
        "ArrayOfInt",
        "ArrayOfLong",
        "ArrayOfFloat",
        "ArrayOfDouble",
        "ArrayOfDate",
        "ArrayOfString",
        "ArrayOfObject"
    };        

    String getType() { 
        return typeMnem[type];
    }

    String name;
    int    type;
    String cls;
    JavaField next;
}

class JavaClass { 
    JavaField first;
    JavaField last;
    boolean hasReferences;

    void addField(JavaField f) { 
        if (first == null) { 
            first = f;
        } else { 
            last.next = f;
        }
        last = f;
        hasReferences |= f.type == JavaField.tpObject;
    }        

    static void fill(Writer writer, int indent) throws IOException { 
        if (indent < 0) {
            writer.write(' ');
        } else { 
            writer.write(JavaFile.newLine.toCharArray());
            while (--indent >= 0) { 
                writer.write(' ');
            }
        }
    }        

    void isPinnable(Writer writer, int indent) throws IOException { 
        fill(writer, indent);
        if (hasReferences) { 
            writer.write("return false;".toCharArray());
        } else { 
            writer.write("return super.isPinnable();".toCharArray());
        }
    }

    void pack(Writer writer, String param, int indent) throws IOException { 
        fill(writer, indent);
        writer.write(("super.writeObject(" + param + ");").toCharArray());
        for (JavaField f = first; f != null; f = f.next) { 
            fill(writer, indent);
            writer.write((param + ".write" + f.getType() + "(" + f.name + ");").toCharArray());
        }
    }


    void unpack(Writer writer, String param, int indent) throws IOException { 
        fill(writer, indent);
        writer.write(("super.readObject(" + param + ");").toCharArray());
        for (JavaField f = first; f != null; f = f.next) { 
            fill(writer, indent);
            writer.write((f.name + " = " + (f.type == JavaField.tpObject ? "(" + f.cls  + ")" : "") + param + ".read" + f.getType() + "();").toCharArray());
        }
    }

}

class JavaMethod {
    JavaClass  cls;
    int        indentation;
    int        offset;
    int        length;
    String     param;    
    
    static final int READ_OBJECT = 1;
    static final int WRITE_OBJECT = 2;
    static final int IS_PINNABLE = 3;
    int        kind;

    JavaMethod next;
}

class JavaFile { 
    char[] content;
    int    contentLength;
    
    static String newLine = "\n";

    JavaMethod first;
    JavaMethod last;

    static final int TAB = 4;

    void write(Writer writer) throws IOException { 
        int offs = 0;
        for (JavaMethod m = first; m != null; m = m.next) { 
            writer.write(content, offs, m.offset - offs);
            switch (m.kind) { 
            case JavaMethod.READ_OBJECT:
                m.cls.unpack(writer, m.param, m.indentation + TAB);
                break;
             case JavaMethod.WRITE_OBJECT:
                m.cls.pack(writer, m.param, m.indentation + TAB);
                break;
             case JavaMethod.IS_PINNABLE:
                m.cls.isPinnable(writer, m.indentation + TAB);
                break;                
            }
            offs = m.offset + m.length;
        }
        writer.write(content, offs, contentLength - offs);
    }

    void addMethod(JavaMethod m) { 
        if (first == null) { 
            first = m;
        } else { 
            last.next = m;
        }
        last = m;
    }        
}

class ParseError extends Error { 
    ParseError(String msg) { 
        super(msg);
    }
}

class JavaParser { 
    static final int tknVoid           = 0;
    static final int tknByte           = 1;
    static final int tknBoolean        = 2;
    static final int tknShort          = 3;
    static final int tknChar           = 4;
    static final int tknInt            = 5;
    static final int tknLong           = 6;
    static final int tknFloat          = 7;
    static final int tknDouble         = 8;
    static final int tknDate           = 9;
    static final int tknString         = 10;
    static final int tknObject         = 11;
    static final int tknLink           = 12;
    static final int tknStatic         = 13;
    static final int tknFinal          = 14;
    static final int tknPublic         = 15;
    static final int tknProtected      = 16;
    static final int tknPrivate        = 17;
    static final int tknTransient      = 18;
    static final int tknNative         = 19;
    static final int tknVolatile       = 20;
    static final int tknSynchronized   = 21;
    static final int tknAbstract       = 22;
    static final int tknClass          = 23;
    static final int tknInterface      = 24;
    static final int tknReadObject     = 25;
    static final int tknWriteObject    = 26;
    static final int tknIInputStream   = 27;
    static final int tknIOutputStream  = 28;
    static final int tknComma          = 29;
    static final int tknSemicolon      = 30;
    static final int tknLbr            = 31;
    static final int tknRbr            = 32;
    static final int tknLpar           = 33;
    static final int tknRpar           = 34;
    static final int tknLsqbr          = 35;
    static final int tknRsqbr          = 36;
    static final int tknStringLiteral  = 37;
    static final int tknCharLiteral    = 38;
    static final int tknNumericLiteral = 39;
    static final int tknIdent          = 40;
    static final int tknOperator       = 41;
    static final int tknAssign         = 42;
    static final int tknIsPinnable     = 43;
    static final int tknEof            = 44;

    Hashtable symbolTable;
    
    JavaParser() { 
        symbolTable = new Hashtable();
        symbolTable.put("void", new Integer(tknVoid));
        symbolTable.put("byte", new Integer(tknByte));
        symbolTable.put("boolean", new Integer(tknBoolean));
        symbolTable.put("short", new Integer(tknShort));
        symbolTable.put("char", new Integer(tknChar));
        symbolTable.put("int", new Integer(tknInt));
        symbolTable.put("long", new Integer(tknLong));
        symbolTable.put("float", new Integer(tknFloat));
        symbolTable.put("double", new Integer(tknDouble));
        symbolTable.put("Date", new Integer(tknDate));
        symbolTable.put("String", new Integer(tknString));
        symbolTable.put("Link", new Integer(tknLink));
        symbolTable.put("java.util.Date", new Integer(tknDate));
        symbolTable.put("java.lang.String", new Integer(tknString));
        symbolTable.put("org.garret.perst.Link", new Integer(tknLink));
        symbolTable.put("static", new Integer(tknStatic));
        symbolTable.put("abstract", new Integer(tknAbstract));
        symbolTable.put("final", new Integer(tknFinal));
        symbolTable.put("public", new Integer(tknPublic));
        symbolTable.put("protected", new Integer(tknProtected));
        symbolTable.put("private", new Integer(tknPrivate));
        symbolTable.put("volatile", new Integer(tknVolatile));
        symbolTable.put("transient", new Integer(tknTransient));
        symbolTable.put("native", new Integer(tknNative));
        symbolTable.put("synchronized", new Integer(tknSynchronized));
        symbolTable.put("class", new Integer(tknClass));
        symbolTable.put("interface", new Integer(tknInterface));
        symbolTable.put("readObject", new Integer(tknReadObject));
        symbolTable.put("writeObject", new Integer(tknWriteObject));
        symbolTable.put("IInputStream", new Integer(tknIInputStream));
        symbolTable.put("IOutputStream", new Integer(tknIOutputStream));
        symbolTable.put("org.garret.perst.IInputStream", new Integer(tknIInputStream));
        symbolTable.put("org.garret.perst.IOutputStream", new Integer(tknIOutputStream));
        symbolTable.put("isPinnable", new Integer(tknIsPinnable));
    }

    char[] content;
    int    contentLength;
    int    pos;
    int    line;
    String ident;

    int scan() { 
        char ch = '\0';
        while (true) { 
            while (true) { 
                if (pos >= contentLength) { 
                    return tknEof;
                }
                ch = content[pos++];
                if (!Character.isWhitespace(ch)) { 
                    break;
                }
                if (ch == '\n') { 
                    line += 1;
                }
            }
            switch (ch) { 
            case '=':
                return tknAssign;
            case ',':
                return tknComma;
            case ';':
                return tknSemicolon;
            case '{':
                return tknLbr;
            case '}':
                return tknRbr;
            case '[':
                return tknLsqbr;
            case ']':
                return tknRsqbr;
            case '(':
                return tknLpar;
            case ')':
                return tknRpar;
            case '\'':
                while (pos < contentLength && content[pos++] != '\'') {
                    if (content[pos-1] == '\\') { 
                        pos += 1;
                    }
                }
                return tknCharLiteral;
            case '"':
                 while (pos < contentLength && content[pos++] != '\"') {
                    if (content[pos-1] == '\\') { 
                        pos += 1;
                    }
                }
                return tknStringLiteral;
            case '/':
                if (pos < contentLength) { 
                    switch (content[pos]) {
                    case '/':
                        while (++pos < contentLength && content[pos] != '\n');
                        line += 1;
                        pos += 1;
                        continue;
                    case '*':
                        while (++pos < contentLength-1 && !(content[pos] == '*' && content[pos+1] == '/')) { 
                            if (content[pos] == '\n') { 
                                line += 1;
                            }
                        }
                        pos += 2;
                        continue;
                    default:
                        break;
                    }
                }
                return tknOperator;
            default:
                if (Character.isJavaIdentifierStart(ch)) { 
                    int beg = pos-1;
                    while (pos < contentLength && (content[pos] == '.' || Character.isJavaIdentifierPart(content[pos]))) {
                        pos += 1;
                    }
                    ident = new String(content, beg, pos-beg);
                    Integer tkn = (Integer)symbolTable.get(ident);
                    if (tkn != null) { 
                        return tkn.intValue();
                    }
                    return tknIdent;
                } else if (Character.isDigit(ch)) { 
                    while (pos < contentLength && Character.isLetterOrDigit(content[pos])) { 
                        pos += 1;
                    }
                    return tknNumericLiteral;
                } else { 
                    return tknOperator;
                }
            }
        }
    }

    void patchMethod(JavaFile file, JavaClass cls, int kind, int paramType)
    {
        if (scan() != tknLpar) {
            error("'(' expected");
        }
        int tkn = scan();    
        if (tkn == paramType) { 
            if (paramType != tknRpar && scan() != tknIdent) { 
                error("Parameter expected");
            } else { 
                String param = ident;
                if (paramType != tknRpar && scan() != tknRpar) {
                    error("')' expected");
                }
                int offset = pos + skipForwardToNewLine();
                if (parseMethod()) { 
                    JavaMethod m = new JavaMethod();
                    if (offset >= pos) { // {}
                        m.offset = pos-1;
                        m.length = 0;
                        m.indentation = Integer.MIN_VALUE;
                    } else { 
                        m.offset = offset;
                        m.indentation = skipBackwardToNewLine() - 2;
                        m.length = pos - m.indentation - 2 - offset;
                        
                        if (content[offset + m.length - 1] == '\r') { 
                            m.length -= 1;
                        }
                    }
                    m.kind = kind;
                    m.param = param;
                    m.cls = cls;
                    file.addMethod(m);
                }
            }
        } else {
            parseMethod();
        }
    }

    void skipBody() 
    { 
        int balance = 1;
        do { 
            switch (scan()) { 
            case tknEof:
                error("Unexpected end of file");
            case tknLbr:
                balance += 1;
                break;
            case tknRbr:
                balance -= 1;
            }
        } while (balance != 0);
    }

    void parseClass(JavaFile file) 
    {
        int lex;
        while ((lex = scan()) != tknLbr) {
            if (lex == tknEof) { 
                error("Unexpected end of file");
            }
        }
        JavaClass cls = new JavaClass();
      Members:
        while (true) { 
            boolean isSerialaziable = true;
            while (true) { 
                switch (lex = scan()) {
                case tknEof:
                    error("Unexpected end of file");
                case tknRbr:
                    return;
                case tknClass:
                case tknInterface:
                    parseClass(file);
                    continue Members;
                case tknSemicolon:
                    continue Members;
                case tknAbstract:
                case tknNative:
                case tknPublic:
                case tknPrivate:
                case tknProtected:
                case tknSynchronized:
                case tknVolatile:
                    continue;
                case tknFinal:
                case tknStatic:
                case tknTransient:
                    isSerialaziable = false;
                    continue;
                case tknLbr: // static initializer                
                    skipBody();
                    break;                
                case tknVoid:
                case tknByte:
                case tknBoolean:
                case tknShort:
                case tknChar:
                case tknInt:
                case tknLong:
                case tknFloat:
                case tknDouble:
                case tknDate:
                case tknString:
                case tknLink:
                case tknIdent:
                { 
                    int type = lex == tknIdent ? JavaField.tpObject : lex - tknVoid;
                    String clsName = ident;
                    switch (scan()) { 
                    case tknLpar: // constructor 
                        parseMethod();
                        break;
                    case tknLsqbr:
                        type += JavaField.tpArrayOfBoolean - JavaField.tpBoolean;
                        if (scan() != tknRsqbr || scan() != tknIdent) { 
                            error("']' expected");
                        }
                        // no break
                    case tknIdent:
                    { 
                        String field = ident;
                        while (true) { 
                            switch (lex = scan()) { 
                            case tknLsqbr:
                                type += JavaField.tpArrayOfBoolean - JavaField.tpBoolean;
                                if (scan() != tknRsqbr) { 
                                    error("']' expected");
                                }
                                continue;
                            case tknAssign:
                              Initializer:
                                while (true) { 
                                    switch (lex = scan()) {
                                    case tknComma:
                                    case tknSemicolon:
                                        break Initializer;
                                    case tknLbr: // aggregate
                                        skipBody();
                                        continue;
                                    case tknEof:
                                        error("Invalid variable initialization");
                                    }
                                }
                                // no break
                            case tknComma:
                            case tknSemicolon:
                                if (isSerialaziable) { 
                                    JavaField f = new JavaField();
                                    f.name = field;
                                    f.type = type;
                                    f.cls = clsName;
                                    cls.addField(f);
                                }
                                if (lex == tknComma) { 
                                    if (scan() != tknIdent) { 
                                        error("Field name expected");
                                    }
                                    continue;
                                }
                                break;
                            case tknLpar:
                                parseMethod();
                                break;
                            }
                            break;
                        }
                        break;
                    }
                    case tknReadObject:
                        patchMethod(file, cls, JavaMethod.READ_OBJECT, tknIInputStream);
                        break;
                    case tknWriteObject:
                        patchMethod(file, cls, JavaMethod.WRITE_OBJECT, tknIOutputStream);
                        break;
                    case tknIsPinnable:
                        patchMethod(file, cls, JavaMethod.IS_PINNABLE, tknRpar);
                        break;
                    default:
                        error("Field or method name expected");
                    } 
                    break;
                }
                default:
                    error("Field or method type expected");
                }
                break;
            }
        }
    }

    int skipForwardToNewLine() 
    {
        int p;
        for (p = pos; p < contentLength && content[p] != '\n' && content[p] != '\r'; p++);
        return p - pos;
    }

    int skipBackwardToNewLine() 
    {
        int p;
        for (p = pos; p >= 0 && content[p] != '\n'; p--);
        return pos - p;
    }

    void error(String msg) { 
        throw new ParseError(msg);
    }

    boolean parseMethod() {
        int lex;
        while ((lex = scan()) != tknLbr && lex != tknSemicolon) {
            if (lex == tknEof) { 
                error("Unexpected end of file");
            }
        }
        if (lex == tknLbr) { 
            skipBody();
            return true;
        }
        return false;
    }

    void parse(JavaFile f) { 
        content = f.content;
        contentLength = f.contentLength;
        for (int i = 0; i < contentLength; i++) {
            if (content[i] == '\n') { 
                JavaFile.newLine = (i > 0 && content[i-1] == '\r') ? "\r\n" : "\n";
                break;
            }
        }
        pos = 0;
        line = 1;
        parseClass(f);
    }
}
    
public class SerGen 
{ 
    static void processFile(JavaParser parser, File file) throws IOException
    { 
        if (file.isDirectory()) {
            String[] list = file.list();
            for (int i = 0; i < list.length; i++) { 
                File f = new File(file, list[i]);
                if (f.isDirectory() || list[i].endsWith(".java")) { 
                    processFile(parser, f);
                }
            }
        } else { 
            FileReader reader = new FileReader(file);
            int fileLength = (int)file.length();
            char[] buf = new char[fileLength];
            int offs = 0, rc;
            while ((rc = reader.read(buf, offs, fileLength - offs)) > 0) { 
                offs += rc;
            }
            reader.close();
            JavaFile f = new JavaFile();
            f.content = buf;
            f.contentLength = offs;
            try { 
                parser.parse(f);
            } catch (ParseError e) { 
                System.err.println("Error in file " + file.getPath() + " at line " + parser.line + ": " + e.getMessage());
            }
            if (f.first != null) { 
                File tmp = new File(file.getPath() + ".tmp");
                FileWriter writer = new FileWriter(tmp);
                f.write(writer);
                writer.close();
                file.delete();
                if (!tmp.renameTo(file)) { 
                    System.err.println("Failed to rename file to " + file.getPath());
                } else { 
                    System.out.println("Patch file " + file.getPath());
                }
            }
        }
    }

    public static void main(String args[]) throws IOException { 
        JavaParser parser = new JavaParser();
        for (int i = 0; i < args.length; i++) { 
            processFile(parser, new File(args[i]));
        }
    }
}