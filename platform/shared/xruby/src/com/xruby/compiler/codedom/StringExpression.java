/**
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.codedom;

public class StringExpression extends Expression {
    private final StringBuilder value_ = new StringBuilder();

    public StringExpression(String value, boolean double_quote) {
        appendString(value, double_quote);
    }

    public String getValue() {
        return value_.toString();
    }

    static String convertDoubleQuoteString(String value) {
        StringBuilder result = new StringBuilder();

        for (int i = 0; i < value.length(); ++i) {
            if (value.charAt(i) != '\\') {
                result.append(value.charAt(i));
                continue;
            }

            if (i == value.length() - 1) {
                //'\\' is the last char
                result.append('\\');
                break;
            }

            char next_char = value.charAt(i + 1);
            switch (next_char) {
            case 'n':
                result.append('\n');
                ++i;
                break;
            case 't':
                result.append('\t');
                ++i;
                break;
            case 'r':
                result.append('\r');
                ++i;
                break;
            case '\\':
                result.append('\\');
                ++i;
                break;
            case 'b':
                result.append('\b');
                ++i;
                break;
            case 'f':
                result.append('\f');
                ++i;
                break;
            case '\"':
                result.append('\"');
                ++i;
                break;
            case '\'':
                result.append('\'');
                ++i;
                break;
            case '#':
                result.append('#');
                ++i;
                break;
            case 'u':
            {
                //e.g. "\\u{463}"
                int nEnd = value.indexOf('}',i);
                String hex = value.substring(i + 3, nEnd);
                char h = (char)Integer.parseInt(hex, 16);
                result.append(h);
                i = nEnd;
                break;
            }
            case 'x':
            {
                //e.g. "\x63", "\x0"
                String hex = value.substring(i + 2, i + (value.length() > 4 ? 4 : 3));
                char h = (char)Integer.parseInt(hex, 16);
                result.append(h);
                i += 3;
                break;
            }                
            default:
                if (Character.isDigit(next_char)) {
                    //e,g. "\142", '\0', '\56'
                    int number_length = 1;
                    while ((i + 1 + number_length) < value.length() &&
                            Character.isDigit(value.charAt(i + 1 + number_length)) &&
                            number_length <= 3) {
                        ++number_length;
                    }

                    String oct = value.substring(i + 1, i + 1 + number_length);
                    char o = (char)Integer.parseInt(oct, 8);
                    result.append(o);
                    i += number_length;
                } else {
                    result.append('\\');
                }
            }
        }

        return result.toString();
    }

    static String convertSingleQuoteString(String value)
    {
        StringBuilder result = new StringBuilder();

        for (int i = 0; i < value.length(); ++i)
        {
            if (value.charAt(i) != '\\')
            {
                result.append(value.charAt(i));
                continue;
            }

            if (i == value.length() - 1)
            {
                //'\\' is the last char
                result.append('\\');
                break;
            }

            char next_char = value.charAt(i + 1);
            switch (next_char)
            {
                case '\\':
                    result.append('\\');
                    ++i;
                    break;
                case '\'':
                    result.append('\'');
                    ++i;
                    break;
                default:
                    result.append(value.charAt(i));
                    break;
            }
        }

        return result.toString();
    }

    public void appendString(String value, boolean double_quote) {
        if (double_quote) {
            value_.append(convertDoubleQuoteString(value));
        } else {
            value_.append(convertSingleQuoteString(value));
        }
    }

    public void appendRegExString(String value) {
        value_.append(value);
    }

    public void accept(CodeVisitor visitor) {
        visitor.visitStringExpression(value_.toString());
    }
}
