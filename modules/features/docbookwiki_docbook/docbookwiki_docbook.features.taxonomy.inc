<?php
/**
 * @file
 * docbookwiki_docbook.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function docbookwiki_docbook_taxonomy_default_vocabularies() {
  return array(
    'docbook_keywords' => array(
      'name' => 'DocBook Keywords',
      'machine_name' => 'docbook_keywords',
      'description' => NULL,
      'hierarchy' => '0',
      'module' => 'taxonomy',
      'weight' => '0',
      'rdf_mapping' => array(
        'rdftype' => array(
          0 => 'skos:ConceptScheme',
        ),
        'name' => array(
          'predicates' => array(
            0 => 'dc:title',
          ),
        ),
        'description' => array(
          'predicates' => array(
            0 => 'rdfs:comment',
          ),
        ),
      ),
    ),
  );
}
