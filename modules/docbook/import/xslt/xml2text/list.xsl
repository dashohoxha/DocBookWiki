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

<!-- ordered and unordered lists -->

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version='1.0'>


<!-- print out a list -->
<xsl:template match="orderedlist|itemizedlist">
  <xsl:if test="ancestor::itemizedlist | ancestor::orderedlist">
    <xsl:text>
</xsl:text>
  </xsl:if>
  <xsl:apply-templates />

  <xsl:variable name="indent">
    <xsl:apply-templates select="." mode="get-indent" />
  </xsl:variable>
  <xsl:variable name="following-text">
    <xsl:apply-templates select="following-sibling::text()" />
  </xsl:variable>

  <xsl:if test="following-sibling::programlisting
                or following-sibling::literallayout
                or following-sibling::screen
                or following-sibling::itemizedlist
                or following-sibling::orderedlist
                or $following-text!=''
                or (following::* and not(following::listitem))">
    <xsl:value-of select="$indent" />
    <xsl:text>/
</xsl:text>
  </xsl:if>
</xsl:template>


<!-- print out a list item -->
<xsl:template match="listitem">
  <xsl:variable name="indent">
    <xsl:apply-templates select=".." mode="get-indent" />
  </xsl:variable>
  <xsl:variable name="bullet">
    <xsl:apply-templates select=".." mode="get-bullet" />
  </xsl:variable>

  <xsl:text>
</xsl:text>
  <xsl:value-of select="$indent" />
  <xsl:value-of select="$bullet" />
  <xsl:value-of select="' '" />
  <xsl:apply-templates />

  <xsl:if test="not(descendant::orderedlist or descendant::itemizedlist)">
    <xsl:text>
</xsl:text>
  </xsl:if>
</xsl:template>

<xsl:template match="listitem/para">
  <xsl:apply-templates />
</xsl:template>


<!-- get-indent -->
<xsl:template match="itemizedlist|orderedlist" mode="get-indent">
  <xsl:choose>
    <xsl:when test="ancestor::itemizedlist or ancestor::orderedlist">
      <xsl:apply-templates select="../../.." mode="get-indent" />
      <xsl:value-of select="'  '" />
    </xsl:when>
    <xsl:otherwise><xsl:value-of select="''" /></xsl:otherwise>
  </xsl:choose>
</xsl:template>

<xsl:template match="*" mode="get-indent" />


<!-- get-bullet -->
<xsl:template match="itemizedlist" mode="get-bullet">
  <xsl:text>*</xsl:text>
</xsl:template>

<xsl:template match="orderedlist" mode="get-bullet">
  <xsl:choose>
    <xsl:when test="@numeration='arabic'">
      <xsl:text>1.</xsl:text>
    </xsl:when>
    <xsl:when test="@numeration='loweralpha'">
      <xsl:text>a.</xsl:text>
    </xsl:when>
    <xsl:when test="@numeration='lowerroman'">
      <xsl:text>i.</xsl:text>
    </xsl:when>
    <xsl:when test="@numeration='upperalpha'">
      <xsl:text>A.</xsl:text>
    </xsl:when>
    <xsl:when test="@numeration='upperroman'">
      <xsl:text>I.</xsl:text>
    </xsl:when>
    <xsl:otherwise><xsl:text>1.</xsl:text></xsl:otherwise>
  </xsl:choose>
</xsl:template>


</xsl:transform>
