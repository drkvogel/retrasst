<xsl:stylesheet 
    version='1.0' 
    xmlns:xsl='http://www.w3.org/1999/XSL/Transform'
    xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' >

    <xsl:output method='text'/>

    <xsl:variable name="NULL_VALUE" select="'rabbit'"/>

    <xsl:template match='table_data'/>

    <xsl:template match='table_structure/field'>
        <xsl:variable name="isLast" select="not(following-sibling::field)"/>
        <xsl:value-of select="concat( @Field, ' = c0' )"/>
        <xsl:choose>
            <xsl:when test="$isLast"><xsl:text>nl</xsl:text></xsl:when>
            <xsl:otherwise><xsl:text>comma</xsl:text></xsl:otherwise>
        </xsl:choose>
        <xsl:text> with null('</xsl:text><xsl:value-of select="$NULL_VALUE"/><xsl:text>')</xsl:text>
        <xsl:if test="not( $isLast )"><xsl:text>,&#xa;</xsl:text></xsl:if>
    </xsl:template>

</xsl:stylesheet> 

