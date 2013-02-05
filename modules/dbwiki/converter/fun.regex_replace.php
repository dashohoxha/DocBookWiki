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
   * Replace wiki inline markups with docbook markups,
   * using regular expressions.
   *
   * @package docbook
   * @subpackage wikiconverter
   *
   * Inline marks are denoted by special characters and are contained
   * inside a single line (cannot span multiple lines). They are:
   *
   *   * Links: [link_name > href]
   *     [...] should be escaped like this: \[...]
   *
   *   * Cross referencies: [>sect_id]
   *
   *   * Bibliography referencies: [->citekey]
   *
   *   * Footnotes: [/...]
   *
   *   * Menu items: [O_ptions->G_eneral->S_ave (Ctrl-S)]
   *
   *   * Images: [< filename < width < alt ]
   *
   *   * Square brackets ([...]) can be escaped like this: (\[...]).
   *
   *   * Filename: ~filename~, emphasis: _word_, command: `cmd`
   *
   *   * Special chars can be escaped by a slash, like this:
   *     \~, \_, \`, \<, \>
   *
   *   * Prompt and command are separated by '!#' in a single line:
   *     prompt$!# command
   *     It is escaped like this: \!#
   *
   *   * As a special case, the lines that start with 'bash$' or 'bash#'
   *     are treated as command lines, so there is no need to use '!#'.
   *
   */
function regex_replace($line)
{
  $patterns = 
    array(
          // [->citekey]
          "#(?<!\\\\|\[)\[->([^\]]+)\]#",

          // [menu->item->subitem (Ctrl-C)]
          "#(?<!\\\\|\[)\[(([^\]]*->[^\]]*)?\s*(\(.*\))?)\s*\]#e",

          // [>xref]
          "#(?<!\\\\|\[)\[>([^\]]+)\]#",

          // [<imagename<width<alt]
          "#(?<!\\\\|\[)\[<([^\]<]*)(<([^\<]*))?(<([^\]]*))?\]#e",

          // [label>url]
          "#(?<!\\\\|\[)\[([^\]]+)>([^\]]*)\]#",

          // [/footnote]
          "#(?<!\\\\|\[)\[/([^\]]+)\]#",

          // [url]
          "#(?<!\\\\|\[)\[([^\]]+)\]#",

          // `command`
          "#(?<!\\\\)`(.+?)(?<!\\\\)`#",

          // ~filename~
          "#(?<!\\\\)~(.+?)(?<!\\\\)~#",

          // _emphasized_
          "#(?<!\\\\)_(.+?)(?<!\\\\)_#",

          /* prompt!# command */
          "/^(\s*)(.*?)\s*(?<!\\\\)!#\s*(.*?)(\s*)\$/",

          /* bash$ cmd   or bash# cmd */
          "/^(\s*)(?<!\\\\)(bash\\\$|bash#)\s+(.*?)(\s*)\$/",

          // \[escaped]
          "#\\\\\[([^\]]+)\]#",

          /* \< -> &lt; */
          "/\\\\</",

          /* \> -> &gt; */
          "/\\\\>/",

          /* \!# -> !# */
          "/\\\\!#/",

          /* '\bash$' and '\bash#' at the begining of a line */
          "/^(\s*)\\\\(bash\\\$|bash#)/",

          /* remove the slash (\) before the escaped chars */
          "/\\\\(_|`|~)/",
          );

  $replacements = 
    array(
          // [-> (citekey)]
          '<citation><xref linkend="\\1"/></citation>',

          // [menu->item->subitem]
          "convert_menu_items('\\1')",

          // [> (xref)]
          '<xref linkend="\\1"/>',

          // [< (imagename) (< (width))? (< (alt))?]
          "mediaobject('\\1', '\\3', '\\5')",

          // [(label)>(url)]
          '<ulink url="\\2">\\1</ulink>',

          // [/(footnote)]
          '<footnote><para>\\1</para></footnote>',

          // [(url)]
          '<ulink url="\\1"/>',

          // `(command)`
          '<command>\\1</command>',

          // ~(filename)~
          '<filename>\\1</filename>',

          // _(emphasized)_
          '<emphasis>\\1</emphasis>',

          /* prompt!# command */
          '\\1<prompt>\\2</prompt> <command>\\3</command>\\4',

          /* bash$ cmd   or bash# cmd */
          '\\1<prompt>\\2</prompt> <command>\\3</command>\\4',

          // \[(escaped)]
          '[\\1]',

          /* \< -> &amp;lt; */
          '&amp;lt;',

          /* \> -> &amp;gt; */
          '&amp;gt;',

          /* \!# -> !# */
          '!#',

          /* '\bash$' and '\bash#' at the begining of a line */
          '\\1\\2',

          /* remove the slash (\) before the escaped chars */
          '\\1',
          );

  $line = preg_replace($patterns, $replacements, $line);
  return $line;
}

/**
 * This function gets a string like this: 'M_enu->S_ubmenu->MenuI_tem (Ctrl-C)'
 * and returns a docbook code like this:
 *
 * <menuchoice>
 *   <shortcut>
 *     <keycombo><keycap>Ctrl</keycap><keycap>C</keycap></keycombo>
 *   </shortcut>
 *   <guimenu><accel>M</accel>enu</guimenu>
 *   <guisubmenu><accel>S</accel>ubmenu</guisubmenu>
 *   <guimenuitem>Menu<accel>I</accel>tem</guimenuitem>
 * </menuchoice>.
 *
 */
function convert_menu_items($str)
{
  $pos = strpos($str, '(');
  if ($pos===false)
    {
      $menu_path = $str;
      $shortcut = '';
    }
  else
    {
      $menu_path = substr($str, 0, $pos);
      $shortcut = substr($str, $pos + 1);

      $pos = strpos($shortcut, ')');
      if ($pos)  $shortcut = substr($shortcut, 0, $pos);
    }
  $menu_path = trim($menu_path);
  $shortcut = trim($shortcut);

  $arr_menu = explode('->', $menu_path);
  $arr_keys = explode('-', $shortcut);

  //count the number of nonempty menuitems
  for ($i=0; $i < sizeof($arr_menu); $i++)
    {
      $item = trim($arr_menu[$i]);
      if ($item!='') $item_cnt++;
    }

  //decide whether to output a <menuchoice> element
  if ($menu_path=='')  $mchoice = false;
  else if ($item_cnt==1 and $shortcut=='')  $mchoice = false;
  else $mchoice = true;

  $xml = '';
  if ($mchoice)  $xml .= '<menuchoice>';
  if ($shortcut!='')
    {
      if ($mchoice)  $xml .= '<shortcut>';
      $xml .= '<keycombo>';
      for ($i=0; $i < sizeof($arr_keys); $i++)
        {
          $key = trim($arr_keys[$i]);
          $xml .= '<keycap>'.$key.'</keycap>';          
        }
      $xml .= '</keycombo>';
      if ($mchoice)  $xml .= '</shortcut>';
    }
  if ($menu_path!='')
    {
      for ($i=0; $i < sizeof($arr_menu); $i++)
        {
          $item = $arr_menu[$i];
          $item = preg_replace('/(.)_/', '<accel>\\1</accel>', $item);
          $item = trim($item);
          if ($item=='')  continue;

          if ($i==0)
            $xml .= '<guimenu>'.$item.'</guimenu>';
          else if ($i==(sizeof($arr_menu)-1))
            $xml .= '<guimenuitem>'.$item.'</guimenuitem>';
          else
            $xml .= '<guisubmenu>'.$item.'</guisubmenu>';
        }
    }
  if ($mchoice)  $xml .= '</menuchoice>';

  return $xml;
}

/** Returns a <mediobject> element */
function mediaobject($filename, $width ='', $alt ='')
{
  //print "'$filename' '$width' '$alt'"; //debug
  $widthparam = ($width=='' ? '' : "width=\"$width\"");
  $str = "
<mediaobject>
  <imageobject><imagedata fileref=\"$filename\" $widthparam align=\"center\"/></imageobject>
  <textobject><phrase>$alt</phrase></textobject>
</mediaobject>
";
  return $str;
}
?>