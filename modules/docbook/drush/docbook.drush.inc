<?php
/**
 * @file
 * Drush commands for importing/exporting DocBook files.
 */

/**
 * Implementation of hook_drush_command().
 *
 * @return
 *   An associative array describing commands.
 */
function docbook_drush_command() {
  $items = array();

  // The 'dbw-import' command
  $items['dbw-import'] = array(
    'description' => "Import a DocBook file into DocBookWiki.",
    'arguments' => array(
      'file.xml' => 'The DocBook file to be imported (absolute path or relative to DRUPAL_ROOT).',
    ),
    'options' => array(
    ),
    'examples' => array(
      "drush dbw-import test.xml" => 'Import test.xml into DocBookWiki.',
    ),
    'aliases' => array('dbi'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
  );

  /* TODO: describe drush commands in the file docbook-topic.txt
  // These will appear in the list of topics when `drush topic` is executed.
  // To view this topic, run `drush --include=/full/path/to/examples topic`
  $items['docbook-exposition'] = array(
    'description' => dt('Importing and exporting DocBook files.'),
    'hidden' => TRUE,
    'topic' => TRUE,
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
    'callback' => 'drush_print_file',
    'callback arguments' => array(dirname(__FILE__) . '/docbook-topic.txt'),
  );
  */

  return $items;
}

/**
 * Implementation of hook_drush_help().
 *
 * @param
 *   A string with the help section (prepend with 'drush:')
 *
 * @return
 *   A string with the help text for your command.
 */
function docbook_drush_help($section) {
  switch ($section) {
    case 'drush:dbw-import':
      return dt("This command will import a DocBook file into DocBookWiki.");
      /*
    case 'drush:dbw-export':
      return dt("This command will export a DocBook file into DocBookWiki.");
      */
    case 'meta:docbook:title':
      return dt("Docbook Commands");
    case 'meta:docbook:summary':
      return dt("Handle the import/export of DocBook files for DocBookWiki.");
  }
}

/**
 * Command callback for dbw-import.
 */
function drush_docbook_dbw_import($file) {
  include_once drupal_get_path('module', 'docbook') . '/import/import.inc';
  docbook_import($file);
}

