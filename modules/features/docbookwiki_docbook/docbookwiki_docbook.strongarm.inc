<?php
/**
 * @file
 * docbookwiki_docbook.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function docbookwiki_docbook_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_docbook';
  $strongarm->value = array();
  $export['menu_options_docbook'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_docbook_section';
  $strongarm->value = array();
  $export['menu_options_docbook_section'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_docbook';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_docbook'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_docbook_section';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_docbook_section'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_docbook';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_docbook'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_docbook_section';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_docbook_section'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_docbook';
  $strongarm->value = 0;
  $export['node_preview_docbook'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_docbook_section';
  $strongarm->value = 0;
  $export['node_preview_docbook_section'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_docbook';
  $strongarm->value = 0;
  $export['node_submitted_docbook'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_docbook_section';
  $strongarm->value = 0;
  $export['node_submitted_docbook_section'] = $strongarm;

  return $export;
}
