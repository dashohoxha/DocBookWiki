<?php
/**
 * This include file defines the functions _docbook_install_fields()
 * and _docbook_install_instancies(), which return arrays of fields
 * and instancies related to content types 'docbook' and 'docbook_section'.
 */

/**
 * Called from hook_uninstall() to delete fields and instancies.
 */
function _docbook_install_delete_fields_and_instancies() {
  // delete fields
  foreach ( _docbook_install_fields() as $field ) {
    field_delete_field($field['field_name']);
  }

  // delete instances
  $instances = field_info_instances('node', 'docbook_section');
  foreach ($instances as $instance_name => $instance) {
    field_delete_instance($instance);
  }
  $instances = field_info_instances('node', 'docbook');
  foreach ($instances as $instance_name => $instance) {
    field_delete_instance($instance);
  }
}


/**
 * Return an array of the fields that are to be installed.
 * (see: http://www.thecarneyeffect.co.uk/creating-custom-content-type-adding-fields-programmatically-drupal-7)
 */
function _docbook_install_fields() {

  return array(

    array(
      'field_name' => 'field_dbw_author',
      'type' => 'text',
      'cardinality' => '-1',
    ),

    array(
      'field_name' => 'field_dbw_id',
      'type' => 'text',
      'settings' => array(
        'max_length' => '50',
      ),
    ),

    array(
      'field_name' => 'field_dbw_keywords',
      'type' => 'taxonomy_term_reference',
      'cardinality' => '-1',
      'settings' => array(
        'allowed_values' => array(
          array(
            'vocabulary' => 'docbook_keywords',
            'parent' => '0',
          ),
        ),
      ),
    ),

    array(
      'field_name' => 'field_dbw_date',
      'type' => 'text',
      'settings' => array(
        'max_length' => '50',
      ),
    ),

    array(
      'field_name' => 'field_dbw_releaseinfo',
      'type' => 'text',
      'settings' => array(
        'max_length' => '100',
      ),
    ),

    array(
      'field_name' => 'field_dbw_abstract',
      'type' => 'text_long',
    ),

    array(
      'field_name' => 'field_dbw_copyright',
      'type' => 'text_long',
    ),

    array(
      'field_name' => 'field_dbw_doctype',
      'type' => 'list_text',
      'settings' => array(
        'allowed_values' => array(
          'book' => 'book',
          'article' => 'article',
        ),
        'allowed_values_function' => '',
      ),
    ),

    array(
      'field_name' => 'field_dbw_content',
      'type' => 'text_long',
    ),

    array(
      'field_name' => 'field_dbw_sectype',
      'type' => 'list_text',
      'settings' => array(
        'allowed_values' => array(
          'chapter' => 'chapter',
          'section' => 'section',
          'simplesect' => 'simplesect',
        ),
      ),
    ),

  );
}

/**
 * Return an array of the instancies that are to be installed.
 * (see: http://www.thecarneyeffect.co.uk/creating-custom-content-type-adding-fields-programmatically-drupal-7)
 */
function _docbook_install_instancies() {

  return array(

    // docbook instancies

    array(
      'field_name' => 'field_dbw_doctype',
      'entity_type' => 'node',
      'bundle' => 'docbook',
      'label' => 'Type',
      'description' => 'The type of the docbook document.',
      'widget' => array(
        'weight' => '0',
        'type' => 'options_select',
      ),
      'required' => TRUE,
      'default_value' => array(
        array('value' => 'article'),
      ),
    ),

    array(
      'field_name' => 'field_dbw_id',
      'entity_type' => 'node',
      'bundle' => 'docbook',
      'label' => 'ID',
      'description' => 'A unique alphanumeric identifier for the book/article.',
      'required' => TRUE,
      'widget' => array(
        'type' => 'text_textfield',
        'weight' => '1',
      ),
    ),

    array(
      'field_name' => 'field_dbw_abstract',
      'entity_type' => 'node',
      'bundle' => 'docbook',
      'label' => 'Abstract',
      'description' => 'A summary of the document.',
      'widget' => array(
        'weight' => '3',
        'type' => 'text_textarea',
      ),
    ),

    array(
      'field_name' => 'field_dbw_keywords',
      'entity_type' => 'node',
      'bundle' => 'docbook',
      'label' => 'Keywords',
      'description' => 'Keywords describing the content of the document.',
      'widget' => array(
        'weight' => '4',
        'type' => 'taxonomy_autocomplete',
      ),
    ),

    array(
      'field_name' => 'field_dbw_author',
      'entity_type' => 'node',
      'bundle' => 'docbook',
      'label' => 'Author',
      'description' => 'The name of the author(s) of the document, in the format: firstname, lastname, email, organization, org_url (organization and url are optional).',
      'widget' => array(
        'type' => 'text_textfield',
        'weight' => '5',
      ),
    ),

    array(
      'field_name' => 'field_dbw_date',
      'entity_type' => 'node',
      'bundle' => 'docbook',
      'label' => 'Date',
      'description' => 'The date of publication or revision of the document.',
      'widget' => array(
        'weight' => '6',
        'type' => 'text_textfield',
      ),
    ),

    array(
      'field_name' => 'field_dbw_releaseinfo',
      'entity_type' => 'node',
      'bundle' => 'docbook',
      'label' => 'Release Info',
      'description' => 'Information about the release of the document.',
      'widget' => array(
        'weight' => '7',
        'type' => 'text_textfield',
      ),
    ),

    array(
      'field_name' => 'field_dbw_copyright',
      'entity_type' => 'node',
      'bundle' => 'docbook',
      'label' => 'Copyright',
      'description' => 'Copyright information about the document.',
      'widget' => array(
        'weight' => '8',
        'type' => 'text_textarea',
      ),
      'default_value' => array(
        array(
          'value' => 'Copyright (C) 2013 Firstname Lastname. Permission is granted to copy, distribute and/or modify this document under the terms of the GNU Free Documentation License, Version 1.1 or any later version published by the Free Software Foundation; with no Invariant Sections, with no Front-Cover Texts, and with no Back-Cover Texts. A copy of the license is included in the section entitled "GNU Free Documentation License."',
        ),
      ),
    ),

    // docbook_section instancies

    array(
      'field_name' => 'field_dbw_sectype',
      'entity_type' => 'node',
      'bundle' => 'docbook_section',
      'label' => 'Type',
      'description' => 'The type of this section. Chapter can be used only for the books. SimpleSect can have no nested sections.',
      'widget' => array(
        'weight' => '0',
        'type' => 'options_select',
      ),
      'required' => 1,
      'default_value' => array(
        array('value' => 'section'),
      ),
    ),

    array(
      'field_name' => 'field_dbw_id',
      'entity_type' => 'node',
      'bundle' => 'docbook_section',
      'label' => 'ID',
      'description' => 'Unique alphanumeric identifier of the section.',
      'widget' => array(
        'weight' => '1',
        'type' => 'text_textfield',
      ),
      'required' => TRUE,
    ),

    array(
      'field_name' => 'field_dbw_content',
      'entity_type' => 'node',
      'bundle' => 'docbook_section',
      'label' => 'Content',
      'description' => 'The content of this section.',
      'widget' => array(
        'weight' => '4',
        'type' => 'text_textarea',
        'settings' => array(
          'rows' => '20',
        ),
      ),
      'settings' => array(
        'text_processing' => '1',
      ),
    ),
  );
}
