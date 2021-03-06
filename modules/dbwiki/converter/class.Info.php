<?php
  /*
   This  file is part  of WikiConverter.   WikiConverter is  a program
   that  converts   text/wiki  into   other  formats  (like   html  or
   xml/docbook).

   Copyright (c) 2005 Dashamir Hoxha, dhoxha@inima.al
   Copyright (c) 2013 Dashamir Hoxha, dashohoxha@gmail.com

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
   * Handle bookinfo or articleinfo elements.
   *
   * @package docbook
   * @subpackage wikiconverter
   */
class Info
{
  var $authors;
  var $abstract;
  var $keywords;
  var $date;
  var $releaseinfo;
  var $copyright;

  function Info()
  {
    $this->authors = array();
    $this->abstract = '';
    $this->keywords = array();
    $this->date = '';
    $this->releaseinfo = '';
    $this->copyright = '';
  }

  /**
   * Extract the various parts of the info section
   * (@author, @abstract, etc.)
   */
  function parse($content)
  {
    $lines = explode("\n", $content);
    $attrib = 'dummy';
    $value  = '';

    for ($i=0; $i < sizeof($lines); $i++)
      {
        $line = $lines[$i];
        if (!preg_match('/^@([^:]+): (.*)$/', $line, $regs))
          {
            $value .= $line;
          }
        else
          {
            //process the previous attribute
            $func_name = "parse_$attrib";
            $this->$func_name($value);

            //initialize the new attribute
            $attrib = $regs[1];
            $value  = $regs[2];
          }
      }
    //process the last attribute
    $func_name = "parse_$attrib";
    $this->$func_name($value);
  }

  function parse_dummy($str) {}

  function parse_author($str)
  {
    $arr = explode(',', $str);
    $this->authors[] = array(
                             'firstname' => trim($arr[1]),
                             'surname' => trim($arr[0]),
                             'email' => trim($arr[2]),
                             'org' => trim($arr[3]),
                             'org_url' => trim($arr[4]),
                             );
  }

  function parse_keywords($str)
  {
    $arr = explode(',', $str);
    for ($i=0; $i < sizeof($arr); $i++)
      {
        $keyword = trim($arr[$i]);
        if ($keyword=='')  continue;
        $this->keywords[] = $keyword;
      }
  }

  function parse_abstract($str)
  {
    $this->abstract = trim($str);
  }

  function parse_date($str)
  {
    $this->date = trim($str);
  }

  function parse_releaseinfo($str)
  {
    $this->releaseinfo = trim($str);
  }

  function parse_copyright($str)
  {
    $this->copyright = trim($str);
  }

  /** convert the info data to xml */
  function to_xml()
  {
    $xml .= $this->render_authors();
    $xml .= $this->render_abstract();
    $xml .= $this->render_keywords();
    $xml .= $this->render_date();
    $xml .= $this->render_releaseinfo();
    $xml .= $this->render_copyright();
    return $xml;
  }

  function render_authors()
  {
    if (sizeof($this->authors)==0)  return '';

    for ($i=0; $i < sizeof($this->authors); $i++)
      {
        extract($this->authors[$i]);

        $xml .= "\n  <author>\n";
        $xml .= "    <firstname>$firstname</firstname>\n";
        $xml .= "    <surname>$surname</surname>\n";
        if (!empty($email)) {
	  $xml .= "    <email>$email</email>\n";
	}
        if (!empty($org))
          {
	    $org = regex_replace($org);
            $xml .= "    <affiliation>\n";
            $xml .= "      <orgname>\n";
	    if (empty($org_url)) {
	      $xml .= "        $org\n";
	    } else {
	      $xml .= "        <ulink url='$org_url'>$org</ulink>\n";
	    }
            $xml .= "      </orgname>\n";
            $xml .= "    </affiliation>\n";
          }
        $xml .= "  </author>\n";
      }

    return $xml;
  }

  function render_abstract()
  {
    $xml = "\n  <abstract><para>"
      . $this->abstract
      . "</para></abstract>\n";

    return $xml;
  }

  function render_keywords()
  {
    if (sizeof($this->keywords)==0)  return '';

    $xml = "\n  <keywordset>\n";
    for ($i=0; $i < sizeof($this->keywords); $i++)
      {
        $keyword = $this->keywords[$i];
        $xml .= "    <keyword>$keyword</keyword>\n";
      }
    $xml .= "  </keywordset>\n";

    return $xml;
  }

  function render_date()
  {
    if (empty($this->date))  return '';

    return "\n  <date>$this->date</date>\n";
  }

  function render_releaseinfo()
  {
    if (empty($this->releaseinfo))  return '';

    return "\n  <releaseinfo>$this->releaseinfo</releaseinfo>\n";
  }

  function render_copyright()
  {
    if ($this->copyright=='')
      {
        $this->copyright = "Copyright (C) 2004, 2005 Firstname Lastname.
Permission is granted to copy, distribute and/or modify this document
under the terms of the GNU Free Documentation License, Version 1.1
or any later version published by the Free Software Foundation;
with no Invariant Sections, with no Front-Cover Texts, and with no
Back-Cover Texts. A copy of the license is included in the section
entitled \"GNU Free Documentation License.\"";
      }

    $xml = "\n  <legalnotice><para>"
      . $this->copyright
      . "</para></legalnotice>\n";

    return $xml;
  }
}
?>