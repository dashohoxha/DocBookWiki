<?xml version='1.0'?><!-- -*-SGML-*- -->

<!-- defines the template 'set-id', which generates
     and sets an id for a section, if it is missing  -->

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version='1.0'>

<xsl:template name="set-id">
  <xsl:if test="not(@id)">
    <xsl:attribute name="id">
      <xsl:variable name="count">
	<xsl:apply-templates select="." mode="count" />
      </xsl:variable>
      <xsl:value-of select="concat(name(.), '-', $count)" />
    </xsl:attribute>
  </xsl:if>
</xsl:template>

<xsl:template match="book | article" mode="count">
  <xsl:value-of select="0"/>
</xsl:template>

<xsl:template match="bookinfo | articleinfo" mode="count">
  <xsl:value-of select="1"/>
</xsl:template>

<xsl:template match="preface" mode="count">
  <xsl:number count="preface" from="book" level="single" format="01" />
</xsl:template>

<xsl:template match="chapter" mode="count">
  <xsl:number count="chapter" from="book" level="single" format="01" />
</xsl:template>

<xsl:template match="appendix" mode="count">
  <xsl:number count="appendix" from="/" level="any" format="01"/>
</xsl:template>

<xsl:template match="bibliography" mode="count">
  <xsl:number count="bibliography" from="/" level="any" format="01"/>
</xsl:template>

<xsl:template match="section" mode="count">
  <xsl:number count="section" from="/" level="any" format="001" />
</xsl:template>

<xsl:template match="simplesect" mode="count">
  <xsl:number count="simplesect" from="/" level="any" format="001" />
</xsl:template>

<xsl:template match="sect1" mode="count">
  <xsl:number count="sect1" from="/" level="any" format="01" />
</xsl:template>

<xsl:template match="sect2" mode="count">
  <xsl:number count="sect2" from="/" level="any" format="01" />
</xsl:template>

<xsl:template match="sect3" mode="count">
  <xsl:number count="sect3" from="/" level="any" format="01" />
</xsl:template>

<xsl:template match="sect4" mode="count">
  <xsl:number count="sect4" from="/" level="any" format="01" />
</xsl:template>

<xsl:template match="sect4" mode="count">
  <xsl:number count="sect5" from="/" level="any" format="01" />
</xsl:template>

</xsl:transform>
