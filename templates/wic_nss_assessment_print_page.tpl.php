<?php

/**
 * @file
 * Display all submissions in a self-assessment.
 *
 * Available variables:
 * - $bid: The nid of the assessment book node.
 * - $submission_content: An array containing the following keys:
 *   - title: The title of the submitted webform.
 *   - content: The rendered content of the submission.
 */
?>

<?php if (empty($submission_content)): ?>
  <p><?php print t("You haven't submitted any forms for this self-assessment yet."); ?></p>
  <p><?php print l(t('Start filling out forms.'), 'node/' . $bid); ?></p>

<?php else: ?>
  <?php foreach ($submission_content as $submission): ?>
    <div class="wic-nss-webform-submission">
      <h2><?php print $submission['title']; ?></h2>
      <?php print render($submission['content']); ?>
    </div>
  <?php endforeach; ?>
<?php endif; ?>