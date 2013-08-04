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

<!-- inline elements -->

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version='1.0'>


<!-- output an empty space only if not in
     programlisting or screen or literallayout -->
<xsl:template name="output-space">
  <xsl:if test="not(ancestor::programlisting or ancestor::screen 
                    or ancestor::literallayout)">
    <xsl:text> </xsl:text>
  </xsl:if>
</xsl:template>


<!-- ~filename~ -->
<xsl:template match="filename">
  <xsl:call-template name="output-space" />
  <xsl:text>~</xsl:text>
  <xsl:apply-templates />
  <xsl:text>~</xsl:text>
  <xsl:call-template name="output-space" />
</xsl:template>


<!-- _emphasis_ -->
<xsl:template match="emphasis">
  <xsl:call-template name="output-space" />
  <xsl:text>_</xsl:text>
  <xsl:apply-templates />
  <xsl:text>_</xsl:text>
  <xsl:call-template name="output-space" />
</xsl:template>


<!-- `command` -->
<xsl:template match="command">
  <xsl:choose>
    <xsl:when test="preceding-sibling::prompt and ancestor::screen">
      <xsl:apply-templates />
    </xsl:when>
    <xsl:otherwise>
      <xsl:call-template name="output-space" />
      <xsl:text>`</xsl:text>
      <xsl:apply-templates />
      <xsl:text>`</xsl:text>
      <xsl:call-template name="output-space" />
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>


<!-- prompt!# command -->
<xsl:template match="prompt">
  <xsl:variable name="prompt">
    <xsl:apply-templates />
  </xsl:variable>
  <xsl:value-of select="$prompt" />
  <xsl:if test="not($prompt='bash#' or $prompt='bash$')">
    <xsl:text>!#</xsl:text>
  </xsl:if>
  <xsl:call-template name="output-space" />
</xsl:template>


<!-- ulink -->
<xsl:template match="ulink">
  <xsl:variable name="label">
    <xsl:value-of select="normalize-space(.)" />
  </xsl:variable>

  <xsl:call-template name="output-space" />
  <xsl:text>[</xsl:text>
  <xsl:if test="$label!=''">
    <xsl:value-of select="concat(.,'>')"/>
  </xsl:if>
  <xsl:value-of select="@url"/>
  <xsl:text>]</xsl:text>
  <xsl:call-template name="output-space" />
</xsl:template>


<!-- cross reference -->
<xsl:template match="xref">
  <xsl:call-template name="output-space" />
  <xsl:text>[</xsl:text>
    <xsl:value-of select="concat('>', @linkend)"/>
  <xsl:text>]</xsl:text>
  <xsl:call-template name="output-space" />
</xsl:template>


<!-- bibliography reference -->
<xsl:template match="citation">
  <xsl:call-template name="output-space" />
  <xsl:text>[</xsl:text>
    <xsl:value-of select="concat('->', xref/@linkend)"/>
  <xsl:text>]</xsl:text>
  <xsl:call-template name="output-space" />
</xsl:template>


<!-- footnote -->
<xsl:template match="footnote">
  <xsl:text>[/</xsl:text>
    <xsl:value-of select="./para"/>
  <xsl:text>]</xsl:text>
  <xsl:call-template name="output-space" />
</xsl:template>


<!-- media object -->
<xsl:template match="mediaobject">
  <xsl:call-template name="output-space" />

  <xsl:text>[&lt;</xsl:text>
  <xsl:value-of select="./imageobject/imagedata/@fileref" />

  <xsl:text>&lt;</xsl:text>
  <xsl:if test="./imageobject/imagedata/@width">
    <xsl:value-of select="./imageobject/imagedata/@width" />    
  </xsl:if>

  <xsl:text>&lt;</xsl:text>
  <xsl:if test="./textobject/phrase">
    <xsl:value-of select="./textobject/phrase" />    
  </xsl:if>

  <xsl:text>]</xsl:text>
  <xsl:call-template name="output-space" />
</xsl:template>


</xsl:transform>