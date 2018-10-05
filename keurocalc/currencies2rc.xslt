<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output method="text" encoding="utf-8" omit-xml-declaration="yes"/>
<xsl:template match="/currencies/currency">
// i18n: file: <xsl:value-of select="$filename"/>
i18n("<xsl:value-of select="@name"/>");
</xsl:template>
</xsl:stylesheet>
