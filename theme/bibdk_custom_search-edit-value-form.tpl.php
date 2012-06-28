<?php

/**
 * @file
 * Default theme implementation to configure search page element.
 */

drupal_add_js(drupal_get_path('module', 'bibdk_custom_search') . '/js/bibdk_custom_search_value.js');

?>
<?php print drupal_render($form['vid']); ?>
<?php print drupal_render($form['sort']); ?>
<table id="edit-search-page-value">
  <tbody>
    <tr>
      <td><?php print drupal_render($form['value_title']); ?></td>
      <td><?php print drupal_render($form['value_name']); ?></td>
      <td><?php print drupal_render($form['type']); ?></td>
      <td><?php print drupal_render($form['search_code']); ?></td>
      <td><?php print drupal_render($form['default_value']); ?></td>
    </tr>
  </tbody>
</table>
