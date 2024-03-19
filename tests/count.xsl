<?xml version="1.0"?>

<xsl:stylesheet version="1.0"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:m="http://www.w3.org/1998/Math/MathML"
>

<xsl:param name="test" select="'6'"/>
<xsl:output method="text"/>

<xsl:template match="/">
  <xsl:choose>
    <xsl:when test="$test='1'">
      <xsl:value-of select="count(descendant::node())"/>
    </xsl:when>
    <xsl:when test="$test='2'">
      <xsl:value-of select="count(descendant::m:mrow[attribute::xref])"/>
    </xsl:when>
    <xsl:when test="$test='3'">
      <xsl:value-of select="count(descendant::m:mrow[attribute::xref]/child::text())"/>
    </xsl:when>
    <xsl:when test="$test='4'">
      <xsl:value-of select="count(descendant::text()[parent::m:mrow[attribute::xref]])"/>
    </xsl:when>
    <xsl:when test="$test='5'">
      <xsl:value-of select="count(descendant::*[attribute::xref][child::text()])"/>
    </xsl:when>
    <xsl:when test="$test='6'">
      <xsl:value-of select="count(descendant::text()/parent::*[attribute::xref])"/>
    </xsl:when>
  </xsl:choose>
  <xsl:text>
</xsl:text>
</xsl:template>

</xsl:stylesheet>

