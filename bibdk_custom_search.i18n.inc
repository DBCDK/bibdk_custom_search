<?php
/**
 * @file
 * Internationalization (i18n) hooks
 */

/**
 * Implements hook_i18n_string_info().
 */
function bibdk_custom_search_i18n_string_info() {
  // an object group named 'bibdk_custom_search' exists, and provide a name and description
  $groups['bibdk_custom_search'] = array(
    'title'       => t('bibliotek.dk custom search'),
    'description' => t('bibdk_custom_search description.'),
    'format'      => FALSE,  // This group doesn't have strings with format
    'list'        => TRUE,   // This group cannot list all strings
    'refresh callback' => 'bibdk_custom_search_i18n_string_refresh',
  );
  return $groups;
}

/**
 * Implements hook_i18n_object_info().
 */
function bibdk_custom_search_i18n_object_info() {
  // define the relevant details of objects in this group for translation
  $info['bibdk_custom_search_page'] = array(
    'title' => t('bibdk_custom_search page'),
    'key' => 'pid',
    // 'load callback' => 'bibdk_custom_search_page_load',
    'placeholders' => array(
      '%pid' => 'pid',
    ),
    // 'edit path' => 'admin/config/search/bibdk_custom_search/%pid/edit',
    // 'translate tab' => 'admin/config/search/bibdk_custom_search/%pid/translate',
    'string translation' => array(
      'textgroup' => 'bibdk_custom_search',
      'type' => 'page',
      'table' => 'bibdk_custom_search',
      'properties' => array(
        'menu_title' => t('Menu title'),
      ),
      // 'translate path' => 'admin/config/search/bibdk_custom_search/%pid/translate/%i18n_language',
    )
  );
  $info['bibdk_custom_search_element'] = array(
    'title' => t('bibdk_custom_search element'),
    'key' => 'eid',
    'string translation' => array(
      'textgroup' => 'bibdk_custom_search',
      'type' => 'element',
      'table' => 'bibdk_custom_search_elements',
      'properties' => array(
        'element_label' => t('Element label'),
        'description' => t('Element description'),
        'help_text' => t('Element help text'),
      ),
    )
  );
  $info['bibdk_custom_search_option'] = array(
    'title' => t('bibdk_custom_search option'),
    'key' => 'oid',
    'string translation' => array(
      'textgroup' => 'bibdk_custom_search',
      'type' => 'option',
      'table' => 'bibdk_custom_search_options',
      'properties' => array(
        'label' => t('Option label'),
        'value' => t('Option value'),
      ),
    )
  );
  return $info;
}
