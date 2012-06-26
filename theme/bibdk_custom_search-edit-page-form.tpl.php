<?php

/**
 * @file
 * Default theme implementation to configure pages.
 */

?>
<?php print drupal_render($form['pid']); ?>
<?php print drupal_render($form['sort']); ?>
<table id="edit-search-page">
  <tbody>
    <tr>
      <td><?php print drupal_render($form['page_title']); ?></td>
      <td><?php print drupal_render($form['menu_title']); ?></td>
      <td><?php print drupal_render($form['page_path']); ?></td>
      <td><?php print drupal_render($form['delimiter']); ?></td>
      <td><?php print drupal_render($form['expand']); ?></td>
    </tr>
  </tbody>
</table>