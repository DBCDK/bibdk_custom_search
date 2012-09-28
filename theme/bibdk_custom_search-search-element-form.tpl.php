<?php

/**
 * @file
 * Theme implementation for bib.dk custom search elements.
 */

?>

<?php if ( $form['#help_text'] ) { print '<p class="helptext" title="' . $form['#help_text'] . '">?</p>'; } ?>

<div class="bibdk-custom-search-element">
<?php print drupal_render_children($form); ?>

</div>