<?php
/**
 * @file
 * Functions used for exporting to DocBook format.
 */

/**
 * Generates DocBook for export when invoked by book_export().
 *
 * @param $nid
 *   An integer representing the node id (nid) of the node to export.
 *
 * @return
 *   A string containing DocBook representing the node and its children.
 */
function book_export_docbook($nid) {
  if (!user_access('access printer-friendly version')) {
    drupal_access_denied();
    return;
  }

  $node = node_load($nid);
  if (!isset($node->book)) {
    drupal_not_found();
    return;
  }
  //print '<xmp>'; print_r($node); print '</xmp>';  exit(0);  //debug

  $id = $node->dbw_id['und'][0]['value'];
  $tree = book_menu_subtree_data($node->book);

  $depth = $node->book['depth'];
  if ($depth > 1) {  //exporting a section
    $xml_content = book_export_traverse($tree, 'docbook_node_export');
    $docbook = <<<EOD
<?xml version="1.0" encoding="utf-8" standalone="no"?>

$xml_content
EOD;
  }
  else {    //exporting an article or a book
    $tree = array_values($tree);
    $sections = $tree[0]['below'];
    $contents = book_export_traverse($sections, 'docbook_node_export');

    $info = docbook_info_export($node);

    $doctype = $node->dbw_doctype['und'][0]['value'];
    $title = $node->title;

    $docbook = <<<EOD
<?xml version="1.0" encoding="utf-8" standalone="no"?>

<!DOCTYPE $doctype PUBLIC "-//OASIS//DTD DocBook XML V4.5//EN"
          "http://docbook.org/xml/4.5/docbookx.dtd">

<$doctype id='$id'>
  <title>$title</title>
  $info
  $contents
</$doctype>
EOD;
  }

  header('Content-Type: text/xml; charset=utf-8');
  //header('Content-Type: application/docbook+xml; charset=utf-8');
  //header('Content-Disposition: attachment; filename="' . $id . '.xml"');
  return $docbook;
}

function docbook_info_export($node) {
  //print '<xmp>'; print_r($node); print '</xmp>';  exit(0);  //debug

  $file = drupal_get_path('module', 'dbwiki') . "/converter/class.Info.php";
  include_once $file;
  $info = new Info;

  if (isset($node->dbw_abstract['und'])) {
    $abstract = $node->dbw_abstract['und'][0]['value'];
    $info->abstract = trim($abstract);
  }

  if (isset($node->dbw_copyright['und'])) {
    $copyright = $node->dbw_copyright['und'][0]['value'];
    $info->copyright = trim($copyright);
  }

  if (isset($node->dbw_release['und'])) {
    $release = $node->dbw_release['und'][0]['value'];
    $info->release = trim($release);
  }

  if (isset($node->dbw_date['und'])) {
    $date = $node->dbw_date['und'][0]['value'];
    $info->date = trim($date);
  }

  if (isset($node->dbw_keywords['und'])) {
    $keywords = $node->dbw_keywords['und'];
    foreach ($keywords as $keyword) {
      $term = taxonomy_term_load($keyword['tid']);
      $info->keywords[] = $term->name;
    }
  }

  if (isset($node->dbw_author['und'])) {
    $authors = $node->dbw_author['und'];
    foreach ($authors as $author) {
      $info->parse_author($author['value']);
    }
  }

  $doc_info = $info->to_xml();
  $doctype = $node->dbw_doctype['und'][0]['value'];  // book or article
  $infotag = $doctype."info";

  return "
  <$infotag>
  $doc_info
  </$infotag>
";
}

/**
 * Generates DocBook for a node.
 *
 * @param $node
 *   The node that will be output.
 * @param $children
 *   (optional) All the rendered child nodes within the current node.
 *   Defaults to an empty string.
 *
 * @return
 *   The DocBook generated for the given node.
 *
 * @see book_export_traverse()
 */
function docbook_node_export($node, $children = '') {
  //print '<xmp>'; print_r($node); print '</xmp>'; exit(0); //debug

  $file = drupal_get_path('module', 'dbwiki') . "/converter/package.WikiConverter.php";
  include_once $file;

  $parser = new WikiParser;
  $dbw_content = $node->dbw_content['und'][0]['value'];
  $tpl = $parser->parse_string($dbw_content);

  $indent = '';
  $depth = $node->book['depth'];
  while ($depth > 0) {
    $indent .= '  ';
    $depth -= 1;
  }
  $xml_content = $tpl->to_xml($indent);
  if (trim($xml_content)=='')  $xml_content = '<para/>';

  $section = $node->dbw_sectype['und'][0]['value'];
  $id = $node->dbw_id['und'][0]['value'];
  $title = $node->title;

  $element = "
$indent<$section id='$id'>
$indent  <title>$title</title>
$indent  $xml_content
$indent  $children
$indent</$section>
";

  return $element;
}
