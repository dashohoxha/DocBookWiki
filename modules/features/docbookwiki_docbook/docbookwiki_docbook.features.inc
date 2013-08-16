<?php
/**
 * @file
 * docbookwiki_docbook.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function docbookwiki_docbook_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function docbookwiki_docbook_node_info() {
  $items = array(
    'docbook' => array(
      'name' => t('DocBook'),
      'base' => 'node_content',
      'description' => t('<em>DocBooks</em> are <em>Books</em> that can be exported into <em>DocBook/XML</em> format. It is also possible to import simple DocBook documents and to convert Books to DocBooks.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => t('This will create a new DocBook document (of type \'book\' or \'article\').'),
    ),
    'docbook_section' => array(
      'name' => t('DocBook Section'),
      'base' => 'node_content',
      'description' => t('A section is a DocBook chunk. It can be of type Chapter, Section, Simplesect, etc. Section and Simplesect can be nested under Chapter and Section, however Simplesect can have no nested sections.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => t('This will create a new DocBook Section (\'chapter\', \'section\', \'simplesect\', etc.)'),
    ),
  );
  return $items;
}
