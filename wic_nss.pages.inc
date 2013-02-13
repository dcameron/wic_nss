<?php

/**
 * @file
 * Page callbacks for self-assessment management and printing.
 */

/**
 * Page callback: Activates an assessment.
 */
function wic_nss_assessment_activate($assessment = NULL) {
  // Set the status of all the user's assessments to 0.
  global $user;
  wic_nss_assessment_status_deactivate($user->uid);

  // Update the assessment's status in the database.
  $assessment->status = 1;
  wic_nss_assessment_save($assessment);

  // Get the webform book node's data.
  $node = node_load($assessment->nid);
  drupal_set_message(t('%assessment_title was activated.  Any form submissions in %webform_book_title will be saved as part of this self-assessment.', array('%assessment_title' => $assessment->title, '%webform_book_title' => $node->title)));
  drupal_goto('assessments');
}

/**
 * Form constructor for the assessment add/edit form.
 *
 * @param type $assessment
 *   A fully-loaded assessment object.
 *
 * @return
 *   The assessment add/edit form.
 */
function wic_nss_assessment_form($form, &$form_state, $assessment = NULL) {
  $webform_book_nids = variable_get('webform_book_nids', array());
  if (empty($webform_book_nids)) {
    return array(
      'empty' => array(
        '#markup' => '<p>' . t("We're sorry!  Assessments are unavailable at this time!") . '</p>',
      ),
    );
  }

  if (!isset($form_state['assessment'])) {
    $form_state['assessment'] = $assessment;
  }
  else {
    $assessment = $form_state['assessment'];
  }

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Give a title to this self-assessment.'),
    '#maxlength' => 255,
    '#default_value' => isset($assessment->title) ? $assessment->title : '',
  );
  // Add the webform book nid as a value if there's only one to pick from.
  // Create a select element if there a multiple webform books.
  if (count($webform_book_nids) == 1) {
    $form['nid'] = array(
      '#type' => 'value',
      '#value' => isset($assessment->nid) ? $assessment->nid : reset($webform_book_nids),
    );
  }
  else {
    $nodes = node_load_multiple($webform_book_nids);
    $options = array();
    foreach ($nodes as $nid => $node) {
      $options[$nid] = $node->title;
    }
    $form['nid'] = array(
      '#type' => 'select',
      '#title' => t('Select the type of self-assessment that you want to take.'),
      '#options' => $options,
      '#default_value' => isset($assessment->nid) ? $assessment->nid : NULL,
    );
  }
  $form['activation_info'] = array(
    '#markup' => '<p>' . t('In order to have form submissions saved as part of a single report, you must first activate a self-assessment.') . "</p>\n",
  );
  $form['status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Activate this assessment.'),
    '#default_value' => isset($assessment->status) ? $assessment->status : 0,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => array('wic_nss_assessment_form_submit'),
  );
  if (!empty($assessment->wbid)) {
    $form['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array('wic_nss_assessment_form_delete_submit'),
    );
  }
  return $form;
}

/**
 * Form submission handler for wic_nss_assessment_form().
 *
 * @see wic_nss_assessment_form()
 */
function wic_nss_assessment_form_submit($form, &$form_state) {
  $assessment = $form_state['assessment'];
  $insert = empty($assessment->wbid);

  // Load the assessment object with input from the form.
  $assessment->title = $form_state['values']['title'];
  $assessment->nid = $form_state['values']['nid'];
  $assessment->status = $form_state['values']['status'];
  // Save the assessment to the database.
  $assessment = wic_nss_assessment_save($assessment);

  if ($insert) {
    watchdog('wic_nss', 'Assessment: added %title.', array('%title' => $assessment->title));
    drupal_set_message(t('Assessment %title has been created.', array('%title' => $assessment->title)));
  }
  else {
    watchdog('wic_nss', 'Assessment: updated %title.', array('%title' => $assessment->title));
    drupal_set_message(t('Assessment %title has been updated.', array('%title' => $assessment->title)));
  }

  if ($assessment->status) {
    // Redirect to the assessment's activate page. wic_nss_assessment_activate()
    // can properly handle deactivation of other assessments and displaying the
    // activation message to the user.
    $form_state['redirect'] = 'assessments/' . $assessment->wbid . '/activate';
  }
  else {
    $form_state['redirect'] = 'assessments';
  }
}

/**
 * Form submission handler for wic_nss_assessment_form().
 *
 * Handles the 'Delete' button on the assessment form.
 *
 * @see wic_nss_assessment_form()
 */
function wic_nss_assessment_form_delete_submit($form, &$form_state) {
  $destination = array();
  if (isset($_GET['destination'])) {
    $destination = drupal_get_destination();
    unset($_GET['destination']);
  }
  $assessment = $form_state['assessment'];
  $form_state['redirect'] = array('assessments/' . $assessment->wbid . '/delete', array('query' => $destination));
}

/**
 * Page callback: Displays the add assessment form.
 */
function wic_nss_assessment_add_page() {
  global $user;

  $assessment = (object) array('uid' => $user->uid);
  drupal_set_title(t('Start a new self-assessment'), PASS_THROUGH);
  return drupal_get_form('wic_nss_assessment_form', $assessment);
}

/**
 * Page callback: Displays the assessment management form.
 */
function wic_nss_assessment_default_page() {
  global $user;

  // Get the current user's assessments.
  $assessments = wic_nss_get_assessments_by_user($user->uid);

  $output = '';
  // Print a message about activating assessments.
  if (!empty($assessments)) {
    $output .= '<p>' . t('In order to have form submissions saved as part of a single report, you must first activate a self-assessment.  Click the "activate" link in the Operations column for any Inactive self-assessment.') . "</p>\n";
  }

  // Print a table of the assessments.
  $header = array(t('Self-assessment title'), t('Status'), t('Operations'));
  $rows = array();
  foreach ($assessments as $assessment) {
    // Create the operations links.
    $operations = array();
    $operations['edit'] = array(
      'title' => t('edit'),
      'href' => 'assessments/' . $assessment->wbid . '/edit',
    );
    $operations['delete'] = array(
      'title' => t('delete'),
      'href' => 'assessments/' . $assessment->wbid . '/delete',
    );

    // Create the status text and link.
    if ($assessment->status) {
      $status = t('Active');
    }
    else {
      $status = t('Inactive');
      $operations['activate'] = array(
        'title' => 'activate',
        'href' => 'assessments/' . $assessment->wbid . '/activate',
      );
    }

    $row = array(
      $assessment->title,
      $status,
      theme('links', array('links' => $operations, 'attributes' => array('class' => array('links', 'inline')))),
    );
    $rows[] = $row;
  }
  $caption = t("Here are the self-assessments that you've started so far.");
  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'caption' => $caption, 'empty' => "You haven't started any self-assessments yet!"));

  return $output;
}

/**
 * Page callback: Displays the delete assessment form.
 *
 * @param type $assessment
 *   A fully-loaded assessment object.
 */
function wic_nss_assessment_delete_confirm($form, &$form_state, $assessment) {
  $form_state['assessment'] = $assessment;
  $form['wbid'] = array(
    '#type' => 'value',
    '#value' => $assessment->wbid,
  );
  return confirm_form($form,
    t('Are you sure you want to delete %title?', array('%title' => $assessment->title)),
    'node/' . $assessment->wbid,
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Form submission handler for wic_nss_assessment_delete_confirm().
 *
 * @see wic_nss_assessment_delete_confirm()
 */
function wic_nss_assessment_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $assessment = wic_nss_assessment_load($form_state['values']['wbid']);
    wic_nss_assessment_delete($form_state['values']['wbid']);
    watchdog('wic_nss', 'Assessment: deleted %title.', array('%title' => $assessment->title));
    drupal_set_message(t('Self-assessment %title has been deleted.', array('%title' => $assessment->title)));
  }

  $form_state['redirect'] = 'assessments';
}

/**
 * Page callback: Displays the edit assessment form.
 *
 * @param $assessment
 *   A fully-loaded assessment object.
 *
 * @return
 *   The assessment editing form.
 */
function wic_nss_assessment_edit_page($assessment) {
  drupal_set_title(t('<em>Edit</em> @title', array('@title' => $assessment->title)), PASS_THROUGH);
  return drupal_get_form('wic_nss_assessment_form', $assessment);
}