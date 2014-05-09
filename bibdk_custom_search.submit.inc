<?php

/**
 * Process a block search form submission.
 **/
function bibdk_custom_search_get_submitted_searchcodes($get_values) {
  // The search form relies on control of the redirect destination for its
  // functionality, so we override any static destination set in the request,
  // for example by drupal_access_denied() or drupal_not_found()
  // (see http://drupal.org/node/292565).
  if (isset($_GET['destination'])) {
    unset($_GET['destination']);
  }
  dpm($get_values);

  /*$form_id = $form['form_id']['#value'];
  $keys = $get_values[$form_id];

  form_state_values_clean($form_state);

  $q = array();
  if (!empty($keys)) {
    $q[] = $keys;
  }*/

  $q[] = $get_values['search_block_form'];
  $extended_fields = _bibdk_custom_search_get_field_searchcodes();
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

  // Check to see if the form was submitted empty.
  // If it is empty, display an error message.
  // (This method is used instead of setting #required to TRUE for this field
  // because that results in a confusing error message.  It would say a plain
  // "field is required" because the search keywords field has no title.
  // The error message would also complain about a missing #title field.)
  if (sizeof($q) == 0 && sizeof($extended) == 0) {
    form_set_error('keys', t('Please enter some keywords.', array(), array('context' => 'bibdk_custom_search:error')));
    return FALSE;
  }

  if (isset($get_values['delimiter']) && $delimiter = $get_values['delimiter']) {
    $extended['qe']['delimiter'] = $delimiter;
  }

  if (isset($get_values['page_id'])) {
    $extended['page_id'] = $get_values['page_id'];
  }

  $op = _bibdk_custom_search_get_cql('AND');
  $q = join($op, $q);

  $controls = module_invoke_all('ting_search_get_controls', $get_values);

  $query = $controls + $extended;

  $search_info = array();
  $request_path = arg();

  if ($request_path[0] != 'search' || !isset($request_path[1])) {
    $search_info = search_get_default_module_info();
  }
  else {
    foreach (search_get_info() as $search_engine) {
      if ($search_engine['path'] == $request_path[1]) {
        $search_info = $search_engine;
        break;
      }
    }
  }
  if (!empty($search_info['path']) && in_array($search_info['module'], variable_get('search_active_modules', array()))) {
    $url = 'search/' . $search_info['path'] . '/' . trim($q);
    return $query;
    drupal_goto($url, array('query' => $query, 'fragment' => 'content'));

  }
  else {
    form_set_error('keys', t('Search is currently disabled.', array(), array('context' => 'bibdk_custom_search:error')), 'error');
  }
}
