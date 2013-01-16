<?php

/**
 * @file
 * Default theme implementation to configure pages.
 */

$form['page_title']['#title'] .= ' <span class="helptext" title="' . htmlspecialchars(strip_tags($form['page_title']['#description'])) . '">?</span>';
unset($form['page_title']['#description']);

$form['menu_title']['#title'] .= ' <span class="helptext" title="' . htmlspecialchars(strip_tags($form['menu_title']['#description'])) . '">?</span>';
unset($form['menu_title']['#description']);

$form['page_path']['#title'] .= ' <span class="helptext" title="' . htmlspecialchars(strip_tags($form['page_path']['#description'])) . '">?</span>';
unset($form['page_path']['#description']);

$form['delimiter']['#title'] .= ' <span class="helptext" title="' . htmlspecialchars(strip_tags($form['delimiter']['#description'])) . '">?</span>';
unset($form['delimiter']['#description']);

$form['expand']['#title'] .= ' <span class="helptext" title="' . htmlspecialchars(strip_tags($form['expand']['#description'])) . '">?</span>';
unset($form['expand']['#description']);


?>
<?php print drupal_render($form['p_uuid']); ?>
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