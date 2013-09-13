<?php
/**
 * @file
 * Functions for converting a book from type 'book'
 * to 'docbook', and from 'docbook' to 'book'.
 */

/**
 * Convert the given book to the given book type.
 */
function docbook_change_book_type($book_id, $book_type) {
  //get a list of the nodes that belong to this book
  $params = array(':bid' => $book_id);
  $nids = db_query("SELECT nid FROM {book} WHERE bid=:bid ORDER BY nid", $params)->fetchCol();

  //convert each of them
  foreach ($nids as $nid) {
    if ($book_type=='book') {
      _docbook_convert_to_book($nid);
    }
    elseif ($book_type=='docbook') {
      _docbook_convert_to_docbook($nid);
    }
  }
}

/**
 * Convert the given node from type 'docbook'
 * or 'docbook_section' to type 'book'.
 */
function _docbook_convert_to_book($nid) {
  static $book_id;

  $node = node_load($nid);
  $depth = $node->book['depth'];
  if ($depth == 1) {
    $node->body['und'][0]['value'] = _docbook_convert_get_body_from_info($node);
    $node->body['und'][0]['format'] = 'plain_text';
    $book_id = $node->dbw_id['und'][0]['value'];
    $node->path = array('alias' => $book_id);
  }
  else {
    $txt = $node->dbw_content['und'][0]['value'];
    $node->body['und'][0]['value'] = _filter_dbwiki($txt, NULL);
    $node->body['und'][0]['format'] = 'full_html';
    $sect_id = $node->dbw_id['und'][0]['value'];
    $node->path = array('alias' => "$book_id/$sect_id");
  }
  $node->type = 'book';
  node_save($node);
}

function _docbook_convert_get_body_from_info($node) {
  $body = '';

  if (!empty($node->dbw_abstract['und'][0]['value'])) {
    $body .= '@abstract: ' . $node->dbw_abstract['und'][0]['value'];
    $body .= "\n\n";
  }

  if (!empty($node->dbw_date['und'][0]['value'])) {
    $body .= '@date: ' . $node->dbw_date['und'][0]['value'];
    $body .= "\n\n";
  }

  if (!empty($node->dbw_releaseinfo['und'][0]['value'])) {
    $body .= '@releaseinfo: ' . $node->dbw_releaseinfo['und'][0]['value'];
    $body .= "\n\n";
  }

  if (isset($node->dbw_keywords['und'])) {
    $arr_terms = array();
    $keywords = $node->dbw_keywords['und'];
    foreach ($keywords as $keyword) {
      $term = taxonomy_term_load($keyword['tid']);
      $arr_terms[] = $term->name;
    }
    if (!empty($arr_terms)) {
      $body .= '@keywords: ' . implode(', ', $arr_terms);
      $body .= "\n\n";
    }
  }

  if (isset($node->dbw_author['und'])) {
    $authors = $node->dbw_author['und'];
    foreach ($authors as $author) {
      $body .= '@author: ' . $author['value'];
      $body .= "\n\n";
    }
  }

  if (!empty($node->dbw_copyright['und'][0]['value'])) {
    $body .= '@copyright: ' . $node->dbw_copyright['und'][0]['value'];
    $body .= "\n\n";
  }

  return $body;
}

/**
 * Convert the given node from type 'book'
 * to type 'docbook' or 'docbook_section'.
 */
function _docbook_convert_to_docbook($nid) {
  $node = node_load($nid);

  $depth = $node->book['depth'];
  if ($depth == 1) {
    $node->type = 'docbook';
    $node->dbw_doctype['und'][0]['value'] = 'article';

    $dbw_id = pathauto_cleanstring($node->title);
    pathauto_alias_uniquify($dbw_id, 'node/'.$node->nid, 'und');
    $node->dbw_id['und'][0]['value'] = $dbw_id;

    _docbook_convert_get_info_from_body($node);
  }
  else {
    $node->type = 'docbook_section';
    $node->dbw_sectype['und'][0]['value'] = 'section';

    $dbw_id = pathauto_cleanstring($node->title);
    $node->dbw_id['und'][0]['value'] = $dbw_id;

    $text = _docbook_convert_html2text($node->body['und'][0]['value']);
    $node->dbw_content['und'][0]['value'] = $text;
    $node->dbw_content['und'][0]['format'] = 'docbookwiki';
  }

  node_save($node);
}

function _docbook_convert_get_info_from_body(&$node) {
  $info = new Info;
  $content = $node->body['und'][0]['value'];
  $info->parse($content);

  $node->dbw_abstract['und'][0]['value'] = $info->abstract;
  $node->dbw_date['und'][0]['value'] = $info->date;
  $node->dbw_releaseinfo['und'][0]['value'] = $info->releaseinfo;
  $node->dbw_copyright['und'][0]['value'] = $info->copyright;

  foreach ($info->authors as $author) {
    $str_author = implode(', ', array_values($author));
    $node->dbw_author['und'][] = array('value' => $str_author);
  }

  foreach ($info->keywords as $keyword) {
    $tid =  _docbook_import_get_tid($keyword);
    $node->dbw_keywords['und'][] = array('tid' => $tid);
  }
}

function _docbook_convert_html2text($html) {
  // Set up some configuration options for Tidy
  $config = array(
                  'add-xml-decl' => true,
                  'output-xml' => TRUE,
		  'doctype' => 'omit',
                  'char-encoding' => 'utf8',
                  //'input-encoding' => 'utf8',
                  //'output-encoding' => 'utf8',
                  'enclose-text' => TRUE,
                  'drop-proprietary-attributes' => TRUE,
                  'clean' => TRUE,
                  'logical-emphasis' => TRUE,
                  'preserve-entities' => TRUE,
    );

  $tidy = new Tidy();
  $tidy->parseString($html, $config, 'utf8');
  $tidy->cleanRepair();
  $xhtml = tidy_get_output($tidy);

  include_once drupal_get_path('module', 'docbook') . "/import/transform.inc";
  $dbw = _docbook_xslt($xhtml, 'xhtml2dbw.xsl');
  $text = _docbook_xslt($dbw, 'xml2text.xsl');

  return $text;
}