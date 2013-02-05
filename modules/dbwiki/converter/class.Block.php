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
class Block extends Tpl
{
  var $block_type;
  var $block_title;

  function Block($type ='code', $title ='')
  {
    static $id = 'Block_01';

    Tpl::Tpl('Block');
    $this->id = $id++;
    $this->block_type = $type;
    $this->block_title = $title;
  }

  function to_html($indent ='', $class ='')
  {
    $tag = ($this->block_type=='code' ? 'xmp' : 'pre');
    $html = ("\n$indent<$tag $class>"
             . Tpl::to_html('', $class)
             . "</$tag>\n");
    return $html;
  }

  function line_to_html($line, $indent)
  {
    //without changing the whitespace, no trim no indentation
    $line = regex_replace($line);
    return $line;
  }

  function to_xml($indent ='')
  {
    $type = $this->block_type;
    switch ($type)
      {
      case 'code':
        $tag = 'programlisting';
        break;
      case 'scr':
      case 'screen':
        $tag = 'screen';
        break;
      case 'll':
        $tag = 'literallayout';
        break;
      case 'fig':
      case 'figure':
        $tag = 'figure';
        break;
      case 'xmp':
      case 'example':
        $tag = 'example';
        break;
      case 'n':
      case 'note':
        $tag = 'note';
        break;
      case 'w':
      case 'warning':
        $tag = 'warning';
        break;
      case 'c':
      case 'caution':
        $tag = 'caution';
        break;
      case 'imp':
      case 'important':
        $tag = 'important';
        break;
      case 'tip':
        $tag = 'tip';
        break;
      }

    if ($tag=='programlisting' or $tag=='screen' or $tag=='literallayout')
      {
        $xml = ("\n$indent<$tag>"
                . Tpl::to_xml()
                . "</$tag>\n");
      }
    else if ($tag=='figure' or $tag=='example')
      {
        $title = $this->block_title;
        $xml = ("\n$indent<$tag><title>$title</title>"
                . Tpl::to_xml()
                . "$indent</$tag>\n");
      }
    else //admonitions
      {
        $title = $this->block_title;
        if ($title!='')  $title = '<title>'.$title.'</title>';
        $xml = "\n$indent<$tag>$title<para>"
          . Tpl::to_xml()
          . "</para></$tag>\n";
      }

    return $xml;
  }

  function line_to_xml($line, $indent)
  {
    $line = regex_replace($line);
    return $line;
  }
}
?>