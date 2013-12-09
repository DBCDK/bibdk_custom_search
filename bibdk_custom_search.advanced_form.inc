<?php

/** Generates the form for advanced search pages.
 *
 * @param $elements
 * @param $current_page
 * @param $default_values
 * @param $extended_fields
 */
function _bibdk_custom_search_create_advanced_form($elements, $current_page, $default_values) {
  $fields = array();

  // We loop through all search elements
  foreach ($elements as $element) {


    // Explode elements with key-value pairs in value and empty search_code
    if (empty($element->search_code) && strpos($element->value, '=')){
      $exploded = explode('=', $element->value);
      $element->search_code = $exploded[0];
      $element->value = $exploded[1];
    }

    // translate all values
    $element->value = _translate_boolean($element->value);

    // Translate all labels, descriptions and help-texts
    $element->element_label = bibdk_custom_search_translate($element->element_label, 'element_label');
    $element->description = bibdk_custom_search_translate($element->description, 'description');
    $element->help_text = bibdk_custom_search_translate($element->help_text, 'help_text');



    /* REGIONS */
    $region = $element->region;
    if (!isset($fields[$region]) && $region != 'main') {
      $fields[$region] = array(
        '#type' => 'fieldset',
        '#title' => t('Advanced search'),
        '#collapsible' => TRUE,
        '#collapsed' => ($current_page['expand']) ? $current_page['expand'] : FALSE,
        '#weight' => 102,
      );
    }

    /* GROUPS */
    $group = strtolower($element->element_label);

    // we add a subgroup because elements can have matching searchcodes
    $subgroup = $element->e_uuid;

    if (!isset($fields[$region][$group][$subgroup])) {
      $fields[$region][$group][$subgroup] = array(
        '#type' => 'fieldset',
        '#collapsible' => FALSE,
        '#collapsed' => FALSE,
        '#tree' => FALSE,
        '#theme' => 'bibdk_search_element',
        '#title_text' => $element->element_label,
        '#description_text' => $element->description,
        '#help_text' => $element->help_text,
        '#attributes' => array(
          'class' => array(_bibdk_custom_search_machine_readable($group), 'element-wrapper'),
        ),
      );
    }

    /* VALUES */
    $value = str_replace('.', '_', $element->search_code);
    if (!isset($fields[$region][$group][$subgroup][$value])) {
      $fields[$region][$group][$subgroup][$value] = array(
        '#type' => $element->value_type,
        '#title' => $element->value_title,
        '#title_display' => 'invisible',
        '#disabled' => $element->is_disabled || $element->value_disabled,
        '#attributes' => array(
          'tabindex' => $element->tab_index,
          'accesskey' => $element->access_key,
          'data-term' => array($value),
        ),
      );
    }

    // Add type specific settings for values
    switch ($element->value_type) {
      CASE 'textarea':
        $fields[$region][$group][$subgroup][$value]['#size'] = 30;
      CASE 'textfield':
        $fields[$region][$group][$subgroup][$value]['#maxlength'] = 64;
        $fields[$region][$group][$subgroup][$value]['#default_value'] = isset($default_values[$element->search_code]) ? array_shift($default_values[$element->search_code]) : NULL;
        $fields[$region][$group][$subgroup][$value]['#parents'] = array($value, $element->value_title);
        $fields[$region][$group][$subgroup][$value]['#tree'] = FALSE;
        break;
      CASE 'select':
      CASE 'radios':
        $fields[$region][$group][$subgroup][$value]['#default_value'] = isset($default_values[$element->search_code][0]) ? $default_values[$element->search_code][0] : $element->default_value;
        if (!$element->option_disabled) {
          $fields[$region][$group][$subgroup][$value]['#options'][$element->value] = bibdk_custom_search_translate($element->label, 'label');
        }
        break;
      CASE 'checkboxes':
        $fields[$region][$group][$subgroup][$value]['#type'] = 'container';
        $fields[$region][$group][$subgroup][$value]['#tree'] = TRUE;
        $checkbox_values = isset($fields[$region][$group][$subgroup][$value]['#values']) ? $fields[$region][$group][$subgroup][$value]['#values'] : array();
        if(strpos($element->value, 'facet.') !== FALSE ){
          $fields[$region][$group][$subgroup][$value] += bibdk_custom_search_get_list_options($element->value, $element->search_code, $default_values, $checkbox_values);
        } else {
          $options = $element->expand ? _bibdk_custom_search_get_options_db_raw($element->expand) : array();
          $checkboxes = bibdk_custom_search_create_checkboxes($element, $default_values, $options);
          $fields[$region][$group][$subgroup][$value] +=  $checkboxes['elements'];
          $fields[$region][$group][$subgroup][$value]['#values'] =  array_merge((array)$checkbox_values, $checkboxes['values']);
        }
        break;
      DEFAULT:
        break;
    }
  }
  return $fields;
}

/** Generate checkboxes for advanced search elements. Function works recursively
 * if a checkbox is expanded and has subelements
 * @param $element
 * @param $default_values
 * @param array $options
 * @return array
 */
function bibdk_custom_search_create_checkboxes($element, $default_values, $options = array()) {
  $parent = str_replace('.', '_', $element->search_code);
  // Get default values

  // TODO : Simplefy
  $default = ($element->default_value == $element->value) ? $element->default_value : NULL;
  $default = isset($default_values[$element->search_code]) && is_array($default_values[$element->search_code]) && in_array($element->value, $default_values[$element->search_code]) ? $element->value : $default;


  $return[$element->value] = array(
    '#type' => 'checkbox',
    '#default_value' => $default,
    '#disabled' => ($element->is_disabled || $element->option_disabled) ? TRUE : FALSE,
    '#title' => $element->label,
    '#return_value' => $element->value,
    //'#parents' => array($parent),
    '#attributes' => array(
      'data-group' => $parent,
      'class' => array(($element->default_value == $element->value) ? 'default-value' : ''),
    ),
  );
  $values = array($element->value);
  // Create nested checkbox options
  if ($element->expand) {
    $key = $element->value;
    $return[$element->value]['#attributes'] += array(
      'data-parent' => $parent,
    );
    $return[$element->value]['#title'] = '<span data-child="' . $key . '" class="toggle-subgroup"> + </span>' . $return[$element->value]['#title'];
    $return[$key . '-fieldset'] = array(
      '#type' => 'fieldset',
      '#attributes' => array(
        'data-child' => $key,
        'class' => array('sub-elements'),
      ),
      '#parents' => array($parent),
    );
    foreach ($options as $option) {
      $option = (object)array_merge((array)$element, (array)$option);
      $option_elements = bibdk_custom_search_create_checkboxes($option, $default_values);
      $return[$key . '-fieldset'] += $option_elements['elements'];
      $values = array_merge($values , $option_elements['values']);
    }
  }
  return array('elements' => $return, 'values' => $values);
}
