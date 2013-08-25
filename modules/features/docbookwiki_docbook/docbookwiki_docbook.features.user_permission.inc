<?php
/**
 * @file
 * docbookwiki_docbook.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function docbookwiki_docbook_user_default_permissions() {
  $permissions = array();

  // Exported permission: create docbook content.
  $permissions['create docbook content'] = array(
    'name' => 'create docbook content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: create docbook_section content.
  $permissions['create docbook_section content'] = array(
    'name' => 'create docbook_section content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: delete any docbook content.
  $permissions['delete any docbook content'] = array(
    'name' => 'delete any docbook content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: delete any docbook_section content.
  $permissions['delete any docbook_section content'] = array(
    'name' => 'delete any docbook_section content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: delete own docbook content.
  $permissions['delete own docbook content'] = array(
    'name' => 'delete own docbook content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: delete own docbook_section content.
  $permissions['delete own docbook_section content'] = array(
    'name' => 'delete own docbook_section content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: edit any docbook content.
  $permissions['edit any docbook content'] = array(
    'name' => 'edit any docbook content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: edit any docbook_section content.
  $permissions['edit any docbook_section content'] = array(
    'name' => 'edit any docbook_section content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: edit own docbook content.
  $permissions['edit own docbook content'] = array(
    'name' => 'edit own docbook content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: edit own docbook_section content.
  $permissions['edit own docbook_section content'] = array(
    'name' => 'edit own docbook_section content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: use text format docbookwiki.
  $permissions['use text format docbookwiki'] = array(
    'name' => 'use text format docbookwiki',
    'roles' => array(
      0 => 'authenticated user',
    ),
    'module' => 'filter',
  );

  return $permissions;
}
