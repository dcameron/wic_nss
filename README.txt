
CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installation

INTRODUCTION
------------

Current maintainer: David Cameron
Original author: David Cameron <david.a.cameron@sbcglobal.net>

The WIC NSS Self-Assessment module was created for the WICWorks office at NAL.
This module extends the Book and Webform modules to create a self-assessment
application for self-improvement of WIC clinics and agencies.  It provides
progress tracking via a dashboard page, HTML and PDF report generation, a custom
webform component for setting improvement goals, and email reminders about those
goals.

The dependency on the "clone" module requres explanation.  clone is the
machine name for the Node Clone module, http://drupal.org/project/node_clone.
This name discrepancy causes issues when trying to use it as a dependency.  The
Node Clone module is optional.  It does not have to be enabled for the
Self-Assessment module to work correctly.  Node Clone is an excellent tool for
templating and generating new forms quickly, which is required by the NSS
project.

INSTALLATION
------------

Download and install the Node Clone module from
http://drupal.org/project/node_clone.

Install as usual, see http://drupal.org/node/895232 for further information.
