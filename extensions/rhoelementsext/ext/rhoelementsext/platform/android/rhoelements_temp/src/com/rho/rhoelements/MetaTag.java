package com.rho.rhoelements;

/**
 * A container class to hold 2 strings that describe a MetaTag
 * @author Geoff Day, Ben Kennedy
 */
public class MetaTag
{
    private String equiv;
    private String content;

    public MetaTag (String equiv, String content)
    {
	this.equiv = equiv;
	this.content = content;
    }

    /**
     * @return the equiv
     */
    public String getEquiv()
    {
        return equiv;
    }

    /**
     * @param equiv the equiv to set
     */
    public void setEquiv(String equiv)
    {
        this.equiv = equiv;
    }

    /**
     * @return the content
     */
    public String getContent()
    {
        return content;
    }

    /**
     * @param content the content to set
     */
    public void setContent(String content)
    {
        this.content = content;
    }
}