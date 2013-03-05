<?php

/**
 * @file
 * Page callbacks for self-assessment management and printing.
 */

/**
 * Page callback: Displays all submissions in an assessment.
 *
 * @param $assessment
 *   A fully-loaded assessment object.
 */
function wic_nss_assessment_print_page($assessment) {
  $nodes = array();
  $submissions = array();
  $rendered_submissions = array();

  // Set the page title.
  drupal_set_title(t('Print @title', array('@title' => $assessment->title)));

  // Query for all sids in this assessment.
  $results = db_query("SELECT nid, sid FROM {webform_book_index} WHERE wbid = :wbid ORDER BY created ASC", array(':wbid' => $assessment->wbid));
  foreach ($results as $result) {
    $node = webform_menu_load($result->nid);
    $submission = webform_menu_submission_load($result->sid, $result->nid);
    $nodes[$result->nid] = $node;
    $submissions[$result->sid] = $submission;
    $rendered_submissions[$result->sid] = webform_submission_render($node, $submission, NULL, 'html');
  }

  $page = array(
    '#theme' => 'wic_nss_assessment_print_page',
    '#assessment' => $assessment,
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
  $output = array();
  foreach ($variables['rendered_submissions'] as $sid => $content) {
    // Prepend the webform node's title to the rendered submission content.
    $submission = $variables['submissions'][$sid];
    $title = $variables['nodes'][$submission->nid]->title;
    $output[] = '<h2>' . check_plain($title) . "</h2>\n" . $content;
  }
  if (empty($output)) {
    $variables['submission_content'] = '<p>' . t("You haven't submitted any forms for this self-assessment yet." . '</p>
      <p>' . l(t('Start filling out forms.'), 'node/' . $variables['assessment']->bid) . '</p>');
  }
  else {
    $variables['submission_content'] = implode('<hr />', $output);
  }
}
