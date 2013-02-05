<?php
  /*
   This  file is part  of WikiConverter.   WikiConverter is  a program
   that  converts   text/wiki  into   other  formats  (like   html  or
   xml/docbook).

   Copyright (c) 2005 Dashamir Hoxha, dhoxha@inima.al

   WikiConverter  is free  software;  you can  redistribute it  and/or
   modify  it under the  terms of  the GNU  General Public  License as
   published by the Free Software  Foundation; either version 2 of the
   License, or (at your option) any later version.

   WikiConverter is  distributed in the  hope that it will  be useful,
   but  WITHOUT ANY  WARRANTY; without  even the  implied  warranty of
   MERCHANTABILITY or  FITNESS FOR A PARTICULAR PURPOSE.   See the GNU
   General Public License for more details.

   You should have  received a copy of the  GNU General Public License
   along  with  WikiConverter; if  not,  write  to  the Free  Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
   USA
  */

  /**
   * @package docbook
   * @subpackage wikiconverter
   */
class Paragraph extends Tpl
{
  function Paragraph()
  {
    static $id = 'Paragraph_01';

    Tpl::Tpl('Paragraph');
    $this->id = $id++;
  }

  function to_html($indent ='', $class ='')
  {
    $html = ("\n$indent<p $class>"
             . Tpl::to_html($indent, $class)
             . "</p>\n");
    return $html;
  }

  function to_xml($indent ='')
  {
    $pattern1 = '#^(\\s*<comment>\\d+</comment>\\s*)+$#';
    $contents = implode("\n", $this->contents);
    if (preg_match($pattern1, $contents))
      {
        //don't enclose in <para> tags
        $xml = "\n$indent" . Tpl::to_xml($indent) . "\n";
      }
    else
      {
        $xml = ("\n$indent<para>"
                . Tpl::to_xml($indent)
                . "</para>\n");
      }

    return $xml;
  }
}
?>