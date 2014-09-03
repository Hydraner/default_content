<?php

/**
 * @file
 * Drush integration for the default_content module.
 */

/**
 * Implements hook_drush_command().
 */
function default_content_drush_command() {
  $items['default-content-export'] = [
    'description' => dt('Exports a single entity'),
    'arguments' => [
      'entity_type' => dt('The entity type to export.'),
      'entity_id' => dt('The ID of the entity to export.'),
    ],
    'options' => [
      'file' => dt('Write out the exported content to a file instead of stdout'),
    ],
    'required-arguments' => 2,
  ];
  $items['default-content-import'] = [
    'description' => dt('Imports all default content available'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
    'core' => array('8+'),
    'aliases' => array('dcoi'),
  ];

  return $items;
}

/**
 * Exports a piece of content into the stdout or into a file.
 *
 * @param string $entity_type_id
 *   The entity type ID.
 * @param $entity_id
 *   The entity ID to export.
 *
 * @return string
 *   The rendered export as hal.
 */
function drush_default_content_export($entity_type_id, $entity_id) {
  /** @var \Drupal\default_content\DefaultContentManagerInterface $manager */
  $manager = \Drupal::service('default_content.manager');
  $export = $manager->exportContent($entity_type_id, $entity_id);

  if ($file = drush_get_option('file')) {
    file_put_contents($file, $export);
  }
  else {
    drush_print($export);
  }
}

/**
 * Imports all the content provided by modules.
 *
 * @return string
 *   Success message
 */
function drush_default_content_import() {
  $modules = \Drupal::moduleHandler()->getModuleList();
  foreach ($modules as $module_name => $module) {
    \Drupal::service('default_content.manager')->importContent($module_name);
  }

  drush_print('Content imported');
}
