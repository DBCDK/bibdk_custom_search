<?php

/**
 * @file
 * Theme implementation for bib.dk custom search elements.
 */
// dpm($form);
?>

<div class="bibdk-custom-search-element">

<?php if ( $form['#help_text'] ) { print '<p class="helptext" title="' . $form['#help_text'] . '">?</p>'; } ?>

<?php print drupal_render_children($form); ?>

</div>