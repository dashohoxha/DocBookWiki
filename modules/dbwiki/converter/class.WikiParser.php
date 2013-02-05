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
class WikiParser
{
  /** lines that will be parsed and their index */
  var $arr_lines;
  var $idx;

  function WikiParser()
  {
    $this->arr_lines = array();
    $this->idx = -1;

    global $tpl_collection;
    $tpl_collection = array();
  }

  function parse_file($filename)
  {
    if ( !file_exists($filename) )
      {
        print "'$filename' not found\n";
        return UNDEFINED;
      }

    $arr_lines = file($filename);
    $tpl = $this->parse_lines($arr_lines);
    return $tpl;
  }

  function parse_string($str)
  {
    $arr_lines = explode("\n", $str);
    for ($i=0; $i < sizeof($arr_lines); $i++)
      {
        $arr_lines[$i] .= "\n";
      }

    $tpl = $this->parse_lines($arr_lines);
    return $tpl;
  }

  function parse_lines($arr_lines)
  {
    $this->arr_lines = $arr_lines;
    $this->idx = 0;
    $tpl = $this->parse_text();

    global $tpl_collection;
    $tpl_collection['Text'] = $tpl;

    return $tpl;
  }

  /** parse the text wiki */
  function parse_text()
  {
    $text = new Text;
    while (!$this->end_of_lines())
      {
        if ($this->block_start())
          {
            $block = $this->parse_block();
            $this->link_template($text, $block);
          }
        else if ($this->list_start())
          {
            $list = $this->parse_list();
            $this->link_template($text, $list);
          }
        else if ($this->nonempty_line())
          {
            $paragraph = $this->parse_paragraph();
            $this->link_template($text, $paragraph);
          }
        else //line is empty
          {
            $this->next_line(); //skip it
          }
      }
    return $text;
  }

  function parse_paragraph()
  {
    $paragraph = new Paragraph;
    while (!$this->end_of_lines())
      {
        if ($this->block_start())
          {
            $block = $this->parse_block();
            $this->link_template($paragraph, $block);
          }
        else if ($this->list_start())
          {
            $list = $this->parse_list();
            $this->link_template($paragraph, $list);
          }
        else if ($this->nonempty_line())
          {
            $this->append_line($paragraph);
            $this->next_line();  //get the next line
          }
        else //empty-line
          {
            //end of this paragraph, stop parsing
            break;
          }
      }
    return $paragraph;
  }

  function parse_block()
  {
    //get the block type
    $line = $this->get_current_line();
    preg_match('/^--([^ ]+)(.*)$/', trim($line), $regs);
    $type = $regs[1];
    $title = trim($regs[2]);

    $block = new Block($type, $title);

    $this->next_line();  //skip the block start line
    while (!$this->end_of_lines())
      {
        if ($this->block_end())
          {
            $this->next_line();  //skip the block end line
            break; //stop parsing this block
          }
        else if ($this->block_start())
          {
            $blk = $this->parse_block();
            $this->link_template($block, $blk);
          }
        else if ($this->list_start())
          {
            $list = $this->parse_list();
            $this->link_template($block, $list);
          }
        else
          {
            $this->append_line($block);
            $this->next_line();  //get the next line
          }
      }
    return $block;
  }

  function parse_info()
  {
    //get the block type
    $line = $this->get_current_line();
    preg_match('/^--([^ ]+)(.*)$/', trim($line), $regs);
    $type = $regs[1];
    $title = trim($regs[2]);

    if ($type=='info')
      $block = new InfoBlock();
    else
      $block = new Block($type, $title);

    $this->next_line();  //skip the block start line
    while (!$this->end_of_lines())
      {
        if ($this->block_end())
          {
            $this->next_line();  //skip the block end line
            break; //stop parsing this block
          }
        else if ($this->block_start())
          {
            $blk = $this->parse_block();
            $this->link_template($block, $blk);
          }
        else if ($this->list_start())
          {
            $list = $this->parse_list();
            $this->link_template($block, $list);
          }
        else
          {
            $this->append_line($block);
            $this->next_line();  //get the next line
          }
      }
    return $block;
  }

  function parse_list()
  {
    $mark = $this->get_listitem_mark();
    $indent = $this->get_indentation($mark);
    $bullet = $this->get_bullet($mark);
    $list = new Listing($indent, $bullet);
    while (!$this->end_of_lines())
      {
        if ($this->list_end($mark))
          {
            break;  //stop parsing this list
          }
        else if ($this->listitem_start($mark))
          {
            $item = $this->parse_listitem($mark);
            $this->link_template($list, $item);
          }
      }
    return $list;
  }

  function parse_listitem($mark)
  {
    $listitem = new ListItem;
    $line = $this->get_current_line();
    $line = substr($line, strlen($mark));
    $this->append_line($listitem, $line);
    $this->next_line();  //get the next line

    while (!$this->end_of_lines())
      {
        if ($this->listitem_end($mark))
          {
            break;  //end of this listitem
          }
        else if ($this->block_start())
          {
            $block = $this->parse_block();
            $this->link_template($listitem, $block);
          }
        else if ($this->list_start($mark))
          {
            $list = $this->parse_list();
            $this->link_template($listitem, $list);
          }
        else if ($this->empty_line())
          {
            //skip empty lines
            $this->next_line();
          }
        else
          {
            $this->append_line($listitem);
            $this->next_line();  //get the next line
          }
      }
    return $listitem;
  }

  /** get the current line that is being parsed */
  function get_current_line()
  {
    $line = $this->arr_lines[$this->idx];
    return $line;
  }

  /** return true if the index is at the end of the lines */
  function end_of_lines()
  {
    $eol = ($this->idx >= sizeof($this->arr_lines));
    return $eol;
  }

  /** move the index to the nex line */
  function next_line()
  {
    $this->idx++;
  }

  /** append the current line to the given template */
  function append_line(&$tpl, $line ='UNDEFINED')
  {
    if ($line=='UNDEFINED')  $line = $this->get_current_line();
    $tpl->contents[] = $line;
  }

  /** add a link from the parent template to the child template */
  function link_template(&$parent, $child)
  {
    $parent->contents[] = '&&'.$child->id.";;\n";

    //also add the child template in the template collection
    global $tpl_collection;
    $tpl_collection[$child->id] = $child;
  }

  function empty_line()
  {
    $line = $this->get_current_line();
    return (trim($line) == '');
  }

  function nonempty_line()
  {
    $line = $this->get_current_line();
    return (trim($line) != '');
  }

  function block_start()
  {
    $line = $this->get_current_line();
    $line = trim($line);
    $pattern = '/^--(info|code|scr|screen|ll|fig|figure|xmp|example'
      . '|n|note|w|warning|c|caution|tip|imp|important)/';
    $b_start = preg_match($pattern, $line);
    return $b_start;
  }

  function block_end()
  {
    $line = $this->get_current_line();
    return (trim($line)=='----');
  }

  function get_listitem_mark()
  {
    $line = $this->get_current_line();
    preg_match('/^( *(\*|1\.|a\.|i\.|A\.|I\.))/', $line, $regs);
    $mark = isset($regs[1]) ? $regs[1] : '';
    return $mark;
  }

  /** return the indentaion of the given mark */
  function get_indentation($mark)
  {
    preg_match('/^( *)/', $mark, $regs);
    return $regs[1];
  }

  /** return the bullet of the given mark */
  function get_bullet($mark)
  {
    preg_match('/([^ ]*)$/', $mark, $regs);
    return $regs[1];
  }

  /** return true if the current line starts a new list */
  function list_start($mark ='')
  {
    $new_mark = $this->get_listitem_mark();
    if ($new_mark=='')
      {
        //current line does not contain a listitem mark
        return false;
      }

    if ($mark=='')  return true;

    if ($new_mark==$mark)
      {
        //it is another item in the same list, not a new list
        return false;
      }

    $indent = $this->get_indentation($mark);
    $new_indent = $this->get_indentation($new_mark);
    if (strlen($new_indent) > strlen($indent))
      {
        //it is more indented than the current list,
        //so it must be a new list
        return true;
      }

    return false;
  }

  function list_end($mark)
  {
    $line = $this->get_current_line();
    if (trim($line)=='/')
      {
        //end of list marker
        $this->next_line();
        return true;
      }

    if (trim($line)=='----')
      {
        //end of block marker
        return true;
      }

    $new_mark = $this->get_listitem_mark();
    if ($new_mark=='')
      {
        //current line does not contain a listitem mark
        return false;
      }

    if ($new_mark==$mark)
      {
        //it is another item in the same list, not the end of the list
        return false;
      }

    $indent = $this->get_indentation($mark);
    $new_indent = $this->get_indentation($new_mark);
    if (strlen($new_indent) <= strlen($indent))
      {
        //it is less indented than the current list
        //or it has a different bullet
        //so it must belong to another list, which
        //denotes the end of the current list
        return true;
      }

    return false;
  }

  function listitem_start($mark)
  {
    $new_mark = $this->get_listitem_mark();
    if ($new_mark=='')
      {
        //current line does not contain a listitem mark
        return false;
      }
    else
      {
        //current line contains a list item mark
        return true;
      }
  }

  function listitem_end($mark)
  {
    $line = $this->get_current_line();
    if (trim($line)=='/')
      {
        //end of list marker also marks the end of the last listitem
        return true;
      }

    if (trim($line)=='----')
      {
        //end of block marker
        return true;
      }

    $new_mark = $this->get_listitem_mark();
    if ($new_mark=='')
      {
        //current line does not contain a listitem mark
        return false;
      }
    else if ($new_mark==$mark)
      {
        //it is a new item in the same list
        return true;
      }
    else
      {
        $indent = $this->get_indentation($mark);
        $new_indent = $this->get_indentation($new_mark);
        if (strlen($new_indent) > strlen($indent))
          {
            //it is the first item of a nested list
            //listitem has not ended yet
            return false;
          }
        else
          {
            //it is an item of another list
            return true;
          }
      }
  }


  /*========================= DEBUG ==================================*/

  /** (debug) Outputs the data of each template in $tpl_collection. */
  function template_list()
  {
    global $tpl_collection;

    $tpl_index = "<strong>List of Parsed Templates:</strong>\n";
    $tpl_index .= "<ul>\n";
    reset($tpl_collection);
    while ( list($tpl_id, $tpl) = each($tpl_collection) )
      {
        $tpl_index .= "\t<li><a href='#$tpl->id'>$tpl->id</a></li>\n";
        $tpl_list .= $tpl->render_to_html_table();
      }
    $tpl_index .= "</ul>\n";

    $html = "<a name='top' id='top'></a>\n";
    $html .= "<div class='converter'>\n";
    $html .= $tpl_index;
    $html .= $tpl_list;
    $html .= "</div>\n";
    $html .= "<hr />\n";

    return $html;
  }

  /** (debug) Returns the structure of the loaded templates as a tree. */
  function tpl_to_tree()  //for debugging parse()
  {
    global $tpl_collection;
    $tpl = $tpl_collection['Text'];
    $tree = "<hr />\n";
    $tree .= "<a name='top' id='top'></a>\n";
    $tree .= "<pre class='converter'>\n";
    $tree .= "<strong>The tree structure of the templates:</strong>\n\n";
    $tree .= $this->to_tree($tpl, '');
    $tree .= "</pre>\n";
    return $tree;
  }

  /** Returns the structure of loaded templates as a tree. */
  function to_tree($tpl, $indent)
  {
    global $tpl_collection;

    $tree = $indent."|\n";
    $tree .= $indent."+--<a href='#".$tpl->id."'>".$tpl->id."</a>"
      . " (".$tpl->type.")\n";

    $arr_tpl_id = $tpl->get_subtemplates();
    for ($i=0; $i < sizeof($arr_tpl_id); $i++)
      {
        $tpl_id = $arr_tpl_id[$i];
        $tpl = $tpl_collection[$tpl_id];
        $tree .= $this->to_tree($tpl, $indent."|  ");
      }
    return $tree;
  }
}
?>
