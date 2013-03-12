<?php

/**
 * @file
 * Page callbacks for self-assessment management and printing.
 */

/**
 * Title callback: Returns the title of the assessment dashboard page.
 *
 * Set the title equal to the assessment's book node title here.  That way
 * bookmark and menu links are correctly displayed like "Your assessment
 * dashboard >> Node title."  This is overridden in wic_nss_dashboard_page() to
 * include "Your assessment dashboard: " before the node title.
 *
 * @param int $bid
 *   The nid of the assessment's book node.
 *
 * @return string
 *   The title of the page.
 */
function wic_nss_dashboard_page_title($bid) {
  $node = node_load($bid);
  return $node->title;
}

/**
 * Page callback: Displays the submission status of webforms in assessments.
 *
 * @param int $bid
 *   The nid of the assessment book node.
 */
function wic_nss_dashboard_page($bid = FALSE) {
  $bids = array();
  // Load all assessment book nodes if the argument isn't specified.
  if (!$bid) {
    $bids = variable_get('wic_nss_assessment_nids', array());
  }
  else {
    $bids[] = $bid;
  }

  if (empty($bids)) {
    return '<p>' . t("We're sorry, there are no self-assessments set up yet.  Please contact an administrator.") . '</p>';
  }
  $page = '';
  $nodes = node_load_multiple($bids);

  // Set the page title if we're viewing a specific assessment.
  if ($bid) {
    drupal_set_title(t('Your assessment dashboard') . ': ' . $nodes[$bid]->title);
  }

  foreach ($nodes as $nid => $node) {
    // Print a menu of nodes in each assessment.
    $tree = menu_tree_all_data($node->book['menu_name']);
    $page['assessment_menu_' . $nid] = array(
      '#theme' => 'wic_nss_assessment_menu',
      '#elements' => menu_tree_output($tree),
    );
    // Print a link to the assessment's print page.
    $page['assessment_print_page_' . $nid] = array(
      '#prefix' => '<div class="assessment-print-link">',
      '#markup' => l(t('Print this assessment'), 'dashboard/' . $nid . '/print'),
      '#suffix' => '</div>',
    );
  }
  return $page;
}

/**
 * Preprocess function for wic_nss_assessment_menu.tpl.php
 */
function template_preprocess_wic_nss_assessment_menu(&$variables) {
  // Add webform submission status text.
  wic_nss_menu_links_add_webform_submission_status($variables['elements']);
  return;
}

/**
 * Page callback: Displays all submissions in an assessment.
 *
 * @param int bid
 *   The nid of the assessment book node.
 */
function wic_nss_assessment_print_page($bid) {
  global $user;
  $submissions = array();
  $rendered_submissions = array();

  // Query for all nids in this assessment.
  $nids = db_query("SELECT nid FROM {book} WHERE bid = :bid", array(':bid' => $bid))->fetchCol();
  $nodes = node_load_multiple($nids);
  foreach ($nodes as $nid => $node) {
    if ($node->type != 'webform') {
      continue;
    }
    // Load the user's most recent submission for this webform node.
    $submission = wic_nss_get_last_webform_submission($nid, $user->uid);
    if ($submission == FALSE) {
      continue;
    }
    $submissions[$submission->sid] = $submission;
    // Render the html view of the submission.
    $rendered_submissions[$submission->sid] = webform_submission_render($node, $submission, NULL, 'html');
  }

  $page = array(
    '#theme' => 'wic_nss_assessment_print_page',
    '#bid' => $bid,
    '#nodes' => $nodes,
    '#submissions' => $submissions,
    '#rendered_submissions' => $rendered_submissions,
  );
  return $page;
}

/**
 * Preprocess function for wic_nss_assessment_print_page.tpl.php
 */
function template_preprocess_wic_nss_assessment_print_page(&$variables) {
  // Create an array of links for a jump menu.
  $content_links = array();
  // Create a single content array with the elements we need to render.
  $variables['submission_content'] = array();
  foreach ($variables['rendered_submissions'] as $sid => $content) {
    $submission = $variables['submissions'][$sid];
    $title = $variables['nodes'][$submission->nid]->title;
    $body = $variables['nodes'][$submission->nid]->body['und'][0]['safe_value'];
    // Add a link to the jump menu.
    $content_links[] = array(
      'title' => $title,
      'href' => 'dashboard/' . $variables['bid'] . '/print',
      'fragment' => 'submission-' . $sid,
    );
    // Add the submission's content and node title to the content array.
    $variables['submission_content'][] = array(
      'sid' => $sid,
      'title' => check_plain($title),
      'body' => $body,
      'content' => $content,
    );
  }
  $variables['table_of_contents'] = theme('links', array('links' => $content_links));
}
