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
  drupal_add_js(drupal_get_path('module', 'wic_nss') . '/wic_nss.dashboard.js');
  $page = '';
  $nodes = node_load_multiple($bids);

  // Set the page title if we're viewing a specific assessment.
  if ($bid) {
    drupal_set_title(t('Your assessment dashboard') . ': ' . $nodes[$bid]->title);
  }

  foreach ($nodes as $nid => $node) {
    // Print a menu of nodes in each assessment.
    $tree = menu_tree_all_data($node->book['menu_name']);
    if (!empty($tree)) {
      $page['menu_heading'] = array(
        '#markup' => '<h3>' . t('Self-assessment menu') . '</h3>',
      );
      $page['assessment_menu_' . $nid] = array(
        '#theme' => 'wic_nss_assessment_menu',
        '#elements' => menu_tree_output($tree),
      );
    }

    // Print a link to the assessment's print pages.
    $links = array(
      'assessment_print_page_' . $nid => array(
        'title' => t('View a report of your self-assessment.'),
        'href' => 'dashboard/' . $nid . '/view',
      ),
      'assessment_print_pdf_page_' . $nid => array(
        'title' => t('Print or save a PDF report of your self-assessment.'),
        'href' => 'dashboard/' . $nid . '/print',
      ),
    );
    $page['print_links_' . $nid] = array(
      '#theme' => 'links',
      '#links' => $links,
      '#heading' => array(
        'text' => t('Self-assessment report'),
        'level' => 'h3',
      ),
    );

    // Print the table of goals.
    $nids = _wic_nss_get_flat_book_nodes($tree);
    $goals = _wic_nss_get_user_goals($nids);
    if (!empty($goals)) {
      $header = array(t('Form'), t('Goal'), t('Due date'));
      $rows = array();
      foreach ($goals as $goal) {
        $goal_node = node_load($goal->nid);
        $rows[] = array(
          'node_title' => l($goal_node->title, 'node/' . $goal->nid),
          'goal' => $goal->goal,
          'due_date' => date('F d, Y', $goal->due_date),
        );
      }
      $page['goals_heading'] = array(
        '#markup' => '<h3>' . t("Improvement goals you've set") . '</h3>',
      );
      $page['goals'] = array(
        '#theme' => 'table',
        '#header' => $header,
        '#rows' => $rows,
        '#attributes' => array(
          'class' => array(
            'wic-nss-goals-table',
          ),
        ),
      );
    }
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
function wic_nss_assessment_view_page($bid) {
  $content = _wic_nss_get_report_content($bid);

  // Display the empty report message if there is no content to display.
  if (empty($content)) {
    $wic_nss_empty_report = array(
      '#theme' => 'wic_nss_empty_report',
      '#bid' => $bid,
    );
    return drupal_render($wic_nss_empty_report);
  }

  // Display the assessment report.
  $page = array(
    '#theme' => 'wic_nss_assessment_view_page',
    '#bid' => $bid,
    '#content' => $content,
  );
  return $page;
}

/**
 * Preprocess function for wic_nss_assessment_view_page.tpl.php
 */
function template_preprocess_wic_nss_assessment_view_page(&$variables) {
  // Create an array of links for a jump menu.
  $content_links = array();
  foreach ($variables['content'] as $nid => $content) {
    // Add a link to the jump menu.
    $content_links[] = array(
      'title' => $content['title'],
      'href' => 'dashboard/' . $variables['bid'] . '/view',
      'fragment' => 'node-' . $nid,
    );
  }
  $links = array(
    '#theme' => 'links',
    '#links' => $content_links,
  );
  $variables['table_of_contents'] = drupal_render($links);
}
