<?php

/**
 * @file
 * Extend the TCPDF class to be able to customize the Header and Footer
 *
 * This file is included by the _print_pdf_tcpdf function
 */

class WICNSS extends TCPDF {
  public $page_offset = 1;

  /**
   * Customize the PDF header.
   */
  public function Header() {
    // Print the green background.
    $this->Image(dirname(__FILE__) . '/images/header-bg.jpg', 5, 5, '', '', 'JPG', '', '', false, 300, '', false, false, 0);

  }

  /**
   * Customize the PDF footer.
   */
  public function Footer() {
    // Set the position of the footer.
    $this->SetY(-20);
    // Print the green dot behind the page number.
    $this->Image(dirname(__FILE__) . '/images/footer-bg.jpg', 194, 262, '', '', 'JPG', '', '', false, 300, '', false, false, 0);
    // Print the document name and page number.
    $this->Cell(170, 15, t('WIC NUTRITION SERVICES STANDARDS SELF-ASSESSMENT'), '', 0, 'R');
    $this->SetTextColor(255, 255, 255);
    $this->Cell(15, 15, $this->PageNo() + $this->page_offset, '', 0, 'R');
  }
}
