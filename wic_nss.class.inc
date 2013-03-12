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

  }
}
