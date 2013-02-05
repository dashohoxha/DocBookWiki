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
class Tpl
{
  var $id;
  var $type;
  var $contents;

  /** Constructor */
  function Tpl($type ='Template')
  {
    static $default_id = 'Template_001';

    $this->id = $default_id++;
    $this->type = $type;
    $this->contents = array();
  }

  /**
   * If the given line is a reference to another template,
   * returns the id of the referenced template,
   * otherwise return empty string ''.
   */
  function get_tpl_id($line)
  {
    preg_match('/^&&([[:alnum:]_]+);;$/', trim($line), $regs);
    $tpl_id = isset($regs[1]) ? $regs[1] : '';
    return $tpl_id;
  }

  /**
   * Returns an array of ids of the templates that are referenced
   * from the contents of this template.
   */
  function get_subtemplates()
  {
    $arr_tpl_id = array();
    for ($i=0; $i < sizeof($this->contents); $i++)
      {
        $line = $this->contents[$i];
        $tpl_id = $this->get_tpl_id($line);
        if ($tpl_id!='')  $arr_tpl_id[] = $tpl_id;
      }
    return $arr_tpl_id;
  }

  function to_html($indent ='', $class ='')
  {
    global $tpl_collection;

    $html = '';
    $nr = sizeof($this->contents);
    for ($i=0; $i < $nr; $i++)
      {
        $line = $this->contents[$i];
        $tpl_id = $this->get_tpl_id($line);
        if ($tpl_id != '')
          {
            $tpl = $tpl_collection[$tpl_id];
            $html .= $tpl->to_html('  '.$indent, $class);
          }
        else
          {
            $indent1 = ($i==0 ? '' : $indent);
            $line1 = ($i==($nr-1) ? chop($line) : $line);
            $html .= $this->line_to_html($line1, $indent1);
          }
      }
    return $html;
  }

  function line_to_html($line, $indent)
  {
    $line = regex_replace($line);
    return trim($line).' ';
  }

  function to_xml($indent ='')
  {
    global $tpl_collection;

    $xml = '';
    $nr = sizeof($this->contents);
    for ($i=0; $i < $nr; $i++)
      {
        $line = $this->contents[$i];
        $tpl_id = $this->get_tpl_id($line);
        if ($tpl_id != '')
          {
            $tpl = $tpl_collection[$tpl_id];
            $xml .= $tpl->to_xml('  '.$indent);
          }
        else
          {
            $indent1 = ($i==0 ? '' : $indent);
            $line1 = ($i==($nr-1) ? chop($line) : $line);
            $xml .= $this->line_to_xml($line1, $indent1);
          }
      }

    return $xml;
  }

  function line_to_xml($line, $indent)
  {
    $line = regex_replace($line);
    return trim($line).' ';
  }

  /** Debug function. */
  function to_html_table()
  {
    $html_table = "
<br />
<a name='$this->id' id='$this->id'><span class='h_space'/></a>[<a href='javascript: back()'>Back</a>]
<table class='converter' width='90%' bgcolor='#aaaaaa' border='0' cellspacing='1' cellpadding='2'>
  <tr>
    <td bgcolor='#eeeeee'>
      $this->id, type=$this->type
    </td>
  </tr>
  <tr><td bgcolor='#f9f9ff'>
<pre>";
    for ($i=0; $i < sizeof($this->contents); $i++)
      {
        $line = $this->contents[$i];
        $tpl_id = $this->get_tpl_id($line);
        if ($tpl_id != '')
          {
            $html_table .= "<a href='#$tpl_id'>$line</a>";
          }
        else
          {
            $html_table .= htmlspecialchars($line);
          }
      }

    $html_table .= "</pre>
  </td></tr>
</table>
";
    return $html_table;
  }
}
?>