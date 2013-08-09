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

<!-- convert biblioentries to text/wiki -->

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version='1.0'>


<!-- biblioentry -->
<xsl:template match="biblioentry">
  <xsl:value-of select="./abbrev" />
  <xsl:text>: </xsl:text>
  <xsl:value-of select="./biblioset/pubdate" />
  <xsl:text>, </xsl:text>
  <xsl:value-of select="./biblioset[@relation='analytic']/authorgroup/firstname" />
  <xsl:text> </xsl:text>
  <xsl:value-of select="./authorgroup/othername" />
  <xsl:text> </xsl:text>
  <xsl:value-of select="./authorgroup/lastname" />

  <xsl:text>
</xsl:text>
</xsl:template>


</xsl:transform>
