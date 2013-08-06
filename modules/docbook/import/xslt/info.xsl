<?xml version='1.0'?><!-- -*-SGML-*- -->
<!--
This file  is part of  DocBookWiki.  DocBookWiki is a  web application
that  displays  and  edits  DocBook  documents.

Copyright (C) 2004, 2005 Dashamir Hoxha, dashohoxha@users.sf.net
Copyright (C) 2013 Dashamir Hoxha, dashohoxha@gmail.com

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

<!-- bookinfo and articleinfo -->

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version='1.0'>


<!-- bookinfo or articleinfo -->
<xsl:template match="bookinfo | articleinfo">
  <!-- author, abstract and keywordset -->
  <xsl:apply-templates select="./author" />
  <xsl:apply-templates select="./abstract" />
  <xsl:apply-templates select="./keywordset" />
  <xsl:apply-templates select="./date" />
  <xsl:apply-templates select="./releaseinfo" />

  <!-- legalnotice (copyright) -->
  <xsl:apply-templates select="./legalnotice" />
  <xsl:if test="not(./legalnotice)">
    <xsl:text>
@copyright: Copyright (C) 2004, 2005 Firstname Lastname. Permission is
granted to copy, distribute and/or modify this document under the terms
of the GNU Free Documentation License, Version 1.1 or any later version
published by the Free Software Foundation; with no Invariant Sections,
with no Front-Cover Texts, and with no Back-Cover Texts. A copy of the
license is included in the section entitled
"GNU Free Documentation License."</xsl:text>
  </xsl:if>

</xsl:template>


<!-- author -->
<xsl:template match="author">
  <xsl:text>
@author: </xsl:text>
  <xsl:apply-templates select="./surname" />
  <xsl:text>, </xsl:text>
  <xsl:apply-templates select="./firstname" />
  <xsl:text>, </xsl:text>
  <xsl:apply-templates select="./affiliation/address/email" />
  <xsl:text>, </xsl:text>
  <xsl:apply-templates select="./affiliation/orgname" />

  <xsl:text>
</xsl:text>
</xsl:template>


<!-- abstract -->
<xsl:template match="abstract">
<xsl:text>
@abstract: </xsl:text>

  <xsl:apply-templates />

  <xsl:text>
</xsl:text>
</xsl:template>


<!-- keywordset -->
<xsl:template match="keywordset">
<xsl:text>
@keywords: </xsl:text>

  <xsl:apply-templates />

  <xsl:text>
</xsl:text>
</xsl:template>


<!-- keyword -->
<xsl:template match="keyword">
  <xsl:apply-templates />
  <xsl:text>, </xsl:text>
</xsl:template>


<!-- date -->
<xsl:template match="date">
<xsl:text>
@date: </xsl:text>

  <xsl:apply-templates />

  <xsl:text>
</xsl:text>
</xsl:template>


<!-- releaseinfo -->
<xsl:template match="releaseinfo">
<xsl:text>
@releaseinfo: </xsl:text>

  <xsl:apply-templates />

  <xsl:text>
</xsl:text>
</xsl:template>


<!-- legalnotice -->
<xsl:template match="legalnotice">
<xsl:text>
@copyright: </xsl:text>
  <xsl:apply-templates />
</xsl:template>


<!-- elements that are skipped -->
<xsl:template match="abstract/para | legalnotice/para
                     | surname | firstname | orgname
                     | affiliation/address/email">
  <xsl:apply-templates />
</xsl:template>

<!-- ignore the title and any sections -->
<xsl:template match="title | chapter | section | simplesect" />

<!-- ignore the (empty) text of some elements -->
<xsl:template match="book/text() | article/text()
                     | bookinfo/text() | articleinfo/text()" />

</xsl:transform>
