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

<!-- menuchoice -->

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version='1.0'>


<xsl:template match="menuchoice">
  <xsl:text> [</xsl:text>
  <xsl:apply-templates />

  <xsl:if test="./shortcut">
    <xsl:text> (</xsl:text>
    <xsl:apply-templates select="./shortcut/*" />
    <xsl:text>)</xsl:text>
  </xsl:if>

  <xsl:text>] </xsl:text>
</xsl:template>

<xsl:template match="menuchoice/shortcut" />


<xsl:template match="keycombo">
  <xsl:if test="not(ancestor::menuchoice)">
    <xsl:text> [(</xsl:text>
  </xsl:if>

  <xsl:apply-templates />

  <xsl:if test="not(ancestor::menuchoice)">
    <xsl:text>)] </xsl:text>
  </xsl:if>
</xsl:template>


<xsl:template match="keycombo/keycap">
  <xsl:apply-templates />
  <xsl:if test="following-sibling::keycap">
    <xsl:text>-</xsl:text>
  </xsl:if>
</xsl:template>


<xsl:template match="guimenu">
  <xsl:if test="not(ancestor::menuchoice)">
    <xsl:text> [</xsl:text>
  </xsl:if>

  <xsl:apply-templates />

  <xsl:if test="not(ancestor::menuchoice)">
    <xsl:text>-&gt;] </xsl:text>
  </xsl:if>
</xsl:template>


<xsl:template match="guisubmenu | guimenuitem">
  <xsl:if test="not(ancestor::menuchoice)">
    <xsl:text> [</xsl:text>
  </xsl:if>

  <xsl:text>-&gt;</xsl:text>
  <xsl:apply-templates />

  <xsl:if test="not(ancestor::menuchoice)">
    <xsl:text>] </xsl:text>
  </xsl:if>
</xsl:template>


<xsl:template match="accel">
  <xsl:apply-templates />
  <xsl:text>_</xsl:text>
</xsl:template>


<xsl:template match="guimenu/text() | guisubmenu/text() | guimenuitem/text()">
  <xsl:value-of select="normalize-space(.)" />
</xsl:template>


</xsl:transform>