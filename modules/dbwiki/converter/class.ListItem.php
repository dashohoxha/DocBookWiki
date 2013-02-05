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
class ListItem extends Tpl
{
  function ListItem()
  {
    static $id = 'ListItem_01';

    Tpl::Tpl('ListItem');
    $this->id = $id++;
  }

  function to_html($indent ='', $class ='')
  {
    $arr_subtpl = $this->get_subtemplates();
    $html = "\n$indent<li $class>" . Tpl::to_html($indent);
    if (sizeof($arr_subtpl)!=0)  $html .= $indent;
    $html .= "</li>\n";
    return $html;
  }

  function to_xml($indent ='')
  {
    $arr_subtpl = $this->get_subtemplates();
    $html = "\n$indent<listitem><para>" . Tpl::to_xml($indent);
    if (sizeof($arr_subtpl)!=0)  $html .= $indent;
    $html .= "</para></listitem>\n";
    return $html;
  }
}
?>