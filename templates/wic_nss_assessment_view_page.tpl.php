<?php

/**
 * @file
 * Display all submissions in a self-assessment.
 *
 * Available variables:
 * - $bid: The nid of the assessment book node.
 * - $table_of_contents: A jump menu list of links to the submissions.
 * - $content: An array containing content from nodes in the assessment.
 */
?>

<div class="wic-nss-submissions-toc">
  <h2>Table of contents</h2>
  <?php print $table_of_contents; ?>
</div>
<?php foreach ($content as $nid => $node_content): ?>
  <div id="<?php print 'node-' . $nid; ?>" class="wic-nss-node-display">
    <h2><?php print $node_content['title']; ?></h2>
    <?php print render($node_content['body']); ?>
    <?php if (!empty($node_content['submission'])): ?>
      <?php print render($node_content['submission']); ?>
    <?php endif; ?>
  </div>
<?php endforeach; ?>
