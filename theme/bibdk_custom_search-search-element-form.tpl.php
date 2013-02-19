<?php
/**
 * @file
 * Theme implementation for bib.dk custom search elements.
 */
?>
<div class='fieldset-legend'><?php print $title; ?></div>
<div class="fieldset-description"><?php print $description;?></div>
<p class="helptext" title="<?php print $help; ?>">?</p>
<div class="bibdk-custom-search-element clearfix">
  <?php print drupal_render_children($form); ?>
</div>
