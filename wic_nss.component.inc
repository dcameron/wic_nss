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
      'description' => '',
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
 * Render the goal component to be part of a form.
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
 * Display the result of a submission for the goal component.
 *
 * @param $component
 *   A Webform component array.
 * @param $value
 *   An array of information containing the submission result, directly
 *   correlating to the webform_submitted_data database table schema.
 * @param $format
 *   Either 'html' or 'text'. Defines the format that the content should be
 *   returned as. Make sure that returned content is run through check_plain()
 *   or other filtering functions when returning HTML.
 *
 * @return
 *   A renderable element containing at the very least these properties:
 *    - #title
 *    - #weight
 *    - #component
 *    - #format
 *    - #value
 *   Webform also uses #theme_wrappers to output the end result to the user,
 *   which will properly format the label and content for use within an e-mail
 *   (such as wrapping the text) or as HTML (ensuring consistent output).
 */
function _webform_display_wic_nss_goal($component, $value, $format = 'html') {
  if ($format == 'text') {
    $element = array(
      '#title' => $component['name'],
      '#weight' => $component['weight'],
      '#theme_wrappers' => array('webform_element_text'),
    );
  }
  else {
    $element = array(
      '#type' => 'fieldset',
      '#title' => $component['name'],
      '#weight' => $component['weight'],
    );
  }
  $element['goal'] = array(
    '#title' => t('What are your agency’s goals/plans for improvement?'),
    '#weight' => $component['weight'] + 1,
    '#theme' => 'webform_display_textarea',
    '#theme_wrappers' => $format == 'html' ? array('webform_element') : array('webform_element_text'),
    '#format' => $format,
    '#value' => isset($value['goal']) ? $value['goal'] : '',
    '#translatable' => array('title'),
    '#parents' => array(
      'goal',
      'set-a-goal-for-improvement',
    ),
  );
  $element['resources'] = array(
    '#title' => t('What resources/technical assistance does your agency need to reach your improvement goals/pans?'),
    '#weight' => $component['weight'] + 2,
    '#theme' => 'webform_display_textarea',
    '#theme_wrappers' => $format == 'html' ? array('webform_element') : array('webform_element_text'),
    '#format' => $format,
    '#value' => isset($value['resources']) ? $value['resources'] : '',
    '#translatable' => array('title'),
    '#parents' => array(
      'resources',
      'set-a-goal-for-improvement',
    ),
  );
  $due_date_value = webform_date_array(isset($value['due_date']) ? $value['due_date'] : '', 'date');
  $element['due_date'] = array(
    '#title' => t('Set a due date for this goal.'),
    '#weight' => $component['weight'] + 3,
    '#theme' => 'webform_display_date',
    '#theme_wrappers' => $format == 'html' ? array('webform_element') : array('webform_element_text'),
    '#format' => $format,
    '#value' => $due_date_value,
    '#translatable' => array('title'),
    '#parents' => array(
      'due_date',
      'set-a-goal-for-improvement',
    ),
  );
  if ($value['goal_complete']) {
    $element['goal_complete'] = array(
      '#markup' => $value['goal_complete'],
      '#weight' => $component['weight'] + 4,
    );
  }

  $element['#format'] = $format;

  return $element;
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
