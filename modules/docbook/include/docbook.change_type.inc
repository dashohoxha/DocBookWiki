<?php
/**
 * @file
 * Fuctions for converting a book from type 'book'
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
    $node->body = $node->dbw_content;
    //TODO: convert dbw content to html
    //$node->body['und'][0]['value'] = dbw2html($node->dbw_content['und'][0]['value']);
    //$node->body['und'][0]['format'] = 'full_html';

    $sect_id = $node->dbw_id['und'][0]['value'];
    $node->path = array('alias' => "$book_id/$sect_id");
  }
  $node->type = 'book';
  node_save($node);
}

function _docbook_convert_get_body_from_info($node) {
  $body = '';

  if (!empty($node->dbw_abstract['und'][0]['value'])) {
    $body .= "\n@abstract: " . $node->dbw_abstract['und'][0]['value'];
  }

  if (!empty($node->dbw_date['und'][0]['value'])) {
    $body .= "\n@date: " . $node->dbw_date['und'][0]['value'];
  }

  if (!empty($node->dbw_releaseinfo['und'][0]['value'])) {
    $body .= "\n@releaseinfo: " . $node->dbw_releaseinfo['und'][0]['value'];
  }

  if (isset($node->dbw_keywords['und'])) {
    $arr_terms = array();
    $keywords = $node->dbw_keywords['und'];
    foreach ($keywords as $keyword) {
      $term = taxonomy_term_load($keyword['tid']);
      $arr_terms[] = $term->name;
    }
    if (!empty($arr_terms)) {
      $body .= "\n@keywords: " . implode(', ', $arr_terms);
    }
  }

  if (isset($node->dbw_author['und'])) {
    $authors = $node->dbw_author['und'];
    foreach ($authors as $author) {
      $body .= "\n@author: $author";
    }
  }

  if (!empty($node->dbw_copyright['und'][0]['value'])) {
    $body .= "\n@copyright: " . $node->dbw_copyright['und'][0]['value'];
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
    $node->dbw_id['und'][0]['value'] = $node->nid;
    if (!empty($node->body['und'][0]['value'])) {
      $node->dbw_abstract['und'][0]['value'] = $node->body['und'][0]['value'];
    }
    $node->dbw_copyright['und'][0]['value'] =
      'Copyright (C) 2013 Firstname Lastname. Permission is granted to copy,
distribute and/or modify this document under the terms of the GNU Free
Documentation License, Version 1.1 or any later version published by the Free
Software Foundation; with no Invariant Sections, with no Front-Cover Texts,
and with no Back-Cover Texts. A copy of the license is included in the section
entitled "GNU Free Documentation License.';
  }
  else {
    $node->type = 'docbook_section';
    $node->dbw_sectype['und'][0]['value'] = 'section';
    $node->dbw_id['und'][0]['value'] = $node->nid; //TODO: find a better id
    $node->dbw_content = $node->body;
    //TODO: convert to filter format docbookwiki
    //$node->dbw_content['und'][0]['value'] = $node->body['und'][0]['value'];
    //$node->dbw_content['und'][0]['format'] = $node->body['und'][0]['format'];
  }

  node_save($node);
}
