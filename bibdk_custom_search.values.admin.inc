<?php

/**
 * @file
 * Admin settings for bibdk search pages
 */

/**
 * Includes
 */
module_load_include('inc', 'bibdk_custom_search', 'includes/bibdk_custom_search-values-forms');


/**
 * Search values list form.
 */
function bibdk_custom_search_values_admin() {

  $query = db_select('bibdk_custom_search_values', 'v');
  $query
    ->fields('v', array('vid','value_title','sort'))
    ->orderBy('value_title', 'DESC')
    ->orderBy('sort', 'DESC');
  $result = $query->execute();

  $values = array();

  /* example
    $values = array(
      '0' => array('value_title' => t('Materialer, musik'), 'vid' => '0', 'sort' => -1),
      '1' => array('value_title' => t('Inputboks, forfatter'), 'vid' => '1', 'delete' => '1', 'sort' => 1),
      '2' => array('value_title' => t('&Aring;r, afgr&aelig;nsning'), 'vid' => '2', 'sort' => 2),
      '3' => array('value_title' => t('&Aring;rstal'), 'vid' => '3', 'sort' => 3),
    );
  */

  foreach ($result as $record) {
    $row = array(
      'vid' => check_plain($record->vid),
      'value_title' => check_plain($record->value_title),
      'sort' => check_plain($record->sort),
    );
    $values[] = $row;
  }

  $form = _bibdk_custom_search_values_admin_form($values);
  $form['#submit'][] = '_bibdk_custom_search_values_admin_submit';
  return $form;
}

/**
 * Submit handler for bibdk_custom_search_values_admin();
 */
function _bibdk_custom_search_values_admin_submit($form, &$form_state) {

  $values = $form_state['values']['bibdk_custom_search'];

  foreach ($values as $key => $value) {
    if ($value['vid']=='_new' && !empty($value['value_title'])) {
      _bibdk_custom_search_values_admin_insert($value);
    }
    if ($value['vid']!='_new') {
      _bibdk_custom_search_values_admin_update($value);
    }
  }
  cache_clear_all('bibdk_custom_search', 'cache', TRUE);
  drupal_set_message(t('The search value configuration have been saved.'));
}


/**
 * Search values list values select.
 */
/*
function _bibdk_custom_search_values_admin_select() {
  $query = db_select('bibdk_custom_search_values', 'v');
  $query
    ->fields('v', array('vid','value_title','sort'))
    ->orderBy('value_title', 'DESC')
    ->orderBy('sort', 'DESC');
  $result = $query->execute();
  foreach ($result as $record) {
    $row = array(
      'vid' => check_plain($record->vid),
      'value_title' => check_plain($record->value_title),
      'sort' => check_plain($record->sort),
    );
    $rows[] = $row;
  }
  return $rows;
}
*/

/**
 * Search values list values insert.
 */
function _bibdk_custom_search_values_admin_insert($value) {
  $result = db_insert('bibdk_custom_search_values')
    ->fields(array(
      'value_title' => check_plain($value['value_title']),
      'sort' => (int)$value['sort'],
    ))
    ->execute();
  return $result;
}

/**
 * Search values list values update.
 */
function _bibdk_custom_search_values_admin_update($value) {
  $result = db_update('bibdk_custom_search_values')
    ->fields(array(
      'sort' => (int)$value['sort'],
    ))
    ->condition('vid', $value['vid'])
    ->execute();
  return $result;
}



/**
 * Edit search values settings.
 */
function bibdk_custom_search_value_edit($form_id, $form_state, $value_id, $option_id = NULL) {
  $value = array(
    'vid'             => '2',
    'value_title'     => 'Materialetyper, musik',
    'type'            => 'checkbox',
    'search_code'     => 'dc.title', // NULL if textfield
    'default_value'   => '',  // NULL if not textfield
    'default_text'    => '',  // NULL if not textfield
    'maximum_length'  => '128',
    'expand'          => '10',
    'options' => array(
      '0' => array('oid' => '1', 'label' => t('Alle typer artikler'), 'value' => 'kat=ana', 'sort' => 1),
      '1' => array('oid' => '2', 'label' => t('Avisartikler'), 'value' => 'ma=ai', 'sort' => 2),
      '2' => array('oid' => '3', 'label' => t('Tidsskriftartikel'), 'value' => 'ma=ap', 'sort' => 3),
      '3' => array('oid' => '4', 'label' => t('Netdokumenter'), 'value' => 'ma=xe og (ma=ai eller ma=ap)', 'sort' => 4),
    ),
  );

  if ( $option_id ) {
    $result = db_delete('bibdk_custom_search_options')
      ->condition('vid', (int)$value_id)
      ->condition('oid', (int)$option_id)
      ->execute();
    drupal_set_message( t("Option deleted.") );
  }

  $value = db_select('bibdk_custom_search_values', 'v')
    ->fields('v')
    ->condition('vid', $value_id,'=')
    ->execute()
    ->fetchAssoc();

  $list_values[0] = t('Select submenu values');

  $query = db_select('bibdk_custom_search_values', 'v');
  $query
    ->fields('v', array('vid','value_title','sort'))
    ->orderBy('sort', 'DESC');
  $result = $query->execute();

  foreach ($result as $row) {
    $vid = check_plain($row->vid);
    $value_title = check_plain($row->value_title);
    $list_values[$vid] = $value_title;
  }

  $options = array();

  $query = db_select('bibdk_custom_search_options', 'o');
  $query
    ->fields('o', array('oid','label','value','expand','sort'))
    ->condition('vid', $value_id,'=')
    ->orderBy('sort', 'DESC');
  $result = $query->execute();

  foreach ($result as $option) {
    $row = array(
      'oid' => check_plain($option->oid),
      'label' => check_plain($option->label),
      'value' => ( $option->expand ) ? check_plain($list_values[$option->expand]) : check_plain($option->value),
      'expand' => check_plain($option->expand),
      'sort' => check_plain($option->sort),
    );
    $options[] = $row;
  }
  $value['options'] = $options;

  $form = _bibdk_custom_search_edit_value_form($value,$list_values);
  $form['#action'] = url('admin/config/search/bibdk_custom_search/values/' . $value_id . '/edit');
  $form['#submit'][] = 'bibdk_custom_search_value_edit_submit';
  return $form;
}

/**
 *
 */
function bibdk_custom_search_value_edit_submit($form, &$form_state) {
  $element_value = $form_state['values']['element_value'];
  $value_id = (int)$form_state['values']['element_value']['vid'];
  $result = db_update('bibdk_custom_search_values')
    ->fields(array(
      'value_title' => check_plain($element_value['value_title']),
      'type' => check_plain($element_value['type']),
      'search_code' => check_plain($element_value['search_code']),
      'default_value' => check_plain($element_value['default_value']),
    ))
    ->condition('vid', (int)$element_value['vid'])
    ->execute();

  $element_value_options = $form_state['values']['element_value_options'];
  foreach ($element_value_options as $key => $value_option) {
    if ( ( $value_option['oid']=='_new' && !empty($value_option['value']) ) ||
         ( $value_option['oid']=='_submenu' && !empty($value_option['expand']) ) ) {
      _bibdk_custom_search_value_options_insert($value_id,$value_option);
    } else {
      _bibdk_custom_search_values_options_update($value_id,$value_option);
    }
  }
  cache_clear_all('bibdk_custom_search', 'cache', TRUE);
  $form_state['redirect'] = 'admin/config/search/bibdk_custom_search/values/' . $value_id . '/edit';
}

/**
 * Search values list option insert.
 */
function _bibdk_custom_search_value_options_insert($value_id,$value_option) {
  $result = FALSE;
  $result = db_insert('bibdk_custom_search_options')
    ->fields(array(
      'vid'     => $value_id,
      'label'   => check_plain($value_option['label']),
      'value'   => check_plain($value_option['value']),
      'expand'  => (int)$value_option['expand'],
      'sort'    => (int)$value_option['sort'],
    ))
    ->execute();
  return $result;
}

/**
 * Search values list option update.
 */
function _bibdk_custom_search_values_options_update($value_id,$value_option) {
  $result = FALSE;
  $result = db_update('bibdk_custom_search_options')
    ->fields(array(
      'label' => check_plain($value_option['label']),
      'value' => check_plain($value_option['value']),
      'expand' => check_plain($value_option['expand']),
      'sort' => (int)$value_option['sort'],
    ))
    ->condition('oid', (int)$value_option['oid'])
    ->condition('vid', $value_id)
    ->execute();
  return $result;
}


/**
 * bibdk_custom_search_value_delete confirmation form;
 */
function bibdk_custom_search_value_delete($form_id, $form_state, $value_id) {
  _bibdk_custom_search_element_validate_element_pointers($value_id, &$form);
  $result = db_select('bibdk_custom_search_values', 'v')
    ->fields('v',array('value_title'))
    ->condition('vid', $value_id,'=')
    ->execute()
    ->fetchAssoc();
  $form['vid'] = array(
    '#type' => 'value',
    '#value' => $value_id,
  );
  $form['value_title'] = array(
    '#type' => 'hidden',
    '#value' => $result['value_title'],
  );
  $form['#submit'][] = '_bibdk_custom_search_value_delete_submit';
  $form['#validate'][] = '_bibdk_custom_search_value_delete_validate';
  return confirm_form($form, t("Are you sure you want to delete '@value'?", array('@value' => $result['value_title'])), 'admin/config/search/bibdk_custom_search/values');
}

/**
 * Validation handler for bibdk_custom_search_value_delete();
 */
function _bibdk_custom_search_value_delete_validate($form, &$form_state) {
  _bibdk_custom_search_element_validate_element_pointers($form_state['values']['vid'], &$form);
}

/**
 * Validate element pointers to an value ID.
 */
function _bibdk_custom_search_element_validate_element_pointers($vid, &$form) {
  $query = db_select('bibdk_custom_search_elements', 'e');
  $query -> join('bibdk_custom_search_map_values', 'm', 'm.eid = e.eid');
  $query -> join('bibdk_custom_search_values', 'v', 'm.vid = v.vid');
  $query -> fields('e', array('element_title'));
  $query -> fields('m', array('eid','vid'));
  $query -> fields('v', array('value_title'));
  $query -> condition('m.vid', $vid,'=');
  $result = $query -> execute();
  $element_title = '';
  $element_titles = '';
  $value_title = '';
  foreach ($result as $record) {
    $value_title = t(check_plain($record->value_title));
    if ( $element_titles ) {
      $element_titles .= ', ';
    }
    $element_titles .= $element_title;
    $element_title = '"' . t(check_plain($record->element_title)) . '"';
  }
  if ( $element_titles ) {
    $element_titles .= ' & ';
  }
  $element_titles .= $element_title;

  if ( $element_titles ) {
    form_set_error('vid',
      t('The value "%value_title" can\'t be deleted. It is used in the page element: %element_titles.',
        array(
          '%value_title' => $value_title,
          '%element_titles' => $element_titles,
        )));
  }
}

/**
 * Submit handler for bibdk_custom_search_value_delete();
 */
function _bibdk_custom_search_value_delete_submit($form, &$form_state) {
  $result = db_delete('bibdk_custom_search_options')
    ->condition('vid', $form_state['values']['vid'])
    ->execute();
  $result = db_delete('bibdk_custom_search_values')
    ->condition('vid', $form_state['values']['vid'])
    ->execute();
  drupal_set_message(t("'@value' deleted.", array('@value' => $form_state['values']['value_title'])));
  $form_state['redirect'] = 'admin/config/search/bibdk_custom_search/values';
}

