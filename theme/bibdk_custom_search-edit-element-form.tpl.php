<?php

/**
 * @file
 * Default theme implementation to configure search page element.
 */

?>
<?php print drupal_render($form['id']); ?>
<?php print drupal_render($form['sort']); ?>
<table id="edit-search-page-element">
  <tbody>
    <tr>
      <td><?php print drupal_render($form['element_title']); ?></td>
      <td><?php print drupal_render($form['element_label']); ?></td>
      <td><?php print drupal_render($form['access_key']); ?></td>
      <td><?php print drupal_render($form['tab_index']); ?></td>
    </tr>
    <tr>
      <td><?php print drupal_render($form['description']); ?></td>
      <td colspan="3"><?php print drupal_render($form['help_text']); ?></td>
    </tr>
  </tbody>
</table>
