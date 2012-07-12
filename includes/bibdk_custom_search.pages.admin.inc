<?php

/**
 * @file
 * Admin settings for bibdk search pages
 */

/**
 * Includes
 */
module_load_include('inc', 'bibdk_custom_search', 'includes/bibdk_custom_search-pages-forms');


/**
 * General settings.
 */
function bibdk_custom_search_admin() {
  // Page ordering and create new page.

  $query = db_select('bibdk_custom_search', 'p');
  $query
    ->fields('p', array('pid','page_title','sort'))
    ->orderBy('page_title', 'DESC')
    ->orderBy('sort', 'DESC');
  $result = $query->execute();

  $elements = array();
  foreach ($result as $record) {
    $row = array(
      'pid' => check_plain($record->pid),
      'page_title' => check_plain($record->page_title),
      'sort' => check_plain($record->sort),
    );
    $elements[] = $row;
  }

  $form = _bibdk_custom_search_default_admin_form($elements);
  $form['#submit'][] = 'bibdk_custom_search_admin_submit';
  return $form;
}

/**
 * Submit handler for bibdk_custom_search_admin();
 */
function bibdk_custom_search_admin_submit($form, &$form_state) {
  $pages = $form_state['values']['bibdk_custom_search'];
  $size = sizeof($pages);
  foreach ($pages as $key => $page) {
    if ( $page['pid'] == '_new' && !empty($page['page_title']) ) {
      _bibdk_custom_search_page_insert($page);
      $message[] = t('The page "@page" have been created.',array('@page'=>$page['page_title']));
    } else {
      _bibdk_custom_search_page_update($page);
      $message[] = t('The pages have been updated.');
    }
  }
  drupal_set_message(implode('<br/>',array_unique($message)));
  cache_clear_all('bibdk_custom_search', 'cache', TRUE);
}

/**
 * Search page list insert.
 */
function _bibdk_custom_search_page_insert($page) {
  $result = db_insert('bibdk_custom_search')
    ->fields(array(
      'page_title' => check_plain($page['page_title']),
      'sort' => (int)$page['sort'],
    ))
    ->execute();
  return $result;
}

/**
 * Search page list update.
 */
function _bibdk_custom_search_page_update($page) {
  $result = db_update('bibdk_custom_search')
    ->fields(array(
      'sort' => (int)$page['sort'],
    ))
    ->condition('pid', $page['pid'])
    ->execute();
  return $result;
  drupal_set_message(t('The configuration options have been saved.'));
}


/**
 * Validation handler for bibdk_custom_search_admin.
 */
function bibdk_custom_search_admin_validate($form, &$form_state) {
  // drupal_set_message(t('bibdk page validation.'));
  return $form;
}


/**
 * bibdk_custom_search_delete_page confirmation form;
 */
function bibdk_custom_search_delete_page($form_id, $form_state, $page_id) {

  $result = db_select('bibdk_custom_search', 'p')
    ->fields('p',array('page_title'))
    ->condition('pid', $page_id,'=')
    ->execute()
    ->fetchAssoc();

  $form['page_id'] = array(
    '#type' => 'value',
    '#value' => $page_id,
  );

  $form['page_title'] = array(
    '#type' => 'hidden',
    '#value' => $result['page_title'],
  );

  return confirm_form($form, t("Are you sure you want to delete '@page'?", array('@page' => $result['page_title'])), 'admin/config/search/bibdk_custom_search');
}

/**
 * Submit handler for bibdk_custom_search_delete_page();
 */
function bibdk_custom_search_delete_page_submit($form, &$form_state) {

  $result = db_delete('bibdk_custom_search_map_elements')
    ->condition('pid', (int)$form_state['values']['page_id'])
    ->execute();

  $result = db_delete('bibdk_custom_search')
    ->condition('pid', (int)$form_state['values']['page_id'])
    ->execute();

  drupal_set_message(t("'@page' deleted.", array('@page' => $form_state['values']['page_title'])));

  cache_clear_all('bibdk_custom_search', 'cache', TRUE);

  $form_state['redirect'] = 'admin/config/search/bibdk_custom_search';
}

/**
 *
 */
function bibdk_custom_search_page_edit($form_id, $form_state, $page_id, $element_pointer=FALSE) {

  if ( $element_pointer ) {
    $result = db_delete('bibdk_custom_search_map_elements')
      ->condition('mid', (int)$element_pointer)
      ->execute();
    drupal_set_message( t("Element removed.") );
  }

  // bibdk_custom_search
  $page = db_select('bibdk_custom_search', 'p')
    ->fields('p')
    ->condition('pid', $page_id,'=')
    ->execute()
    ->fetchAssoc();

  // bibdk_custom_search_elements
  $list_elements[0] = 'Select elements';
  $query = db_select('bibdk_custom_search_elements', 'e');
  $query
    ->fields('e', array('eid','element_title','sort'))
    ->orderBy('sort', 'DESC');
  $result = $query->execute();
  foreach ($result as $row) {
    $eid = check_plain($row->eid);
    $element_title = check_plain($row->element_title);
    $list_elements[$eid] = $element_title;
  }

  // bibdk_custom_search_map_elements
  $query = db_select('bibdk_custom_search_map_elements', 'm');
  $query -> join('bibdk_custom_search_elements', 'e', 'm.eid = e.eid');
  $query -> fields('m', array('mid','eid','region','sort'));
  $query -> fields('e', array('element_title'));
  $query -> condition('pid', $page_id,'=');
  $query -> orderBy('sort', 'DESC');
  $result = $query -> execute();
  $page_elements = array();
  foreach ($result as $record) {
    $row = array(
      'mid' => check_plain($record->mid),
      'eid' => check_plain($record->eid),
      'region' => check_plain($record->region),
      'element_title' => check_plain($record->element_title),
      'sort' => check_plain($record->sort),
    );
    $page_elements[] = $row;
  }
  $page['elements'] = $page_elements;

  $form = _bibdk_custom_search_edit_page_form($page,$list_elements);

  $form['#submit'][] = 'bibdk_custom_search_edit_page_submit';

  return $form;
}

/**
 *
 */
function bibdk_custom_search_edit_page_submit($form, &$form_state) {
  $page = $form_state['values']['page'];
  $page_id = (int)$form_state['values']['page']['pid'];

  $result = db_update('bibdk_custom_search')
    ->fields(array(
      'page_title' => check_plain($page['page_title']),
      'page_path' => check_plain($page['page_path']),
      'menu_title' => check_plain($page['menu_title']),
      'delimiter' => check_plain($page['delimiter']),
      'expand' => check_plain($page['expand']),
      'sort' => check_plain($page['sort']),
    ))
    ->condition('pid', (int)$page['pid'])
    ->execute();

  $page_elements = $form_state['values']['elements'];
  foreach ($page_elements as $key => $page_element) {
    if ( $page_element['mid']=='_new' && !empty($page_element['eid']) ) {
      _bibdk_custom_search_page_element_insert($page_id,$page_element);
    }
    if ( $page_element['mid']!=='_new' ) {
      _bibdk_custom_search_page_element_update($page_id,$page_element);
    }
  }

  cache_clear_all('bibdk_custom_search', 'cache', TRUE);

  drupal_set_message(t('The page options have been saved.'));

  $form_state['redirect'] = 'admin/config/search/bibdk_custom_search/' . $page_id . '/edit';
}


/**
 * Search page element insert.
 */
function _bibdk_custom_search_page_element_insert($page_id,$page_element) {
  $result = FALSE;
  $result = db_insert('bibdk_custom_search_map_elements')
    ->fields(array(
      'pid'  => (int)$page_id,
      'eid'  => (int)$page_element['eid'],
      'region' => check_plain($page_element['region']),
      'sort' => (int)$page_element['sort'],
    ))
    ->execute();
  return $result;
}

/**
 * Search page element update.
 */
function _bibdk_custom_search_page_element_update($page_id,$page_element) {
  $result = FALSE;
  $result = db_update('bibdk_custom_search_map_elements')
    ->fields(array(
      'region' => check_plain($page_element['region']),
      'sort' => (int)$page_element['sort'],
    ))
    ->condition('mid', (int)$page_element['mid'])
    ->execute();
  return $result;
}
