<?php

/**
 * @file
 * Administration page callbacks for the wic_nss module.
 */

/**
 * Form constructor for the NSS website admin page.
 */
function wic_nss_admin_page() {
  $form['wic_nss_user_register_form_instructions'] = array(
    '#type' => 'textarea',
    '#title' => t('Instructions for the user registration form.'),
    '#default_value' => variable_get('wic_nss_user_register_form_instructions', ''),
    '#required' => FALSE,
  );
  $form['wic_nss_goal_reminder_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Message subject for goal reminder emails.'),
    '#default_value' => variable_get('wic_nss_goal_reminder_subject', ''),
    '#required' => FALSE,
  );
  $form['wic_nss_goal_reminder_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Message body for goal reminder emails.'),
    '#description' => t('Text entered here will appear above the list of upcoming goals.'),
    '#default_value' => variable_get('wic_nss_goal_reminder_body', ''),
    '#required' => FALSE,
  );
  $form['wic_nss_90_day_notice_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Message subject for emails sent to users with no activity for 90 days.'),
    '#default_value' => variable_get('wic_nss_goal_reminder_subject', ''),
    '#required' => FALSE,
  );
  $form['wic_nss_90_day_notice_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Message body for emails sent to users with no activity for 90 days.'),
    '#description' => t('Text entered here will appear above the list of upcoming goals.'),
    '#default_value' => variable_get('wic_nss_90_day_notice_body', ''),
    '#required' => FALSE,
  );
  return system_settings_form($form);
}

/**
 * Form constructor for the webform book admin page.
 */
function wic_nss_select_books($form, &$form_state) {
  $books = book_get_books();
  if (empty($books)) {
    return array(
      'empty' => array(
        '#markup' => '<p>' . t('You must create a book node before you anything can be selected as an assessment.') . '</p>',
      ),
    );
  }

  // Get the nids of existing assessments.
  $assessments = variable_get('wic_nss_assessment_nids', array());
  $form['#tree'] = TRUE;
  $form['instructions'] = array(
    '#markup' => '<p>' . t('Check the box next to the title of each book that you want to make available as an assessment.') . '</p>',
  );
  foreach ($books as $nid => $book) {
    $form['books'][$nid]['status'] = array(
      '#type' => 'checkbox',
      '#title' => $book['title'],
      '#default_value' => in_array($nid, $assessments),
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => array('wic_nss_select_books_submit'),
  );
  return $form;
}

/**
 * Form submission handler for wic_nss_select_books().
 *
 * @see wic_nss_select_books()
 */
function wic_nss_select_books_submit($form, &$form_state) {
  // Save the nids of the checked books.
  $nids = array();
  foreach ($form_state['values']['books'] as $nid => $value) {
    if ($value['status']) {
      $nids[] = $nid;
    }
  }
  variable_set('wic_nss_assessment_nids', $nids);
  drupal_set_message(t('Your assessment settings have been saved.'));
}
