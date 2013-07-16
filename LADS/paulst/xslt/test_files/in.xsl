<xsl:stylesheet version='1.0' xmlns:xsl='http://www.w3.org/1999/XSL/Transform'>
<xsl:output method='xml' indent='yes'/>
<xsl:include href="included.xsl"/>
<xsl:template match='root'>
<xsl:copy>  
<xsl:copy-of select='@*'/>
<xsl:attribute name='new_attr'>stuff</xsl:attribute>
<xsl:apply-templates/> 
</xsl:copy>  
</xsl:template>
</xsl:stylesheet> 
