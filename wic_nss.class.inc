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

  }

  /**
   * Customize the PDF footer.
   */
  public function Footer() {
    // Set the position of the footer.
    $this->SetY(-20);
    $this->SetTextColor(128, 128, 128);
    // Print the document name and page number.
    $this->Cell(170, 10, t('WIC NUTRITION SERVICES STANDARDS SELF-ASSESSMENT'), '', 0, 'R');
    $this->Cell(15, 10, $this->PageNo() + $this->page_offset, '', 0, 'R');
  }
}
