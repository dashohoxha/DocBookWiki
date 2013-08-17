<?php
/*
  For more info see:
    drush help site-alias
    drush topic docs-aliases

  See also:
    drush help rsync
    drush help sql-sync
 */

$aliases['main'] = array (
  'root' => '/var/www/docbookwiki',
  'uri' => 'http://doc.example.org',
  'path-aliases' => array (
    '%profile' => 'profiles/docbookwiki',
    '%exports' => '/var/www/exports',
    '%downloads' => '/var/www/downloads',
  ),
);

$aliases['dev'] = array (
  'parent' => '@main',
  'root' => '/var/www/docbookwiki_dev',
  'uri' => 'http://dev.doc.example.org',
);
