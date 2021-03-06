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

  // The 'dbw-list' command
  $items['dbw-list'] = array(
    'description' => "Print a list of books.",
    'arguments' => array(),
    'options' => array(
      'type' => array(
        'description' => 'Type of books to be listed (book or docbook).',
        'example-value' => 'docbook',
      ),
    ),
    'examples' => array(
      "drush dbw-list" => 'Print a list of all books.',
      "drush dbl --type=docbook" => 'Print a list of DocBook-s.',
    ),
    'aliases' => array('dbl'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
  );

  // The 'dbw-export' command
  $items['dbw-export'] = array(
    'description' => "Export a book from DocBookWiki in DocBook(XML) format.",
    'arguments' => array(
      'book_id' => 'The id (alias) of the book to be exported.',
    ),
    'options' => array(
    ),
    'examples' => array(
      "drush dbw-export test-docbook > test.xml" => 'Export the book with alias test-docbook and store it to test.xml',
    ),
    'aliases' => array('dbe'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );

  // The 'dbw-import' command
  $items['dbw-import'] = array(
    'description' => "Import a DocBook file into DocBookWiki.",
    'arguments' => array(
      'file.xml' => 'The DocBook file to be imported (absolute path or relative to DRUPAL_ROOT).',
    ),
    'options' => array(
    ),
    'examples' => array(
      "drush -u admin dbw-import \$(pwd)/test.xml" => 'Import test.xml into DocBookWiki as user admin.',
    ),
    'aliases' => array('dbi'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );

  // The command 'docbook2dbw'
  $items['docbook2dbw'] = array(
    'description' => "Transform a docbook document to a format suitable for being imported to DocBookWiki.",
    'arguments' => array(
      'file.xml' => 'The DocBook file to be transformed (absolute path or relative to DRUPAL_ROOT).',
    ),
    'options' => array(
    ),
    'examples' => array(
      "drush docbook2dbw \$(pwd)/test.xml > test1.xml" => 'Transform test.xml and save the result to test1.xml.',
    ),
    'aliases' => array('2dbw'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
  );

  // The 'dbw-convert' command
  $items['dbw-convert'] = array(
    'description' => "Convert between Book and DocBook.",
    'arguments' => array(
      'bid' => 'The id of the book to be converted.',
    ),
    'options' => array(
      'type' => array(
        'description' => 'Type to convert to: book or docbook (default docbook).',
        'example-value' => 'docbook',
      ),
    ),
    'examples' => array(
      "drush dbw-convert 47" => 'Convert the book with nid=47 to DocBook.',
    ),
    'aliases' => array('dbc'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
  );

  // These will appear in the list of topics when `drush topic` is executed.
  $items['docbook'] = array(
    'description' => dt('Importing and exporting DocBook files.'),
    'hidden' => TRUE,
    'topic' => TRUE,
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'callback' => 'drush_print_file',
    'callback arguments' => array(dirname(__FILE__) . '/docbook-topic.txt'),
  );

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
    case 'drush:dbw-list':
      return dt("Print a list of books.");
    case 'drush:dbw-export':
      return dt("Export a book from DocBookWiki.");
    case 'drush:dbw-import':
      return dt("Import a DocBook file into DocBookWiki.");
    case 'drush:docbook2dbw':
      return dt("Transform a docbook document to a format suitable for being imported to DocBookWiki.");
    case 'meta:docbook:title':
      return dt("Docbook Commands");
    case 'meta:docbook:summary':
      return dt("Handle the import/export of DocBook files for DocBookWiki.");
  }
}

/**
 * Command callback for dbw-list.
 */
function drush_docbook_dbw_list() {
  $book_type = drush_get_option('type');
  if ($book_type) {
    _drush_docbook_list($book_type);
  }
  else {
    _drush_docbook_list('book');
    _drush_docbook_list('docbook');
  }
}

function _drush_docbook_list($book_type) {
  print "Books of type: $book_type\n";
  foreach (book_get_books() as $book) {
    if ($book['type'] != $book_type)  continue;

    $path = drupal_lookup_path("alias", $book['link_path']);
    if (!$path)  $path = $book['link_path'];

    $title = $book['title'];
    print "   $path : $title\n";
  }
}

/**
 * Command callback for dbw-export.
 */
function drush_docbook_dbw_export($alias) {
  $path = drupal_lookup_path("source", $alias);
  $node = menu_get_object("node", 1, $path);
  if (empty($node)) {
    print "\nCould find no book with alias '$alias'.\n\n";
    exit(0);
  }
  print book_export_docbook($node->nid);
}

/**
 * Command callback for dbw-import.
 */
function drush_docbook_dbw_import($file) {
  include_once drupal_get_path('module', 'docbook') . '/import/import.inc';
  docbook_import($file);
}

/**
 * Command callback for docbook2dbw.
 */
function drush_docbook_docbook2dbw($file) {
  include_once drupal_get_path('module', 'docbook') . '/import/transform.inc';
  docbook_docbook2dbw($file);
}

/**
 * Command callback for dbw-convert.
 */
function drush_docbook_dbw_convert($alias) {
  $book_type = drush_get_option('type');
  if (!$book_type)  $book_type = 'docbook';

  // get the id of the book to be converted
  $path = drupal_lookup_path("source", $alias);
  if (!$path)  $path = $alias;
  $book_id = str_replace('node/', '', $path);

  //check whether such a book exists
  $node = node_load($book_id);
  if (!$node) {
    return drush_set_error('NOT_FOUND', dt("The book '!book' could not be found!", array('!book' => $alias)));
  }

  include_once drupal_get_path('module', 'pathauto') . "/pathauto.inc";
  include_once drupal_get_path('module', 'dbwiki') . "/converter/class.Info.php";
  include_once drupal_get_path('module', 'docbook') . "/import/import.inc";
  include_once drupal_get_path('module', 'docbook') . '/include/docbook.change_type.inc';
  docbook_change_book_type($book_id, $book_type);
}
