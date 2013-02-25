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
        '#markup' => '<p>' . t('You must create a book node before you anything can be selected as a webform book.') . '</p>',
      ),
    );
  }

  // Get the nids of existing webform books from the webform_book_nids variable.
  $webform_books = variable_get('webform_book_nids', array());
  $form['#tree'] = TRUE;
  $form['instructions'] = array(
    '#markup' => '<p>' . t('Check the box next to the title of each book that you want to make available as a webform book.') . '</p>',
  );
  foreach ($books as $nid => $book) {
    $form['books'][$nid]['status'] = array(
      '#type' => 'checkbox',
      '#title' => $book['title'],
      '#default_value' => in_array($nid, $webform_books),
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
  // Save the nids of the checked books to the webform_book_nids variable.
  $nids = array();
  foreach ($form_state['values']['books'] as $nid => $value) {
    if ($value['status']) {
      $nids[] = $nid;
    }
  }
  variable_set('webform_book_nids', $nids);
  drupal_set_message(t('Your webform book settings have been saved.'));
}
