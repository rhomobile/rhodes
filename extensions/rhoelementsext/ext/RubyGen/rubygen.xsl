<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:output method="text"/> 
	<xsl:template match="/">
	<xsl:for-each select="ALLOBJECTS/FUNC">
module <xsl:call-template name="TitleCase"><xsl:with-param name="text" select="NAME"/></xsl:call-template> 
class &lt;&lt; self #means static methods
		<xsl:for-each select="PARAM">
	def <xsl:value-of select="NAME" />=(argument)
		RhoElementsExt.meta_proc('<xsl:value-of select="../NAME"/>', '<xsl:value-of select="NAME"/>:' + argument.to_s, '')
	end
	</xsl:for-each>
	<xsl:for-each select="METHOD">
	def <xsl:value-of select="NAME" />
		RhoElementsExt.meta_proc('<xsl:value-of select="../NAME"/>', '<xsl:value-of select="NAME"/>', '')
	end
	</xsl:for-each>
	<xsl:for-each select="RETURNS">
	def <xsl:value-of select="NAME" />=(argument)
		if argument.to_s.match('^url\(')
			RhoElementsExt.meta_proc('<xsl:value-of select="../NAME"/>', '<xsl:value-of select="NAME"/>:' + argument.to_s, '')
		else
			RhoElementsExt.meta_proc('<xsl:value-of select="../NAME"/>', '<xsl:value-of select="NAME"/>:url(\'' + argument.to_s + '\')', '')
		end
	end
	</xsl:for-each>
	def setEmml(argument)
		RhoElementsExt.meta_proc('<xsl:value-of select="NAME"/>', argument.to_s, '')
	end
end
end
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