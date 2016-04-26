<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:output method="text"/> 
	<xsl:template match="/">
	<xsl:for-each select="ALLOBJECTS/FUNC">
	<xsl:if test="NAME != 'History:Back'">
window.<xsl:value-of select="NAME" /> = new (function() {
    <xsl:for-each select="PARAM">
    Object.defineProperty(this, '<xsl:value-of select="NAME" />', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('<xsl:value-of select="../NAME"/>', '<xsl:value-of select="NAME"/>:' + val.toString());
        }
    });
	</xsl:for-each>
	<xsl:for-each select="METHOD">
    this['<xsl:value-of select="NAME" />'] = function() {
        metaReceiver.setMeta('<xsl:value-of select="../NAME"/>', '<xsl:value-of select="NAME"/>');
    }
	</xsl:for-each>
	<xsl:for-each select="RETURNS">
    Object.defineProperty(this, '<xsl:value-of select="NAME" />', {
        enumerable: true,
        get: function () { return null; /* not supported by design */ },
        set: function(val) {
            metaReceiver.setMeta('<xsl:value-of select="../NAME"/>', '<xsl:value-of select="NAME"/>:' + val.toString());
        }
    });
	</xsl:for-each>
    this['setEMML'] = function(val) {
        metaReceiver.setMeta('<xsl:value-of select="NAME"/>', val.toString());
    }
}) ();
    </xsl:if>
    </xsl:for-each>
	</xsl:template>
	
	<xsl:template name="TitleCase">  
		<xsl:param name="text" />  
		<xsl:param name="lastletter" select="' '"/>  
		<xsl:if test="$text">  
			<xsl:variable name="thisletter" select="substring($text,1,1)"/>  
			<xsl:choose>  
				<xsl:when test="$lastletter=' '">  
					<xsl:value-of select="translate($thisletter,'abcdefghijklmnopqrstuvwxyz', 'ABCDEFGHIJKLMNOPQRSTUVWXYZ')"/>
				</xsl:when>  
				<xsl:otherwise>  
					<xsl:value-of select="$thisletter"/>  
				</xsl:otherwise>  
			</xsl:choose>  
			<xsl:call-template name="TitleCase">  
				<xsl:with-param name="text" select="substring($text,2)"/>  
				<xsl:with-param name="lastletter" select="$thisletter"/>  
			</xsl:call-template>  
		</xsl:if>  
	</xsl:template> 
	
</xsl:stylesheet>
