<?php
/**
 * Functions for importing an XML/DocBook file into DocBookWiki.
 */

/**
 * Import the given file.
 */
function docbook_import($file)
{
  $doc = new DOMDocument;
  $doc->load($file);
  _docbook_import_doc($doc);
}

function _docbook_import_doc($doc)
{
  _docbook_import_children($doc);
}

function _docbook_import_children($node)
{
  if (! $node->hasChildNodes())  return;

  foreach ($node->childNodes as $child) {
    _docbook_import_node($child);
  }
}

function _docbook_import_node($node)
{
  $arr_elements = array('book', 'article', 'chapter', 'section', 'simplesect');
  if ($node->nodeType == XML_ELEMENT_NODE
      and in_array($node->nodeName, $arr_elements)) {
    $id = $node->getAttribute('id');
    $name = $node->nodeName;

    $doc1 = _docbook_node2doc($node);
    $title = _docbook_apply_stylesheet($doc1, 'get_title.xsl');
    $text  = _docbook_apply_stylesheet($doc1, 'xml2text.xsl');

    print "\n========> $name $id \n";
    print 'Title: ' . $title . "\n";
    print $text;
  }

  // process recursively all the children
  _docbook_import_children($node);
}

/**
 * Convert the given DOMNode to a standalone DOMDocument
 * that contains only this node (under a root element).
 */
function _docbook_node2doc($node)
{
  $doc = new DOMDocument;
  $doc->loadXML('<xyz></xyz>');  // we need a root element whatever
  $new_node = $doc->importNode($node, true);
  $doc->documentElement->appendChild($new_node);
  return $doc;
}

/**
 * Apply the given XSL to the given DOMDocument
 * and return the result as a string.
 */
function _docbook_apply_stylesheet($dom_document, $xsl)
{
  static $proc;
  if (!isset($proc)) $proc = new XSLTProcessor;
  $path = dirname(__FILE__) . '/xslt';
  $xsl_file = $path . '/' . $xsl;
  $proc->importStyleSheet(new SimpleXMLElement($xsl_file, NULL, TRUE));
  return $proc->transformToXML($dom_document);
}
