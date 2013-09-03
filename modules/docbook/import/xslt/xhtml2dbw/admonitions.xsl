<?xml version='1.0'?><!-- -*-SGML-*- -->

<!--
  This is an example of how to use the 'class' attribute
  for making decisions about how to transform certain
  XHTML parts to DocBook. The hints that can be used depend
  on the specific case of the XHTML content that is being
  converted.
-->

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version='1.0'>

<xsl:template match="p[contains(@class,'note')]
                 | div[contains(@class,'note')]">
  <note><para><xsl:apply-templates/></para></note>
</xsl:template>

<xsl:template match="p[contains(@class,'warning')]
                 | div[contains(@class,'warning')]">
  <warning><para><xsl:apply-templates/></para></warning>
</xsl:template>

<xsl:template match="p[contains(@class,'caution')]
                 | div[contains(@class,'caution')]">
  <caution><para><xsl:apply-templates/></para></caution>
</xsl:template>

<xsl:template match="p[contains(@class,'important')]
                 | div[contains(@class,'important')]">
  <important><para><xsl:apply-templates/></para></important>
</xsl:template>

<xsl:template match="p[contains(@class,'tip')]
                 | div[contains(@class,'tip')]">
  <tip><para><xsl:apply-templates/></para></tip>
</xsl:template>

</xsl:transform>
