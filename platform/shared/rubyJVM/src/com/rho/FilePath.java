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

    public String changeBaseName( String szFileName )
    {
        int base = findLastSlash();
        if ( base >= 0 && base < m_szPath.length()-1 )
        {
            String res = m_szPath.substring(0, base+1);
            res += szFileName;

            return res;
        }

        return makeFullPath(szFileName);
    }

    static public String join(String path1, String path2)
    {
        boolean bSlash1 = path1.length()>0 && (path1.charAt(path1.length()-1) == '/' || path1.charAt(path1.length()-1) == '\\');
        boolean bSlash2 = path2.length()>0 && (path2.charAt(0) == '/' || path2.charAt(0) == '\\');
        String res;
        if (bSlash1 && bSlash2)
            res = path1 + path2.substring(1);
        else if ( bSlash1 || bSlash2 )
            res = path1 + path2;
        else
            res = path1 + '/' + path2;

        return res;
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
