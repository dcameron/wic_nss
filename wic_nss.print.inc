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
    $submission_text = webform_submission_render($node, $submission, NULL, 'html');
    // Prepare the submission text for printing in the PDF.
    $submission_text = _wic_nss_parse_html($submission_text);
    $pages[$submission->sid] = array(
      'title' => $node->title,
      'body' => $node->body['und'][0]['safe_value'],
      'submission' => $submission_text,
    );

  }

  // Print the cover page.
  _wic_nss_pdf_cover_page($pdf);

  // Print the header and footer on all content pages.
  $pdf->setPrintHeader(TRUE);
  $pdf->setPrintFooter(TRUE);
  $pdf->SetMargins(PDF_MARGIN_LEFT, 30, PDF_MARGIN_RIGHT);
  _wic_nss_pdf_content($pdf, $pages);

  _wic_nss_pdf_table_of_contents($pdf);

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

  // Set defaults for the body.
  $pdf->SetFont('times', '', 12);
  $pdf->SetMargins(PDF_MARGIN_LEFT, 30, PDF_MARGIN_RIGHT);
  $pdf->setImageScale(PDF_IMAGE_SCALE_RATIO);

  // Set defaults for the footer.
  $pdf->SetFooterFont(array('times', '', 10));
  $pdf->SetFooterMargin(PDF_MARGIN_FOOTER);

  return $pdf;
}

function _wic_nss_pdf_cover_page(&$pdf) {
  // Don't print the header or footer.
  $pdf->setPrintHeader(FALSE);
  $pdf->setPrintFooter(FALSE);

  $pdf->AddPage();
  $pdf->Image(dirname(__FILE__) . '/images/cover-bg.jpg', 0, 25, '', '', '', '', '', FALSE, 300, 'C', FALSE, FALSE, 0);

  // Print text.
  //$text_color = isset($vars['cookbook_cover_text_color']) ? $vars['cookbook_cover_text_color'] : '#000099';
  $html = '<span style="color: #FFFFFF; font-size: 35pt;">Self-Assessment</span><br />
    <span style="color: #FFFFFF; font-size: 15pt;">Created: ' . date('F d, Y') . '</span>';
  $pdf->SetTopMargin(175);
  $pdf->SetLeftMargin(100);
  $pdf->writeHTML($html, TRUE, FALSE, TRUE, FALSE, '');

  // Explicitly end the cover page so that the top margin and header can be reset.
  $pdf->endPage();
}

/**
 * Replaces some HTML elements in webform submissions with valid TCPDF HTML.
 *
 * @param string $html
 *   An HTML webform submission.
 *
 * @return string
 *   The parsed HTML webform submission.
 */
function _wic_nss_parse_html($html) {
  $html = str_replace('<fieldset', '<br /><table', $html);
  $html = str_replace('<legend><span class="fieldset-legend">', '<tr><td class="fieldset-legend">', $html);
  $html = str_replace('</span></legend>', '</td></tr><tr><td>', $html);
  $html = str_replace('</fieldset>', '</td></tr></table>', $html);
  $html = str_replace('  <label ', '<label ', $html);
  $html = str_replace('</label>', '</label><br />', $html);
  return $html;
}

function _wic_nss_pdf_content(&$pdf, $pages) {
  foreach ($pages as $page) {
    // Add a new page to the PDF document.
    $pdf->AddPage();
    $pdf->Bookmark($page['title'], 0, 0, '', '', array(0, 0, 0));
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
        font-weight: bold;
      }
      .webform-component-fieldset {
        border: 1px solid #97BE49;
      }
      .fieldset-legend {
        background-color: #C8DAA1;
        border-top: 1px solid #97BE49;
        border-left: 1px solid #97BE49;
        font-weight: bold;
        line-height: 1.5em;
      }
    </style>
    <h2>' . $page['title'] . '</h2>' . $page['body'] . $page['submission'];
    $pdf->writeHTML($html);
  }
}

function _wic_nss_pdf_table_of_contents(&$pdf) {
  // Add the table of contents page.
  $pdf->addTOCPage();
  // Don't print the footer.
  $pdf->setPrintFooter(FALSE);

  // Write the page title.
  $html = '<style>
      h1 {
        color: #4BBDDA;
        font-family: times;
      }
    </style>
    <h1>' . t('Table of contents') . '</h1>';
  $pdf->writeHTML($html);

  // Set the position where the ToC starts so there is space after the heading.
  $pdf->SetY(43);

  // Add a simple table of contents.
  $pdf->addTOC(2, 'helvetica', '.', 'INDEX', 'B', array(128, 0, 0));
  $pdf->endTOCPage();
}