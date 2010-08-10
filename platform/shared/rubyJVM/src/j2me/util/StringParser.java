package j2me.util;

import java.util.*;
import j2me.lang.*;

public
class StringParser implements Enumeration 
{
	String m_strToSplit, m_strDelim;
	int m_nCurPos = 0;
	boolean m_bFinish = false;
	public StringParser(String strToSplit, String strDelim)
	{
		m_strToSplit = strToSplit;
		m_strDelim = strDelim;
	}
	
	public boolean hasMoreElements() 
	{
		return !m_bFinish;
	}

	public String getRestString()
	{
		return m_strToSplit.substring(m_nCurPos);
	}
	
	public Object nextElement() 
	{
		if ( m_nCurPos >= m_strToSplit.length() )
		{
			m_bFinish = true;
			return null;//"";
		}
		
		String strRes = "";
		
		if ( m_strDelim.length() == 0 )
		{
			strRes += m_strToSplit.charAt(m_nCurPos);
			m_nCurPos += 1;
			
			if ( m_nCurPos >= m_strToSplit.length() )
				m_bFinish = true;
		}
		else
		{
			int nStart = m_nCurPos;
			m_nCurPos = m_strToSplit.indexOf(m_strDelim, nStart);
			
			if ( m_nCurPos >= 0 )
			{
				if ( nStart == m_nCurPos )
					strRes = null;
				else
					strRes = m_strToSplit.substring(nStart, m_nCurPos);
				
				m_nCurPos += m_strDelim.length();
			}
			else
			{
				strRes = m_strToSplit.substring(nStart);
				m_bFinish = true;
			}
		}
		
		return strRes;
	}
	/*
    private int currentPosition;
    private int newPosition;
    private int maxPosition;
    private String str;
    private String delimiters;
    private boolean retDelims;
    private boolean delimsChanged;
    private int maxDelimCodePoint;
    private boolean hasSurrogates = false;
    private int[] delimiterCodePoints;
    
    public StringParser(String str, String delim, boolean returnDelims) {
        currentPosition = 0;
        newPosition = -1;
        delimsChanged = false;
        this.str = str;
        maxPosition = str.length();
        delimiters = delim;
        retDelims = returnDelims;
        makePoint();
    }
    
    public StringParser(String str, String delim) {
        this(str, delim, false);
    }
    
    public StringParser(String str) {
        this(str, " \t\n\r\f", false);
    }

    private int skipDelimiters(int startPos) {
        if (delimiters == null)
            throw new NullPointerException();

        int position = startPos;
        while (!retDelims && position < maxPosition) {
            if (!hasSurrogates) {
                char c = str.charAt(position);
                if ((c > maxDelimCodePoint) || (delimiters.indexOf(c) < 0))
                    break;
                position++;
            } else {
                int c = StringMe.codePointAt(str, position);
                if ((c > maxDelimCodePoint) || !isDelimiter(c)) {
                    break;
                }
                position += CharacterMe.charCount(c);
            }
        }
        return position;
    }

    private int parseTokens(int startPos) {
        int position = startPos;
        while (position < maxPosition) {
            if (!hasSurrogates) {
                char c = str.charAt(position);
                if ((c <= maxDelimCodePoint) && (delimiters.indexOf(c) >= 0))
                    break;
                position++;
            } else {
                int c = StringMe.codePointAt(str,position);
                if ((c <= maxDelimCodePoint) && isDelimiter(c))
                    break;
                position += CharacterMe.charCount(c);
            }
        }
        if (retDelims && (startPos == position)) {
            if (!hasSurrogates) {
                char c = str.charAt(position);
                if ((c <= maxDelimCodePoint) && (delimiters.indexOf(c) >= 0))
                    position++;
            } else {
                int c = StringMe.codePointAt(str,position);
                if ((c <= maxDelimCodePoint) && isDelimiter(c))
                    position += CharacterMe.charCount(c);
            }
        }
        return position;
    }

    private boolean isDelimiter(int codePoint) {
        for (int i = 0; i < delimiterCodePoints.length; i++) {
            if (delimiterCodePoints[i] == codePoint) {
                return true;
            }
        }
        return false;
    }

    public boolean hasMoreTokens() {
        newPosition = skipDelimiters(currentPosition);
        return (newPosition < maxPosition);
    }

    public String nextToken() {

        currentPosition = (newPosition >= 0 && !delimsChanged) ?  
            newPosition : skipDelimiters(currentPosition);

        /// Reset these anyway
        delimsChanged = false;
        newPosition = -1;

        if (currentPosition > maxPosition)
            throw new NoSuchElementException();
        if (currentPosition == maxPosition)
        	return "";
        
        int start = currentPosition;
        currentPosition = parseTokens(currentPosition);
        return str.substring(start, currentPosition);
    }

    public String nextToken(String delim) {
        delimiters = delim;

        // delimiter string specified, so set the appropriate flag.
        delimsChanged = true;

        makePoint();
        return nextToken();
    }

    public boolean hasMoreElements() {
        return hasMoreTokens();
    }

    public Object nextElement() {
        return nextToken();
    }

    public int countTokens() {
        int count = 0;
        int currpos = currentPosition;
        while (currpos < maxPosition) {
            currpos = skipDelimiters(currpos);
            //if (currpos >= maxPosition)
            //    break;
            currpos = parseTokens(currpos);
            count++;
        }
        return count;
    }
    
    private void makePoint() {
        if (delimiters == null) {
            maxDelimCodePoint = 0;
            return;
        }

        int m = 0;
        int c;
        int count = 0;
        for (int i = 0; i < delimiters.length(); i += CharacterMe.charCount(c)) {
            c = delimiters.charAt(i);
            if (c >= CharacterMe.MIN_HIGH_SURROGATE && c <= CharacterMe.MAX_LOW_SURROGATE) {
                c = StringMe.codePointAt(delimiters,i);
                hasSurrogates = true;
            }
            if (m < c)
                m = c;
            count++;
        }
        maxDelimCodePoint = m;

        if (hasSurrogates) {
            delimiterCodePoints = new int[count];
            for (int i = 0, j = 0; i < count; i++, j += CharacterMe.charCount(c)) {
                c = StringMe.codePointAt(delimiters,j);
                delimiterCodePoints[i] = c;
            }
        }
    }*/
    
}
