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

<!--
  Transform the xml_file to a form suitable to be imported to DocBookWiki.
  For example, replaces any elements <sectX> by the element <section>.
-->

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
<xsl:output method="xml" version="1.0" encoding="utf-8"
            omit-xml-declaration="no" standalone="no" indent="yes"
            doctype-public="-//OASIS//DTD DocBook XML V4.5//EN"
            doctype-system="http://docbook.org/xml/4.5/docbookx.dtd" />

<!-- define template 'set-id', which sets the id of a section,
     if it is missing -->
<xsl:include href="set-id.xsl" />

<!-- copy unchanged everything that is reckognized by DocBookWiki -->
<!-- this is the full list of elements and attributes that are
     reckognized and processed by DocBookWiki (comment and cdata
     are not docbook tags, they are introduced by preprocessing) -->
<xsl:template match="book | article | comment | cdata
        | bookinfo | articleinfo
        | author | firstname | surname | email | affiliation | orgname
        | abstract | keywordset | keyword | date | releaseinfo | legalnotice

        | preface | appendix | bibliography
        | chapter | section | simplesect | @id
        | title | para | text()

        | screen | programlisting | literallayout | figure | example
        | note | warning | caution | important|tip

        | orderedlist | itemizedlist| listitem | @numeration

        | filename | emphasis | command | prompt | ulink | xref
        | citation | footnote
        | mediaobject | imageobject | imagedata
        | @url | @linkend | @fileref | @width

        | menuchoice | shortcut | keycombo | keycap
        | guimenu | guisubmenu | guimenuitem | accel

        | biblioentry | abbrev | biblioset | pubdate | @relation
        | authorgroup | firstname | othername | lastname">
  <xsl:copy>
    <xsl:apply-templates select="@*"/>
    <xsl:apply-templates select="node()"/>
  </xsl:copy>
</xsl:template>


<!-- ignore all the other attributes -->
<xsl:template match="@*" />

<!-- ignore all the other elements, but keep their text content -->
<xsl:template match="*">
  <xsl:value-of select="normalize-space(.)" />
</xsl:template>


<!-- call template 'set-id' for section elements -->
<xsl:template match="chapter | section | simplesect
                     | preface | appendix | bibliography">
  <xsl:copy>
    <xsl:call-template name="set-id" />
    <xsl:apply-templates select="@*"/>
    <xsl:apply-templates select="node()"/>
  </xsl:copy>
</xsl:template>


<!-- convert <sectX> elements to <section> elements -->
<xsl:template match="sect1|sect2|sect3|sect4|sect5">
  <section>
    <xsl:call-template name="set-id" />
    <xsl:apply-templates select="@*"/>
    <xsl:apply-templates />
  </section>
</xsl:template>


<!-- included templates will override the ones above -->
<!-- 'author.xsl' makes a correction that is specific
     for my docbook files (that I want to import) -->
<xsl:include href="docbook2dbw/author.xsl" />


</xsl:transform>
