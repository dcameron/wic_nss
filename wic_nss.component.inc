<?php

/**
 * @file
 * Creates a webform component for setting NSS goals.
 */

function _webform_defaults_wic_nss_goal() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'mandatory' => 0,
    'pid' => 0,
    'weight' => 0,
    'extra' => array(
      'title_display' => TRUE,
      'private' => FALSE,
    ),
  );
}

/**
 * Generate the form for editing the goal component.
 *
 * @param $component
 *   A Webform component array.
 *
 * @return
 *   An array of form items to be displayed on the edit component page.
 */
function _webform_edit_wic_nss_goal($component) {
  $form = array();

  return $form;
}

/**
 * Render a Webform component to be part of a form.
 *
 * @param $component
 *   A Webform component array.
 * @param $value
 *   If editing an existing submission or resuming a draft, this will contain
 *   an array of values to be shown instead of the default in the component
 *   configuration. This value will always be an array, keyed numerically for
 *   each value saved in this field.
 * @param $filter
 *   Whether or not to filter the contents of descriptions and values when
 *   rendering the component. Values need to be unfiltered to be editable by
 *   Form Builder.
 *
 * @see _webform_client_form_add_component()
 */
function _webform_render_wic_nss_goal($component, $value = NULL, $filter = TRUE) {
  $form_item = array(
    '#type' => 'fieldset',
    '#title' => $filter ? _webform_filter_xss($component['name']) : $component['name'],
    '#required' => $component['mandatory'],
    '#weight' => $component['weight'],
    '#description'   => $filter ? _webform_filter_descriptions($component['extra']['description']) : $component['extra']['description'],
  );
  $form_item['goal'] = array(
    '#type' => 'textarea',
    '#title' => t('What are your agency’s goals/plans for improvement?'),
  );
  $form_item['resources'] = array(
    '#type' => 'textarea',
    '#title' => t('What resources/technical assistance does your agency need to reach your improvement goals/pans?'),
  );
  $form_item['due_date'] = array(
    '#type' => 'date',
    '#title' => t('Set a due date for this goal.'),
  );
  $form_item['goal_complete'] = array(
    '#type' => 'checkbox',
    '#title' => t('Check here if this goal has been completed.'),
    '#return_value' => t('This goal has been completed!'),
  );

  if (isset($value)) {
    $form_item['goal']['#default_value'] = $value['goal'];
    $form_item['resources']['#default_value'] = $value['resources'];
    $form_item['due_date']['#default_value'] = webform_date_array($value['due_date'], 'date');
    $form_item['goal_complete']['#default_value'] = $value['goal_complete'];
  }

  return $form_item;
}

/**
 * Change the input values before saving to the database.
 *
 * Webform expects a component to consist of a single field, or a single array
 * of fields. If you have a component that requires a deeper form tree
 * you must flatten the data into a single array using this callback
 * or by setting #parents on each field to avoid data loss and/or unexpected
 * behavior.
 *
 * Note that Webform will save the result of this function directly into the
 * database.
 *
 * @param $component
 *   A Webform component array.
 * @param $value
 *   The POST data associated with the user input.
 *
 * @return
 *   An array of values to be saved into the database. Note that this should be
 *   a numerically keyed array.
 */
function _webform_submit_wic_nss_goal($component, $value) {
  // Convert the date to an ISO 8601 format.
  $value['due_date'] = ($value['due_date']['year'] && $value['due_date']['month'] && $value['due_date']['day']) ? webform_date_string($value['due_date'], 'date') : '';
  return $value;
}
