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

    if (empty($element->search_code)){
      $element->search_code = 'n/a' . strtolower($element->element_label);
    }

    _bibdk_custom_search_translate_element($element);

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
    _bibdk_custom_search_add_element($element, $fields[$region][$group][$subgroup], $default_values);

  }
  return $fields;
}

function _bibdk_custom_search_translate_element(&$element){
  // translate all values
  $element->value = _translate_boolean($element->value);

  // Translate all labels, descriptions and help-texts
  $element->element_label = bibdk_custom_search_translate($element->element_label, 'element_label');
  $element->description = bibdk_custom_search_translate($element->description, 'description');
  $element->help_text = bibdk_custom_search_translate($element->help_text, 'help_text');

}


/** Add single field to fields array
 * @param $element
 * @param $field
 * @param $default_values
 */
function _bibdk_custom_search_add_element($element, &$field, $default_values){
  $value = str_replace('.', '_', $element->search_code);
  if (!isset($field[$value])) {
    $field[$value] = array(
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
      $field[$value]['#size'] = 30;
    CASE 'textfield':
      $field[$value]['#maxlength'] = 64;
      $field[$value]['#default_value'] = isset($default_values[$element->search_code]) ? array_shift($default_values[$element->search_code]) : NULL;
      $field[$value]['#parents'] = array($value, $element->value_title);
      $field[$value]['#tree'] = FALSE;
      break;
    CASE 'select':
    CASE 'radios':
      $field[$value]['#default_value'] = isset($default_values[$element->search_code][0]) ? $default_values[$element->search_code][0] : $element->default_value;
      if (!$element->option_disabled) {
        $field[$value]['#options'][$element->value] = $element->label;
      }
      break;
    CASE 'checkboxes':
      $field[$value]['#type'] = 'container';
      $field[$value]['#tree'] = TRUE;
      $checkbox_values = isset($field[$value]['#values']) ? $field[$value]['#values'] : array();
      // If value includes a facet get a list of options
      if(strpos($element->value, 'facet.') !== FALSE ){
        $checkboxes = bibdk_custom_search_get_list_options($element->value, $element->search_code, $default_values, $checkbox_values);
        $field[$value] += $checkboxes;
      } else {
        $options = $element->expand ? _bibdk_custom_search_get_options_db_raw($element->expand) : array();
        $checkboxes = bibdk_custom_search_create_checkboxes($element, $default_values, $options);
        $field[$value] +=  $checkboxes['elements'];
        $field[$value]['#values'] =  array_merge((array)$checkbox_values, $checkboxes['values']);
      }
      break;
    DEFAULT:
      break;
  }

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
    '#parents' => array($parent, $element->value),
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
