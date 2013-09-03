<?xml version='1.0'?><!-- -*-SGML-*- -->
<!--
  Transform the XHTML content to an XML suitable to be converted
  to DocBookWiki. Of course, there is no one-to-one relationship
  between XHTML and DocBookWiki tags, so only a small part of them
  will be converted, and the rest will be lost.

  This convertion is done only for the content of a section, so
  there is no need to worry for the title and the id of the
  section, etc.
-->

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
<xsl:output method="xml" version="1.0" encoding="utf-8"
            omit-xml-declaration="no" standalone="no" indent="no" />


<!-- skip head -->
<xsl:template match="/">
  <xsl:apply-templates select="./html/body" />
</xsl:template>

<xsl:template match="body">
  <section><xsl:apply-templates /></section>
</xsl:template>

<!-- paragraph -->
<xsl:template match="p">
  <para><xsl:apply-templates /></para>
</xsl:template>

<!-- inline markup -->
<xsl:template match="em | strong">
  <emphasis><xsl:apply-templates /></emphasis>
</xsl:template>


<!-- lists -->
<xsl:template match="ul">
  <itemizedlist>
    <xsl:apply-templates />
  </itemizedlist>
</xsl:template>

<xsl:template match="li">
  <listitem>
    <para><xsl:apply-templates /></para>
  </listitem>
</xsl:template>

<xsl:template match="ol">
  <orderedlist>
    <xsl:apply-templates />
  </orderedlist>
</xsl:template>

<xsl:template match="ol/@type">
  <xsl:attribute name="numeration">
    <xsl:choose>
      <xsl:when test=".='a'"><xsl:value-of select="'loweralpha'"/></xsl:when>
      <xsl:when test=".='A'"><xsl:value-of select="'upperalpha'"/></xsl:when>
      <xsl:when test=".='i'"><xsl:value-of select="'lowerroman'"/></xsl:when>
      <xsl:when test=".='I'"><xsl:value-of select="'upperroman'"/></xsl:when>
      <xsl:otherwise><xsl:value-of select="'arabic'"/></xsl:otherwise>
    </xsl:choose>
  </xsl:attribute>
</xsl:template>


<!-- images -->
<xsl:template match="img">
<mediaobject>
  <imageobject>
    <imagedata fileref="{@src}">
      <xsl:if test="not(@width='')"><xsl:copy-of select="@width"/></xsl:if>
      <xsl:copy-of select="@height"/>
    </imagedata>
  </imageobject>
  <xsl:if test="@alt">
    <textobject><phrase><xsl:value-of select="@alt"/></phrase></textobject>
  </xsl:if>
</mediaobject>
</xsl:template>


<!-- hyperlinks -->
<xsl:template match="a[@href != '']">
  <xsl:element name="ulink">
    <xsl:attribute name="url">
      <xsl:value-of select="normalize-space(@href)"/>
    </xsl:attribute>
    <xsl:apply-templates/>
  </xsl:element>
</xsl:template>


<!-- pre-formated -->
<xsl:template match="pre">
  <programlisting><xsl:apply-templates/></programlisting>
</xsl:template>


<!-- ignore all the other attributes -->
<xsl:template match="@*" />

<!-- ignore all the other elements, but keep their text content -->
<xsl:template match="*">
  <xsl:value-of select="normalize-space(.)" />
</xsl:template>


<!-- 'adminitions.xsl' is an example of customizing
     the transformation depending on the specific case
     of the HTML content that is being converted.  -->
<xsl:include href="xhtml2dbw/admonitions.xsl" />


</xsl:transform>
