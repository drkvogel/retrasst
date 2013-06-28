<xsl:stylesheet 
    version='1.0' 
    xmlns:xsl='http://www.w3.org/1999/XSL/Transform'
    xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' >

    <xsl:output method='text'/>

    <xsl:variable name="NULL_VALUE" select="'rabbit'"/>

    <xsl:template match='table_structure'/>

    <xsl:template match='field'>
        <xsl:choose>
            <xsl:when test="@xsi:nil = 'true'"><xsl:value-of select="$NULL_VALUE"/></xsl:when>
            <xsl:otherwise><xsl:value-of select='.'/></xsl:otherwise>
        </xsl:choose>
        <xsl:choose>
            <xsl:when test="following-sibling::field"><xsl:text>,</xsl:text></xsl:when>
            <xsl:otherwise><xsl:text>&#xA;</xsl:text></xsl:otherwise>
        </xsl:choose>
    </xsl:template>

</xsl:stylesheet> 
