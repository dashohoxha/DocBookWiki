<?php
/**
 * Functions for transforming a file to a format
 * suitable for being imported to DocBookWiki.
 */

/**
 * Transform the given docbook file.
 */
function docbook_docbook2dbw($file)
{
  $docbook = file_get_contents($file);
  $dbw = _docbook_xslt($docbook, 'docbook2dbw.xsl');
  print $dbw;    // output to stdout
}

/**
 * Transform the given XML string with the given
 * XSL file and return the result as a string.
 */
function _docbook_xslt($xml, $xsl)
{
  $xml = _docbook_xslt_preprocess($xml);

  $dom = new DOMDocument;
  $dom->loadXML($xml);

  $proc = new XSLTProcessor;
  $path = dirname(__FILE__) . '/xslt';
  $xsl_file = $path . '/' . $xsl;
  $proc->importStyleSheet(new SimpleXMLElement($xsl_file, NULL, TRUE));

  $xml = $proc->transformToXML($dom);

  $xml = _docbook_xslt_postprocess($xml);

  return $xml;
}

/**
 * Replace <![CDATA[...]]> by <cdata>x</cdata> and
 * replace <!--...--> by <comment>x</comment>.
 * Return the rezulting xml (as a string).
 */
function _docbook_xslt_preprocess($xml)
{
  global $arr_cdata, $arr_comments;

  //replace <![CDATA[...]]> by <cdata>x</cdata>
  preg_match_all('#<!\[CDATA\[.*?]]>#s', $xml, $matches);
  $arr_cdata = $matches[0];
  for ($i=0; $i < sizeof($arr_cdata); $i++) {
    $xml = str_replace($arr_cdata[$i], "<cdata>$i</cdata>", $xml);
  }

  //replace <!--...--> by <comment>x</comment>
  preg_match_all('#<!--.*?-->#s', $xml, $matches);
  $arr_comments = $matches[0];
  for ($i=0; $i < sizeof($arr_comments); $i++) {
    $xml = str_replace($arr_comments[$i], "<comment>$i</comment>", $xml);
  }

  return $xml;
}

/**
 * Replace <cdata>x</cdata> by <![CDATA[...]]> and
 * replace <comment>x</comment> by <!--...-->.
 * Return the rezulting xml (as a string).
 */
function _docbook_xslt_postprocess($xml)
{
  global $arr_cdata, $arr_comments;

   // replace <cdata>x</cdata> by the corresponding <![CDATA[...]]>
  preg_match_all('#<cdata>(\\d+)</cdata>#', $xml, $matches);
  $arr_numbers = $matches[1];
  for ($i=0; $i < sizeof($arr_numbers); $i++) {
    $nr = $arr_numbers[$i];
    $cdata = $arr_cdata[$nr];
    $xml = str_replace("<cdata>$nr</cdata>", $cdata, $xml);
  }

  // replace <comment>x</comment> by the corresponding <!--...-->
  preg_match_all('#<comment>(\\d+)</comment>#', $xml, $matches);
  $arr_numbers = $matches[1];
  for ($i=0; $i < sizeof($arr_numbers); $i++) {
    $nr = $arr_numbers[$i];
    $comment = $arr_comments[$nr];
    $xml = str_replace("<comment>$nr</comment>", $comment, $xml);
  }

  return $xml;
}
