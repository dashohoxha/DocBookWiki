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
   * Handle <bibliography> elements (convert from text to XML).
   *
   * @package docbook
   * @subpackage wikiconverter
   */
class Bibliography
{
  /** array of citation keys */
  var $citekeys;

  function Bibliography()
  {
    $this->citekeys = array();
  }

  /**
   * Each line of the content is in the format
   * CK: TY, Y1, A1, T1 (citekey: type, year, author, title)
   * We parse and get only the CK and store it the array of the citekeys.
   * We are not interested in the rest of the line, they are just for
   * the benefit of the editor (syntactic sugar).
   */
  function parse($content)
  {
    $lines = explode("\n", $content);

    for ($i=0; $i < sizeof($lines); $i++)
      {
        $line = $lines[$i];
        list($citekey, $rest) = split(':', $line, 2);
        if (trim($citekey)=='')  continue;
	$this->citekeys[] = trim($citekey);
      }
  }

  /**
   * Get from RefDB the referencies whose citekeys are
   * in the array $this->citekeys. Convert the output
   * to XML that is suitable for the element <bibliography>.
   */
  function to_xml()
  {
    //get a search string for selecting all the referencies
    //in $this->citekeys and write it in a temporary file
    $arr_select = array();
    for ($i=0; $i < sizeof($this->citekeys); $i++)
      {
	$citekey = $this->citekeys[$i];
	$arr_select[] = ":CK:=$citekey"; 
      }
    $str_select = implode(' OR ', $arr_select);
    if ($str_select=='')  return '';
   
    $searchline = write_tmp_file($str_select);

    //get the referencies from RefDB in XML/DocBook format
    $getrefs_sh = SCRIPTS.'refdb/getrefs.sh';
    $xml = shell("$getrefs_sh $searchline");
    if ($xml=='')  $xml = '<biblioentry><title/></biblioentry>';

    //remove the temporary file
    unlink($searchline);

    return $xml;
  }
}
?>