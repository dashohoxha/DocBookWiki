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
class Listing extends Tpl
{
  var $indent;
  var $bullet;

  function Listing($indent ='', $bullet ='*')
  {
    static $id = 'Listing_01';

    Tpl::Tpl('Listing');
    $this->id = $id++;
    $this->indent = $indent;
    $this->bullet = $bullet;
  }

  function to_html($indent ='', $class ='')
  {
    //get the tag and the type of the list
    $b = $this->bullet;
    if ($b=='*')
      {
        $tag = 'ul';
        $type = '';
      }
    else
      {
        $tag = 'ol';
        $t = substr($b, 0, 1);
        $type = "type='$t'";
      }

    $html = ("\n$indent<$tag $type $class>"
             . Tpl::to_html($indent, $class)
             . "$indent</$tag>\n");
    return $html;
  }

  function to_xml($indent ='')
  {
    //get the tag and the type of the list
    $b = $this->bullet;
    if ($b=='*')
      {
        $tag = 'itemizedlist';
        $type = '';
      }
    else
      {
        $tag = 'orderedlist';
        switch ($b)
          {
	  default:
          case '1.':
            $numeration = 'numeration="arabic"';
            break;
          case 'a.':
            $numeration = 'numeration="loweralpha"';
            break;
          case 'i.':
            $numeration = 'numeration="lowerroman"';
            break;
          case 'A.':
            $numeration = 'numeration="upperalpha"';
            break;
          case 'I.':
            $numeration = 'numeration="upperroman"';
            break;
          }
      }

    $html = ("\n$indent<$tag $numeration>"
             . Tpl::to_xml($indent)
             . "$indent</$tag>\n");
    return $html;
  }
}
?>