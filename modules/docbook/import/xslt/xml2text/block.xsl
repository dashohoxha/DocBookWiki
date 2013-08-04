<?xml version='1.0'?><!-- -*-SGML-*- -->
<!--
This file  is part of  DocBookWiki.  DocBookWiki is a  web application
that  displays  and  edits  DocBook  documents.  

Copyright (C) 2004, 2005 Dashamir Hoxha, dashohoxha@users.sf.net

DocBookWiki is free software; you can redistribute it and/or modify it
under the terms of the GNU  General Public License as published by the
Free Software Foundation; either version 2 of the License, or (at your
option) any later version.

DocBookWiki is  distributed in  the hope that  it will be  useful, but
WITHOUT   ANY  WARRANTY;   without  even   the  implied   warranty  of
MERCHANTABILITY  or FITNESS  FOR A  PARTICULAR PURPOSE.   See  the GNU
General Public License for more details.

You  should have received  a copy  of the  GNU General  Public License
along with DocBookWiki; if not, write to the Free Software Foundation,
Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
-->

<!-- block elements (figure, screen, programlisting, etc. -->

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version='1.0'>


<xsl:template match="screen|programlisting|literallayout|figure|example
                    |note|warning|caution|important|tip">
  <xsl:variable name="tag">
    <xsl:call-template name="get-short-name">
      <xsl:with-param name="name" select="name(.)"/>
    </xsl:call-template>
  </xsl:variable>

<xsl:text>
--</xsl:text><xsl:value-of select="$tag"/><xsl:text> </xsl:text>
  <xsl:value-of select="./title" /><xsl:text>
</xsl:text>

  <xsl:apply-templates />

  <xsl:text>
----
</xsl:text>
</xsl:template>


<xsl:template match="note/para | warning/para | caution/para
                    | important/para | tip/para">
  <xsl:apply-templates />
</xsl:template>


<!-- for an xml tag, get the corresponding short name -->
<xsl:template name="get-short-name">
  <xsl:param name="name" />

  <xsl:choose>

    <xsl:when test="$name='screen'">
      <xsl:value-of select="'scr'"/>
    </xsl:when>

    <xsl:when test="$name='literallayout'">
      <xsl:value-of select="'ll'"/>
    </xsl:when>

    <xsl:when test="$name='programlisting'">
      <xsl:value-of select="'code'"/>
    </xsl:when>

    <xsl:when test="$name='important'">
      <xsl:value-of select="'imp'"/>
    </xsl:when>

    <xsl:when test="$name='figure'">
      <xsl:value-of select="'fig'"/>
    </xsl:when>

    <xsl:when test="$name='example'">
      <xsl:value-of select="'xmp'"/>
    </xsl:when>

    <!-- if nothing matches, return the name itself -->
    <xsl:otherwise><xsl:value-of select="$name"/></xsl:otherwise>

  </xsl:choose>

</xsl:template>


<xsl:template match="comment">
<xsl:text>
</xsl:text>
  <xsl:copy><xsl:apply-templates /></xsl:copy>
<xsl:text>
</xsl:text>
</xsl:template>

</xsl:transform>
