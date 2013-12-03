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

    // REGIONS
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

    // GROUPS
    $group = strtolower($element->element_label);

    // we add a subgroup because elements can have matching searchcodes
    $subgroup = $element->value_title;

    if (!isset($fields[$region][$group][$subgroup])) {
      $fields[$region][$group][$subgroup] = array(
        '#type' => 'fieldset',
        '#collapsible' => FALSE,
        '#collapsed' => FALSE,
        '#tree' => FALSE,
        '#theme' => 'bibdk_search_element',
        '#title_text' => bibdk_custom_search_translate($element->element_label, 'element_label'),
        '#description_text' => bibdk_custom_search_translate($element->description, 'description'),
        '#help_text' => bibdk_custom_search_translate($element->help_text, 'help_text'),
        '#attributes' => array(
          'class' => array($group, 'element-wrapper'),
        ),
      );
    }

    // VALUES
    $value = str_replace('.', '_', $element->search_code);
    if (!isset($fields[$region][$group][$subgroup][$value])) {
      $fields[$region][$group][$subgroup][$value] = array(
        '#type' => $element->value_type,
        '#title' => $element->value_title,
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
        break;
      CASE 'select':
        $fields[$region][$group][$subgroup][$value]['#default_value'] = isset($default_values[$element->search_code]) ? $default_values[$element->search_code] : array($element->default_value);
      CASE 'radios':
        $fields[$region][$group][$subgroup][$value]['#default_value'] = isset($default_values[$element->search_code]) ? array_shift($default_values[$element->search_code]) : NULL;
        if (!$element->option_disabled) {
          $fields[$region][$group][$subgroup][$value]['#options'][$element->value] = bibdk_custom_search_translate($element->label, 'label');
        }
        break;
      CASE 'checkboxes':
        $fields[$region][$group][$subgroup][$value]['#type'] = 'container';
        $fields[$region][$group][$subgroup][$value]['#tree'] = TRUE;
        if(strpos($element->value, 'facet.') !== FALSE ){
          $fields[$region][$group][$subgroup][$value] += bibdk_custom_search_get_list_options($element->value, $element->search_code, $default_values);
        } else {
          $fields[$region][$group][$subgroup][$value] += bibdk_custom_search_create_checkboxes($element, $default_values); //$options, $value, $element->default_value, $element->is_disabled);
        }
        break;
      DEFAULT:
        break;
    }
  }
  return $fields;
}


function bibdk_custom_search_create_checkboxes($element, $default_values) {
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
    $options = _bibdk_custom_search_get_options_db_raw($element->expand);
    foreach ($options as $option) {
      $option = (object)array_merge((array)$element, (array)$option);
      $return[$key . '-fieldset'] += bibdk_custom_search_create_checkboxes($option, $default_values);
    }
  }
  return $return;
}


/**
 * Fills form with parameters from search request.
 *
 * @param array $form
 * @param array $get
 * @param array $extended_fields
 */
function _bibdk_custom_search_get_default_values($group, $value, $defaults) {

  if (!is_array($defaults) || !isset($defaults[$group])){
    return NULL;
  }
  if (is_array($defaults[$group]) && in_array($value, $defaults[$group]) ){
    return $value;
  }
  else {
    return $defaults[$group];
  }


  $used_fields = array();

  if (isset($get['qe'])) { // expanded search elements query elements
    $query_elements = $get['qe'];

    foreach ($extended_fields as $key => $val) {
      if (isset($query_elements[$val['search_code']])) {
        $used_fields[$key] = $extended_fields[$key];
        // assigned by reference needed because it is possible to have more
        // elements with same search code.
        $used_fields[$key]['values'] = & $query_elements[$val['search_code']];
      }
      // empty search_code, and [term]=[value] set defined in value.
      if (isset($query_elements['n/a'])) {
        if (array_key_exists($key, $query_elements['n/a'])) {
          $used_fields[$key] = $extended_fields[$key];
          $used_fields[$key]['values'] = $query_elements['n/a'][$key];
        }
      }
    }
  }
  return $used_fields;
}
