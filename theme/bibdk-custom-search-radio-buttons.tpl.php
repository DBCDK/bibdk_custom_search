<?php

/*
See: function template_preprocess_bibdk_custom_search_radio_buttons() in template.php
$first = $form['#data_toggle_value'][0];
$last  = end($form['#data_toggle_value']);
$form[$first]['#prefix'] = '<fieldset id="edit-term-' . $form['#data_toggle_key'] . '" class="sub-elements form-wrapper" data-child="' . $form['#data_toggle_key'] . '" style="display: block;">';
$form[$last]['#suffix'] = '</fieldset>';
*/

echo drupal_render_children($form);

?>
