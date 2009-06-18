package com.rho;

public class FilePath
{
    public FilePath( String str) { m_szPath = str; }

    public String getBaseName(){ 
        int base = findLastSlash();
        if ( base >=0 )
            return m_szPath.substring(base+1);

        return m_szPath;
    }

    String makeFullPath(String szFileName){
        String res = m_szPath;
        if ( res.length() > 0 && findLastSlash() < 0 )
            res += "/";

        res += szFileName;
        
        return res;
    }

    String changeBaseName( String szFileName )
    {
        int base = findLastSlash();
        if ( base >= 0 && base < m_szPath.length()-1 )
        {
            String res = m_szPath.substring(0, base);
            res += szFileName;

            return res;
        }

        return makeFullPath(szFileName);
    }

    int findLastSlash()
    {
        int slash = m_szPath.lastIndexOf('/');
        if ( slash < 0 )
            slash = m_szPath.lastIndexOf('\\');

        return slash;
    }

    String m_szPath;
}
