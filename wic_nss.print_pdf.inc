<?php

/**
 * @file
 * Page callbacks for self-assessment pdf printing.
 */

function wic_nss_assessment_print_pdf_page($bid) {
  // Load include files for creating the PDF.
  if ($path = libraries_get_path('tcpdf')) {
    require_once($path . '/tcpdf.php');
  }
  else {
    return t('Error: Cannot load the TCPDF library.  Please contact an administrator.');
  }

  // Initialize the PDF variable.
  global $user;
  $book = node_load($bid);
  $creator = variable_get('site_name', 'Drupal');
  $author = $user->name;
  $title = $book->title;
  $pdf = _wic_nss_init_tcpdf($creator, $author, $title);

  // Add the submissions as pages to the PDF document.
  $pages = array();
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
    // Render the html view of the submission.
    $pages[$submission->sid] = array(
      'title' => $node->title,
      'content' => webform_submission_render($node, $submission, NULL, 'html'),
    );
  }
  $pdf = _wic_nss_pdf_content($pdf, $pages);
  $pdf = _wic_nss_pdf_table_of_contents($pdf);

  // Set the HTTP headers.
  drupal_add_http_header('Content-Type', 'application/pdf');
  drupal_add_http_header('Cache-Control', 'public, must-revalidate, max-age=0'); // HTTP/1.1
  drupal_add_http_header('Pragma', 'public');
  drupal_add_http_header('Content-Disposition', 'inline; filename="assessment.pdf";');

  // Close and output PDF document.
  die($pdf->Output('assessment.pdf', 'S'));
}

function _wic_nss_init_tcpdf($creator, $author, $title) {
  module_load_include('inc', 'wic_nss', 'wic_nss.class');

  // Create a new PDF document.
  $pdf = new WICNSS(PDF_PAGE_ORIENTATION, PDF_UNIT, 'LETTER', TRUE, 'UTF-8', FALSE);

  // Set the basic document information.
  $pdf->SetCreator($creator);
  $pdf->SetAuthor($author);
  $pdf->SetTitle($title);

  return $pdf;
}

function _wic_nss_pdf_content($pdf, $pages) {
  foreach ($pages as $page) {
    // Add a new page to the PDF document.
    $pdf->AddPage();
    $pdf->Bookmark($page['title'], 0, 0, '', 'B', array(0, 64, 128));
    $html = '<style>
      h1,
      h2,
      h3,
      h4,
      h5,
      h6 {
        color: #4BBDDA;
        font-family: times;
      }
      label {
        display: block;
      }
    </style>
    <h2>' . $page['title'] . '</h2>' . $page['content'];
    $pdf->writeHTML($html);
  }
  return $pdf;
}

function _wic_nss_pdf_table_of_contents($pdf) {
  // Add the table of contents page.
  $pdf->addTOCPage();
  // Don't print the footer.
  $pdf->setPrintFooter(false);

  // Write the page title.
  $html = '<style>
      h1,
      h2,
      h3,
      h4,
      h5,
      h6 {
        color: #4BBDDA;
        font-family: times;
      }
    </style>
    <h1>' . t('Table of contents') . '</h1>';
  $pdf->writeHTML($html);
  $pdf->Ln();

  // Add a simple table of contents.
  $pdf->addTOC(1, 'helvetica', '.', 'INDEX', 'B', array(128, 0, 0));
  $pdf->endTOCPage();

  return $pdf;
}
