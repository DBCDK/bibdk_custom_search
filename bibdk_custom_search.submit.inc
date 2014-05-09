<?php

/**
 * Process a block search form submission.
 **/
function bibdk_custom_search_get_submitted_searchcodes($get_values) {
  $extended_fields = _bibdk_custom_search_get_field_searchcodes();
  $extended = _bibdk_custom_search_get_advanced_values($get_values, $extended_fields);
  $controls = module_invoke_all('ting_search_get_controls', $get_values);

  $query = $controls + $extended;

  return $query;
}


function _bibdk_custom_search_get_advanced_values($get_values, $extended_fields) {
  $extended = array();

  foreach ($get_values as $_key => $values) {
    $key = str_replace('_', '.', $_key);
    $no_searchcode = preg_match('@n/a@', $key);
    if (!isset($extended_fields[$key]['search_code']) && !$no_searchcode) {
      continue;
    }
    if (!is_array($values)) {
      $values = array($values);
    }
    foreach ($values as $value) {

      if ($value) {
        if ($no_searchcode) {
          $extended['qe']['n/a'][$key][] = _translate_boolean($value);
        }
        else {
          $extended['qe'][$key][] = _translate_boolean($value);
        }

      }
    }
  }

  if (isset($get_values['delimiter']) && $delimiter = $get_values['delimiter']) {
    $extended['qe']['delimiter'] = $delimiter;
  }

  if (isset($get_values['page_id'])) {
    $extended['page_id'] = $get_values['page_id'];
  }

  return $extended;
}
