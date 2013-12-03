<?php

/** Returns a list of custom search options as an a array where:
 *    key = search code
 *    value = label
 *
 * @param $type
 * @return array
 */

function bibdk_custom_search_list_from_webservice($type, $number_requested = 1000) {

  $params = array(
    'query' => '*',
    'facets' => array($type),
    'numFacets' => $number_requested,
    'numResults' => 0,
    'objectFormat' => 'DKABM',
  );

  $client = new ting_client_class();

  return $client->do_request('search', $params);

}

function bibdk_custom_search_get_list($type) {

  $result = bibdk_custom_search_list_from_webservice($type);
  $terms = array_keys($result->facets[$type]->terms);
  $terms = array_combine($terms, $terms);
  ksort($terms);
  return $terms;

}

function bibdk_custom_search_get_list_options($facet_type, $term_type, $default_values = array()) {
  $options = bibdk_custom_search_get_list($facet_type);
  $return = bibdk_custom_search_get_list_elements($options, $term_type,  $default_values);
  $return['form'] = bibdk_custom_search_get_list_view_form($options, $facet_type);

  $return['link'] = array(
    '#theme' => 'link',
    '#text' => t('see all options'),
    '#options' => array(
      'attributes' => array(
        'class' => array('popover-button-list'),
      ),
      'fragment' => 'show-options',
      'html' => true,
      'absolute' => false,
      'external' => true,
    ),
    '#attached' => array(
      'js' => array(
        drupal_get_path('module', 'bibdk_custom_search') . '/js/bibdk_custom_search_list.js',
      ),
    ),
  );
  return $return;
}


function bibdk_custom_search_get_list_view_form($options, $type) {

  $form = array(
    '#type' => 'container',
    '#region' => 'main',
    '#attributes' => array(
      'id' => 'custom-search-options-list',
    ),
  );

  $form['pop-over'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('popover', 'visuallyhidden'),
    ),
  );

  $form['pop-over']['header'] = array(
    '#theme' => 'html_tag',
    '#tag' => 'h3',
    '#value' => t('add options to '.$type, array(), array('context' => 'bibdk_custom_search')),
  );

  $form['pop-over']['from'] = array(
    '#type' => 'select',
    '#multiple' => true,
    '#options' => $options,
    '#size' => 20,
    '#attributes' => array(
      'id' => array('select-from-list'),
    ),
  );

  $form['pop-over']['actions'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('list-actions'),
    ),
  );

  $form['pop-over']['actions']['add_button'] = array(
    '#type' => 'button',
    '#value' => t('add >>', array(), array('context' => 'bibdk_custom_search')),
    '#executes_submit_callback' => false,
    '#attributes' => array(
      'class' => array('custom-search-list-action'),
      'data-action-move-from' => array('select-from-list'),
      'data-action-move-to' => array('select-to-list'),
    ),
  );

  $form['pop-over']['actions']['remove_button'] = array(
    '#type' => 'button',
    '#value' => t('<< remove', array(), array('context' => 'bibdk_custom_search')),
    '#executes_submit_callback' => false,
    '#attributes' => array(
      'class' => array('custom-search-list-action'),
      'data-action-move-from' => array('select-to-list'),
      'data-action-move-to' => array('select-from-list'),
    ),
  );

  $form['pop-over']['to'] = array(
    '#type' => 'select',
    '#multiple' => true,
    '#options' => array(),
    '#size' => 20,
    '#attributes' => array(
      'id' => array('select-to-list'),
    ),
  );

  $form['pop-over']['close_button'] = array(
    '#type' => 'button',
    '#value' => 'close',
    '#executes_submit_callback' => false,
    '#attributes' => array(
      'class' => array('custom-search-list-close'),
    ),
  );

  $form['pop-over']['save_button'] = array(
    '#type' => 'button',
    '#value' => 'save',
    '#executes_submit_callback' => false,
    '#attributes' => array(
      'class' => array('custom-search-list-save'),
      'data-action-selected' => array('select-to-list'),
      'data-action-send-to' => array($type . '-options-array'),
    ),
  );
  return $form;

}


function bibdk_custom_search_get_list_elements($elements, $type, $default_values) {
  foreach ($elements as $element) {
    $default = isset($default_values[$type]) && is_array($default_values[$type]) && in_array($element, $default_values[$type]) ? true : false;
    $return[$element.'-container'] = array(
      '#type' => 'container',
      //'#tree' => false,
      '#parents' => array(str_replace('.', '_', $type)),
      '#attributes' => array(
        'data-value' => $element,
        'class' => array(($default) ? 'default' : 'visuallyhidden')
      ),
    );
    $return[$element.'-container'][$element] = array(
      '#type' => 'checkbox',
      '#default_value' => ($default) ? $element : NULL,
      '#title' => t($element, array(), array('context' => 'bibdk_custom_search')),
      '#return_value' => $element,

      '#attributes' => array(
        'data-group' => $type,
      ),
    );
  }

  return $return;
}
