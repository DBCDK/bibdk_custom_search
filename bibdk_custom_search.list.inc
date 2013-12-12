<?php



/** Returns a link, a list of options and a list of form elements, for a defined
 * facet
 * @param $facet_type type of facet e.g. facet.type
 * @param $term_type Sets the parent of the returned elements
 * @param array $default_values elements that has a default value
 * @param array $ignore_values elements that should not be returned
 * @return array Render-array
 */
function bibdk_custom_search_get_list_options($facet_type, $term_type, $default_values = array(), $ignore_values= array()) {
  if (strpos($facet_type, '=')){
    $exploded = explode('=', $facet_type);
    $term_type = $exploded[0];
    $facet_type = $exploded[1];
  }
  // Get options from webservice
  $result = bibdk_custom_search_list_from_webservice($facet_type);
  // Create options array with webservice result
  $options = bibdk_custom_search_list_options($result, $ignore_values);

  // If no options are set we do not return form elements
  if (empty($options)){
    return array();
  }

  // Create form
  $form = array();
  $form += bibdk_custom_search_get_list_elements($options, $term_type,  $default_values);
  $form += bibdk_custom_search_get_list_popup_form($options, $facet_type);

  return $form;
}

/** Get elements from webservice
 * @param $type  type of facet e.g. facet.type
 * @param int $number_requested number of elements to request from service
 * @return bool|stdClass|string
 */
function bibdk_custom_search_list_from_webservice($type, $number_requested = 1000) {

  $cache_key = 'bibdk_custom_search_list_from_webservice' . $type;
  if ($cache = cache_get($cache_key, 'cache')){
    return $cache->data;
  }

  $params = array(
    'query' => '*',
    'facets' => array($type),
    'numFacets' => $number_requested,
    'numResults' => 0,
    'objectFormat' => 'DKABM',
  );

  $client = new ting_client_class();

  $result = $client->do_request('search', $params);

  $facets = isset($result->facets[$type]) ? $result->facets[$type]->terms : array();

  if (!empty($facets)){
    cache_set($cache_key, $facets, 'cache');
  }

  return $facets;

}

/** Returns a list of custom search options as an a array where:
 * key = value & ignore values are omitted
 *
 *
 * @param $type
 * @return array
 */
function bibdk_custom_search_list_options($result, $ignore_values) {

  if (!is_array($result) || empty($result))
    return array();

  // Set keys = values in options array
  $options = array_keys($result);
  $options = array_combine($options, $options);

  // Sort alphabetically
  ksort($options);

  // We don't want double values in view
  $options = array_diff($options, $ignore_values);

  return $options;

}

/** Create form with more options as a pop-over box
 * @param $options
 * @param $type
 * @return mixed
 */
function bibdk_custom_search_get_list_popup_form($options, $type) {

  $type_id = str_replace('.', '-', $type);
  $id = 'custom-search-options-list-' . $type_id;
  $form['pop-over'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'id' => $id,
      'class' => array('visuallyhidden', 'custom-search-options-list'),
    ),
  );

  $form['pop-over']['header'] = array(
    '#theme' => 'html_tag',
    '#tag' => 'h3',
    '#value' => t('add options to '.$type, array(), array('context' => 'bibdk_custom_search')),
  );

  $form['pop-over']['two-panel-multi-select'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('panel-select'),
    ),
  );

  $form['pop-over']['two-panel-multi-select']['from'] = array(
    '#type' => 'select',
    '#multiple' => true,
    '#options' => $options,
    '#size' => 20,
    '#attributes' => array(
      'id' => array('select-from-list-' . $type_id),
    ),
  );

  $form['pop-over']['two-panel-multi-select']['actions'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('list-actions'),
    ),
  );

  $form['pop-over']['two-panel-multi-select']['actions']['add_button'] = array(
    '#type' => 'button',
    '#value' => t('add >>', array(), array('context' => 'bibdk_custom_search')),
    '#executes_submit_callback' => false,
    '#attributes' => array(
      'class' => array('custom-search-list-action'),
      'data-action-move-from' => array('select-from-list-' . $type_id),
      'data-action-move-to' => array('select-to-list-' . $type_id),
    ),
  );

  $form['pop-over']['two-panel-multi-select']['actions']['remove_button'] = array(
    '#type' => 'button',
    '#value' => t('<< remove', array(), array('context' => 'bibdk_custom_search')),
    '#executes_submit_callback' => false,
    '#attributes' => array(
      'class' => array('custom-search-list-action'),
      'data-action-move-from' => array('select-to-list-' . $type_id),
      'data-action-move-to' => array('select-from-list-' . $type_id),
    ),
  );

  $form['pop-over']['two-panel-multi-select']['to'] = array(
    '#type' => 'select',
    '#multiple' => true,
    '#options' => array(),
    '#size' => 20,
    '#attributes' => array(
      'id' => array('select-to-list-' . $type_id),
    ),
  );

  $form['pop-over']['close_button'] = array(
    '#type' => 'button',
    '#value' => 'close',
    '#executes_submit_callback' => false,
    '#attributes' => array(
      'class' => array('custom-search-list-close'),
      'data-id' => $id,
    ),
  );

  $form['pop-over']['save_button'] = array(
    '#type' => 'button',
    '#value' => 'save',
    '#executes_submit_callback' => false,
    '#attributes' => array(
      'class' => array('custom-search-list-save'),
      'data-action-selected' => array('select-to-list-' . $type_id),
      'data-action-send-to' => array($type . '-options-array'),
    ),
  );

  $form['container-link'] = array(
    '#theme' => 'link',
    '#text' => t('see all options'),
    '#options' => array(
      'attributes' => array(
        'class' => array('popover-button-list'),
        'data-id' => $id,
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

  return $form;

}

/** Create list render array elements
 * @param $elements
 * @param $parent
 * @param $default_values
 * @return array
 */
function bibdk_custom_search_get_list_elements($elements, $parent, $default_values) {
  $return = array();
  foreach ($elements as $element) {
    $default = isset($default_values[$parent]) && is_array($default_values[$parent]) && in_array($element, $default_values[$parent]) ? true : false;
    $return[$element.'-container'] = array(
      '#type' => 'container',
      //'#tree' => false,
      '#parents' => array(str_replace('.', '_', $parent)),
      '#attributes' => array(
        'data-value' => $element,
        'data-group' => $parent,
        'class' => array(($default) ? 'default' : 'visuallyhidden')
      ),
    );
    $return[$element.'-container'][$element] = array(
      '#type' => 'checkbox',
      '#default_value' => ($default) ? $element : NULL,
      '#parents' => array(str_replace('.', '_', $parent), $element),
      '#title' => t($element, array(), array('context' => 'bibdk_custom_search')),
      '#return_value' => $element,

      '#attributes' => array(
        'data-group' => $parent,
      ),
    );
  }

  return $return;
}