<?php

/**
 * @file
 * Admin settings for bibdk search pages
 */

/**
 * Includes
 */
module_load_include('inc', 'bibdk_custom_search', 'includes/bibdk_custom_search-elements-forms');


/**
 * Search elements list.
 */
function bibdk_custom_search_elements_admin() {

  $query = db_select('bibdk_custom_search_elements', 'e');
  $query
    ->fields('e', array('eid','element_title','sort'))
    ->orderBy('element_title', 'DESC')
    ->orderBy('sort', 'DESC');
  $result = $query->execute();

  $elements = array();

  /* example
  $elements = array(
    '0' => array('element_title' => t('Film materialetyper'), 'eid' => '0', 'sort' => -1),
    '1' => array('element_title' => t('Forfatter, inputbox'), 'eid' => '1', 'delete' => '1', 'sort' => 1),
    '2' => array('element_title' => t('Titel, inputbox'), 'eid' => '2', 'sort' => 2),
    '3' => array('element_title' => t('Emne, inputbox'), 'eid' => '3', 'sort' => 3),
  );
  */

  foreach ($result as $record) {
    $row = array(
      'eid' => check_plain($record->eid),
      'element_title' => check_plain($record->element_title),
      'sort' => check_plain($record->sort),
    );
    $elements[] = $row;
  }

  $form = _bibdk_custom_search_elements_admin_form($elements);
  $form['#submit'][] = 'bibdk_custom_search_elements_admin_submit';
  return $form;
}

/**
 * Submit handler for bibdk_custom_search_elements_admin();
 */
function bibdk_custom_search_elements_admin_submit($form, &$form_state) {
  $values = $form_state['values']['bibdk_custom_search_page_elements'];
  foreach ( $values as $key => $value ) {
    if ( $value['eid']=='_new' && !empty($value['element_title']) ) {
      _bibdk_custom_search_elements_admin_insert($value);
    }
    if ( $value['eid'] != '_new' ) {
      _bibdk_custom_search_elements_admin_update($value);
    }
  }
  drupal_set_message(t('The search value configuration have been saved.'));
}

/**
 * Search values list values insert.
 */
function _bibdk_custom_search_elements_admin_insert($value) {
  $result = db_insert('bibdk_custom_search_elements')
    ->fields(array(
      'element_title' => check_plain($value['element_title']),
      'sort' => (int)$value['sort'],
    ))
    ->execute();
  return $result;
}

/**
 * Search values list values update.
 */
function _bibdk_custom_search_elements_admin_update($value) {
  $result = db_update('bibdk_custom_search_elements')
    ->fields(array(
      'sort' => (int)$value['sort'],
    ))
    ->condition('eid', $value['eid'])
    ->execute();
  return $result;
}

/**
 * Delete search element settings.
 */
function bibdk_custom_search_element_delete($form_id, $form_state, $element_id) {

  _bibdk_custom_search_element_validate_page_pointers($element_id, $form);

  $result = db_select('bibdk_custom_search_elements', 'e')
    ->fields('e',array('element_title'))
    ->condition('eid', $element_id,'=')
    ->execute()
    ->fetchAssoc();
  $form['eid'] = array(
    '#type' => 'value',
    '#value' => $element_id,
  );
  $form['element_title'] = array(
    '#type' => 'hidden',
    '#value' => $result['element_title'],
  );
  $form['#validate'][] = 'bibdk_custom_search_element_delete_validate';
  $form['#submit'][] = 'bibdk_custom_search_element_delete_submit';
  return confirm_form($form, t("Are you sure you want to delete '@element'?", array('@element' => $result['element_title'])), 'admin/config/search/bibdk_custom_search/elements');
}

/**
 * Validation handler for bibdk_custom_search_element_delete();
 */
function bibdk_custom_search_element_delete_validate($form, &$form_state) {
  _bibdk_custom_search_element_validate_page_pointers($form_state['values']['eid'], $form);
}

/**
 * Validate page pointers to an element ID.
 */
function _bibdk_custom_search_element_validate_page_pointers($eid, &$form) {
  $query = db_select('bibdk_custom_search', 'p');
  $query -> join('bibdk_custom_search_map_elements', 'm', 'p.pid = m.pid');
  $query -> join('bibdk_custom_search_elements', 'e', 'm.eid = e.eid');
  $query -> fields('p', array('page_title'));
  $query -> fields('e', array('element_title'));
  $query -> fields('m', array('pid','eid'));
  $query -> condition('m.eid', $eid,'=');
  $result = $query -> execute();
  $page_title = '';
  $page_titles = '';
  $element_title = '';
  foreach ($result as $record) {
    $element_title = t(check_plain($record->element_title));
    if ( $page_titles ) {
      $page_titles .= ', ';
    }
    $page_titles .= $page_title;
    $page_title = '"' . t(check_plain($record->page_title)) . '"';
  }
  if ( $page_titles ) {
    $page_titles .= ' & ';
  }
  $page_titles .= $page_title;

  if ( $page_titles ) {
    form_set_error('pid',
      t('The value "%element_title" can\'t be deleted. It is used in the page: %page_titles.',
        array(
          '%element_title' => $element_title,
          '%page_titles' => $page_titles,
        )));
  }
}

/**
 * Submit handler for bibdk_custom_search_element_delete();
 */
function bibdk_custom_search_element_delete_submit($form, &$form_state) {
  $result = db_delete('bibdk_custom_search_map_values')
    ->condition('eid', (int)$form_state['values']['eid'])
    ->execute();
  $result = db_delete('bibdk_custom_search_elements')
    ->condition('eid', (int)$form_state['values']['eid'])
    ->execute();
  drupal_set_message(t("'@element' deleted.", array('@element' => $form_state['values']['element_title'])));
  $form_state['redirect'] = 'admin/config/search/bibdk_custom_search/elements';
}



/**
 * Edit search element settings.
 */
function bibdk_custom_search_element_edit($form_id, $form_state, $element_id, $value_pointer=FALSE) {

  if ( $value_pointer ) {
    $result = db_delete('bibdk_custom_search_map_values')
      ->condition('mid', (int)$value_pointer)
      ->execute();
    drupal_set_message( t("Value removed.") );
  }

  // bibdk_custom_search_elements
  $element = db_select('bibdk_custom_search_elements', 'e')
    ->fields('e')
    ->condition('eid', $element_id,'=')
    ->execute()
    ->fetchAssoc();

  // bibdk_custom_search_values
  $list_values[0] = 'Select values';
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

  // bibdk_custom_search_map_values
  $query = db_select('bibdk_custom_search_map_values', 'm');
  $query -> join('bibdk_custom_search_values', 'v', 'm.vid = v.vid');
  $query -> fields('m', array('mid','vid','sort'));
  $query -> fields('v', array('value_title'));
  $query -> condition('eid', $element_id,'=');
  $query -> orderBy('sort', 'DESC');
  $result = $query -> execute();
  $element_values = array();
  foreach ($result as $record) {
    $row = array(
      'mid' => check_plain($record->mid),
      'vid' => check_plain($record->vid),
      'value_title' => check_plain($record->value_title),
      'sort' => check_plain($record->sort),
    );
    $element_values[] = $row;
  }
  $element['values'] = $element_values;

/*
  $element = array(
    'eid'           => '2',
    'sort'          => '2',
    'element_title' => 'Film materialetyper',
    'element_label' => 'Materialetyper',
    'description'   => 'F.eks.: varm luft',
    'help_text'     => 'Skriv den eller de dele af navnet, du er sikker på.
                        Kombiner evt. med et ord fra titlen - det skal skrives i titelboksen.',
    'access_key'    => 'm',
    'tab_index'     => '10',
    'values' => array(
      '0' => array('value_title' => t('Forfatter, inputbox'), 'mid' => '1', 'vid' => '0', 'sort' => -1),
      '1' => array('value_title' => t('Titel, inputbox'), 'mid' => '1', 'vid' => '1', 'delete' => '1', 'sort' => 1),
      '2' => array('value_title' => t('Emne, inputbox'), 'mid' => '1', 'vid' => '2', 'sort' => 2),
      '3' => array('value_title' => t('Fritekst, inputbox'), 'mid' => '1', 'vid' => '3', 'sort' => 3),
      '4' => array('value_title' => t('Materialetype - B&oslash;ger, checkboxes'), 'mid' => '1', 'vid' => '4', 'sort' => 4),
      '5' => array('value_title' => t('Litter&aelig;r form, checkboxes'), 'mid' => '1', 'vid' => '5', 'sort' => 5),
      '6' => array('value_title' => t('Sprog - kort, checkboxes'), 'mid' => '1', 'vid' => '6', 'sort' => 6),
      '7' => array('value_title' => t('&Aring;r'), 'mid' => '1', 'vid' => '7', 'sort' => 7),
    ),
  );
*/

  $form = _bibdk_custom_search_edit_element_form($element,$list_values);
  $form['#action'] = url('admin/config/search/bibdk_custom_search/elements/' . $element_id . '/edit');
  $form['#submit'][] = 'bibdk_custom_search_element_edit_submit';
  return $form;
}

/**
 *
 */
function bibdk_custom_search_element_edit_submit($form, &$form_state) {

  $element = $form_state['values']['bibdk_custom_search_page_element'];
  $element_id = (int)$form_state['values']['bibdk_custom_search_page_element']['eid'];

  $result = db_update('bibdk_custom_search_elements')
    ->fields(array(
      'element_title' => check_plain($element['element_title']),
      'element_label' => check_plain($element['element_label']),
      'description' => check_plain($element['description']),
      'help_text' => check_plain($element['help_text']),
      'tab_index' => check_plain($element['tab_index']),
      'access_key' => check_plain($element['access_key']),
    ))
    ->condition('eid', (int)$element['eid'])
    ->execute();

  $element_values = $form_state['values']['bibdk_custom_search_page_element_values'];
  foreach ($element_values as $key => $element_value) {
    if ( $element_value['mid']=='_new' && !empty($element_value['vid']) ) {
      _bibdk_custom_search_element_value_insert($element_id,$element_value);
    } else {
      _bibdk_custom_search_element_values_update($element_id,$element_value);
    }
  }

  $form_state['redirect'] = 'admin/config/search/bibdk_custom_search/elements/' . $element_id . '/edit';
}

/**
 * Search element value insert.
 */
function _bibdk_custom_search_element_value_insert($element_id,$element_value) {
  $result = FALSE;
  $result = db_insert('bibdk_custom_search_map_values')
    ->fields(array(
      'eid'  => $element_id,
      'vid'  => (int)$element_value['vid'],
      'sort' => (int)$element_value['sort'],
    ))
    ->execute();
  return $result;
}

/**
 * Search element value update.
 */
function _bibdk_custom_search_element_values_update($element_id,$element_value) {
  $result = FALSE;
  $result = db_update('bibdk_custom_search_map_values')
    ->fields(array(
      'sort' => (int)$element_value['sort'],
    ))
    ->condition('mid', (int)$element_value['mid'])
    ->execute();
  return $result;
}
