<?xml version='1.0'?><!-- -*-SGML-*- -->

<!--
  Rearrange the elements of <author> so that <email>
  is not under <affiliation> but directly under <author>.
  This correction is specific for my own docbook files
  (the ones that I want to import).
-->

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version='1.0'>

<xsl:template match="bookinfo/author | articleinfo/author">
  <xsl:text>  </xsl:text>
  <xsl:copy>
    <xsl:if test="./firstname">
      <xsl:text>
      </xsl:text>
      <xsl:apply-templates select="./firstname" />
    </xsl:if>

    <xsl:if test="./surname">
      <xsl:text>
      </xsl:text>
      <xsl:apply-templates select="./surname" />
    </xsl:if>

    <xsl:if test=".//email">
      <xsl:text>
      </xsl:text>
      <xsl:element name="email">
        <xsl:value-of select=".//email" />
      </xsl:element>
    </xsl:if>

    <xsl:if test="./affiliation">
      <xsl:text>
      </xsl:text>
      <xsl:apply-templates select="./affiliation" />
    </xsl:if>

    <xsl:text>
    </xsl:text>
  </xsl:copy>
</xsl:template>


<!-- ignore author/affiliation/email -->
<xsl:template match="author/affiliation/email" />


</xsl:transform>