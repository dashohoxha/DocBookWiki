<?xml version='1.0'?><!-- -*-SGML-*- -->

<!-- Returns the title of a section. -->

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
<xsl:output method="text" encoding="utf-8" />

<!-- return the value of title -->
<xsl:template match="/">
  <xsl:value-of select="/*/*/title" />
</xsl:template>

</xsl:transform>
