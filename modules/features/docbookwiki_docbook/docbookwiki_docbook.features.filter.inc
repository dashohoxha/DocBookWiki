<?php
/**
 * @file
 * docbookwiki_docbook.features.filter.inc
 */

/**
 * Implements hook_filter_default_formats().
 */
function docbookwiki_docbook_filter_default_formats() {
  $formats = array();

  // Exported format: DocBookWiki.
  $formats['docbookwiki'] = array(
    'format' => 'docbookwiki',
    'name' => 'DocBookWiki',
    'cache' => '1',
    'status' => '1',
    'weight' => '0',
    'filters' => array(
      'filter_dbwiki' => array(
        'weight' => '0',
        'status' => '1',
        'settings' => array(),
      ),
    ),
  );

  return $formats;
}
