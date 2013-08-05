<?xml version='1.0'?><!-- -*-SGML-*- -->

<!-- Used to convert content.xml to text, in text editing mode. -->

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version='1.0'>

<xsl:output method="xml" version="1.0" encoding="utf-8"
            omit-xml-declaration="yes" standalone="yes" indent="yes" />

<xsl:include href="xml2text/block.xsl" />
<xsl:include href="xml2text/list.xsl" />
<xsl:include href="xml2text/inline.xsl" />
<xsl:include href="xml2text/menuchoice.xsl" />
<xsl:include href="xml2text/biblioentry.xsl" />

<!-- skip the root element and the section elements under the root -->
<xsl:template match="/* | /*/book | /*/article | /*/bookinfo | /*/articleinfo
                    | /*/preface | /*/appendix | /*/bibliography
                    | /*/chapter | /*/section | /*/simplesect">
  <xsl:apply-templates />
</xsl:template>

<!-- ignore the titles -->
<xsl:template match="title" />

<!-- ignore any child sub-sections -->
<xsl:template match="book | article | bookinfo | articleinfo
                    | preface | appendix | bibliography
                    | chapter | section | simplesect" />

<xsl:template match="para" xml:space="preserve">
<xsl:apply-templates />
</xsl:template>

<!-- normalize text, but not for screen or programlisting or literallayout -->
<xsl:template match="text()">
  <xsl:value-of select="normalize-space(.)" />
</xsl:template>
<xsl:template match="screen/text() | programlisting/text()
                    | literallayout/text()">
  <xsl:value-of select="." />
</xsl:template>

<!-- copy the rest of elements (and their attributes) unchanged -->
<xsl:template match="*|@*">
  <xsl:copy>
    <xsl:apply-templates select="@*"/>
    <xsl:apply-templates select="node()"/>
  </xsl:copy>
</xsl:template>

</xsl:transform>
