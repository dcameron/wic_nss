<?php
/**
 * @file
 * Defines fields added to user entities.
 */

/**
 * Add the user_wic_agency field to user entities.
 */
function _wic_nss_create_user_wic_agency_field() {
  // Add the WIC agency field.
  $field = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'user_wic_agency',
    'foreign keys' => array(
      'format' => array(
        'columns' => array(
          'format' => 'format',
        ),
        'table' => 'filter_format',
      ),
    ),
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'max_length' => 128,
    ),
    'translatable' => 0,
    'type' => 'text',
  );
  field_create_field($field);

  $instance = array(
    'bundle' => 'user',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => t('The name of the state or local WIC agency with which you are associated.'),
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'user',
    'field_name' => 'user_wic_agency',
    'label' => t('WIC agency'),
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => 1,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 7,
    ),
  );
  field_create_instance($instance);

}

/**
 * Deletes the user_wic_agency field from user entities.
 */
function _wic_nss_delete_user_wic_agency_field() {
  $fields = array(
    'user_wic_agency',
  );
  foreach ($fields as $field) {
    field_delete_field($field);
    watchdog('wic_nss', 'Deleted the :field_name field from the user entity.', array(':field_name' => $field));
  }
}
