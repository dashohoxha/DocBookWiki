<?php
/**
 * Functions for importing an XML/DocBook file into DocBookWiki.
 */

/**
 * Import the given file.
 */
function docbook_import($file) {
  $doc = new DOMDocument;
  $doc->load($file);
  _docbook_import_element($doc->documentElement);
}

function _docbook_import_element($domnode, $bid =NULL, $plid =NULL, $book_alias =NULL) {
  if ($domnode->nodeType == XML_ELEMENT_NODE) {
    if (in_array($domnode->nodeName, array('book', 'article'))) {
      list($bid, $plid, $book_alias) = _docbook_import_node_docbook($domnode);
    }
    elseif (in_array($domnode->nodeName, array('chapter', 'section', 'simplesect'))) {
      $plid = _docbook_import_node_docbook_section($domnode, $bid, $plid, $book_alias);
    }
  }

  if (! $domnode->hasChildNodes())  return;

  foreach ($domnode->childNodes as $child) {
    _docbook_import_element($child, $bid, $plid, $book_alias);
  }
}

function _docbook_import_node_docbook($domnode) {
  $doc1 = _docbook_node2doc($domnode);
  $title = _docbook_apply_stylesheet($doc1, 'get_title.xsl');
  $text  = _docbook_apply_stylesheet($doc1, 'info.xsl');

  $file = drupal_get_path('module', 'dbwiki') . "/converter/class.Info.php";
  include_once $file;
  $info = new Info;
  $info->parse($text);

  $node = new stdClass();
  $node->type = 'docbook';
  node_object_prepare($node);

  $node->title = $title;
  $node->language = LANGUAGE_NONE;
  $node->field_dbw_doctype['und'][0]['value'] = $domnode->nodeName;
  $node->field_dbw_id['und'][0]['value'] = $domnode->getAttribute('id');
  $node->field_dbw_abstract['und'][0]['value'] = $info->abstract;
  $node->field_dbw_copyright['und'][0]['value'] = $info->copyright;
  //$node->field_dbw_release['und'][0]['value'] = $info->release;
  //$node->field_dbw_date['und'][0]['value'] = $info->date;
  /*
  foreach ($info->keywords as $keyword) {
    //$tid = ...
    $node->field_dbw_keywords['und'][] = array('tid' => $tid);
  }
  */
  foreach ($info->authors as $author) {
    $str_author = implode(', ', array_values($author));
    $node->field_dbw_author['und'][] = array('value' => $str_author);
  }

  $node->book['mlid'] = NULL;
  $node->book['bid'] = 'new';
  $book_alias = $domnode->getAttribute('id');
  $node->path = array('alias' => $book_alias);

  node_save($node);
  //print_r($node);  exit(0);  //debug
  return array($node->nid, $node->book['mlid'], $book_alias);
}

function _docbook_import_node_docbook_section($domnode, $bid, $plid, $book_alias) {
  $doc1 = _docbook_node2doc($domnode);
  $title = _docbook_apply_stylesheet($doc1, 'get_title.xsl');
  $text  = _docbook_apply_stylesheet($doc1, 'xml2text.xsl');

  $node = new stdClass();
  $node->type = 'docbook_section';
  node_object_prepare($node);

  $node->title = $title;
  $node->language = LANGUAGE_NONE;
  $node->field_dbw_content['und'][0]['value'] = $text;
  $node->field_dbw_sectype['und'][0]['value'] = $domnode->nodeName;
  $node->field_dbw_id['und'][0]['value'] = $domnode->getAttribute('id');

  $id = $domnode->getAttribute('id');
  if (!empty($id))  {
    $alias = $book_alias . '/' . $id;
    $node->path = array('alias' => $alias);
  }

  $node->book['bid'] = $bid;
  $node->book['plid'] = $plid;
  $node->book['mlid'] = NULL;

  node_save($node);
  //print_r($node);  exit(0);  //debug
  return $node->book['mlid'];
}

/**
 * Convert the given DOMNode to a standalone DOMDocument
 * that contains only this node (under a root element).
 */
function _docbook_node2doc($domnode)
{
  $doc = new DOMDocument;
  $doc->loadXML('<xyz></xyz>');  // we need a root element whatever
  $new_domnode = $doc->importNode($domnode, true);
  $doc->documentElement->appendChild($new_domnode);
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
