<?php

/**
 * List search pages form
 */
function _bibdk_custom_search_default_admin_form($elements) {

  drupal_add_css(drupal_get_path('module', 'bibdk_custom_search') . '/bibdk_custom_search.css');

  $form['bibdk_custom_search'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('Search pages ordering'),
    '#description'  => t('Order the pages as you want them to be displayed.'),
    '#tree'         => TRUE,
    '#theme'        => 'bibdk_custom_search-sort-pages-form',
  );

  $i = 0;
  foreach ($elements as $element => $data) {
    $form['bibdk_custom_search'][$i] = array(
      '#title'          => $data['page_title'],
      '#weight'         => $data['sort'],
    );
    $form['bibdk_custom_search'][$i]['pid'] = array(
      '#type'           => 'hidden',
      '#title'          => t('Page ID'),
      '#title_display'  => 'invisible',
      '#default_value'  => $data['pid'],
    );
    $form['bibdk_custom_search'][$i]['page_title'] = array(
      '#type'           => 'hidden',
      '#title'          => t('Page name'),
      '#title_display'  => 'invisible',
      '#default_value'  => $data['page_title'],
    );
    $form['bibdk_custom_search'][$i]['edit'] = array(
      '#type'           => 'link',
      '#title'          => t('edit'),
      '#title_display'  => 'invisible',
      '#href'           =>  'admin/config/search/bibdk_custom_search/' . $data['pid'] . '/edit',
      '#options'        => array('attributes' => array('title' => t('Edit page.'))),
    );
    $form['bibdk_custom_search'][$i]['delete'] = array(
      '#type'           => 'link',
      '#title'          => t('delete'),
      '#title_display'  => 'invisible',
      '#href'           =>  'admin/config/search/bibdk_custom_search/' . $data['pid'] . '/delete',
      '#options'        => array('attributes' => array('title' => t('Delete page.'))),
    );
    $form['bibdk_custom_search'][$i]['sort'] = array(
      '#type'           => 'weight',
      '#title'          => t('Weight'),
      '#title_display' => 'invisible',
      '#default_value'  => $data['sort'],
      '#attributes'     => array('class' => array('element-weight')),
    );
    $i++;
  }

  $form['bibdk_custom_search'][$i] = array(
      '#title'        => 'Add search page',
      '#weight'       => '100',
  );
  $form['bibdk_custom_search'][$i]['pid'] = array(
    '#type'           => 'hidden',
    '#title'          => t('Page ID'),
    '#title_display'  => 'invisible',
    '#default_value'  => '_new',
  );
  $form['bibdk_custom_search'][$i]['page_title'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Name'),
    '#title_display'  => 'invisible',
    '#default_value'  => '',
  );
  $form['bibdk_custom_search'][$i]['sort'] = array(
    '#type'           => 'weight',
    '#title'          => t('Weight'),
    '#default_value'  => 10,
    '#attributes'     => array('class' => array('element-weight')),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save changes'),
  );

  return $form;
}



/**
 * Edit page form
 */
function _bibdk_custom_search_edit_page_form($page = array(),$list_elements=array()) {

  // $page = variable_get('bibdk_custom_search_page', array());

  drupal_add_css(drupal_get_path('module', 'bibdk_custom_search') . '/bibdk_custom_search.css');

  $form['bibdk_custom_search']['page'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('Edit search page'),
    '#description'  => t('Search page configuration.'),
    '#tree'         => TRUE,
    '#theme'        => 'bibdk_custom_search-edit-page-form',
  );

  $form['bibdk_custom_search']['page']['pid'] = array(
    '#type'           => 'hidden',
    '#default_value'  => $page['pid'],
    '#title'          => t('Page ID'),
    '#title_display'  => 'invisible',
  );

  $form['bibdk_custom_search']['page']['sort'] = array(
    '#type'           => 'hidden',
    '#default_value'  => $page['sort'],
    '#title'          => t('Weight'),
  );

  $form['bibdk_custom_search']['page']['page_title'] = array(
    '#type'           => 'textfield',
    '#default_value'  => $page['page_title'],
    '#title'          => t('Page title'),
    '#size'           => 28,
  );

  $form['bibdk_custom_search']['page']['page_path'] = array(
    '#type'           => 'textfield',
    '#default_value'  => $page['page_path'],
    '#title'          => t('Page path'),
    '#size'           => 20,
  );

  $form['bibdk_custom_search']['page']['menu_title'] = array(
    '#type'           => 'textfield',
    '#default_value'  => $page['menu_title'],
    '#title'          => t('Menu title'),
    '#size'           => 28,
  );

  $form['bibdk_custom_search']['page']['delimiter'] = array(
    '#type'           => 'textfield',
    '#default_value'  => $page['delimiter'],
    '#title'          => t('Delimiter'),
    '#size'           => 12,
  );

  $form['bibdk_custom_search']['page']['expand'] = array(
    '#type'           => 'checkbox',
    '#default_value'  => $page['expand'],
    '#title'          => t('Expand options'),
    '#title_display' => 'before',
  );

  $form['bibdk_custom_search']['elements'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('Elements ordering'),
    '#tree'         => TRUE,
    '#description'  => t('Order the form elements as you want them to be displayed.'),
    '#theme'        => 'bibdk_custom_search-edit-page-elements-form',
  );

  $i = 0;
  foreach ($page['elements'] as $key => $element) {
    $form['bibdk_custom_search']['elements'][$i] = array(
      '#title'          => $element['element_title'],
      '#weight'         => $element['sort'],
    );
    $form['bibdk_custom_search']['elements'][$i]['mid'] = array(
      '#type'           => 'hidden',
      '#title'          => t('Element ID'),
      '#title_display'  => 'invisible',
      '#default_value'  => $element['mid'],
    );
    $form['bibdk_custom_search']['elements'][$i]['eid'] = array(
      '#type'           => 'hidden',
      '#title'          => t('Element ID'),
      '#title_display'  => 'invisible',
      '#default_value'  => $element['eid'],
    );
    $form['bibdk_custom_search']['elements'][$i]['element_title'] = array(
      '#type'           => 'hidden',
      '#title'          => t('Page name'),
      '#title_display'  => 'invisible',
      '#default_value'  => $element['element_title'],
    );
    $form['bibdk_custom_search']['elements'][$i]['region'] = array(
      '#type'           => 'select',
      '#options'        => array(
                            'add' => t('Select search region'),
                            'main' => t('Main'),
                            'expand' => t('Expand')
      ),
      '#default_value'  => $element['region'],
      '#title'          => t('Region'),
      '#title_display'  => 'invisible',
      '#attributes'     => array('class' => array('region-select', 'region-select-' . $element['region'])),
    );
    $form['bibdk_custom_search']['elements'][$i]['edit'] = array(
      '#type'           => 'link',
      '#title'          => t('edit'),
      '#title_display'  => 'invisible',
      '#href'           =>  'admin/config/search/bibdk_custom_search/elements/' . $element['eid'] . '/edit',
      '#options'        => array('attributes' => array('title' => t('Edit element.'))),
    );
    $form['bibdk_custom_search']['elements'][$i]['delete'] = array(
      '#type'           => 'link',
      '#title'          => t('remove'),
      '#title_display'  => 'invisible',
      '#href'           =>  'admin/config/search/bibdk_custom_search/' . $page['pid'] . '/remove_element/' . $element['mid'],
      '#options'        => array('attributes' => array('title' => t('Delete element.'))),
    );
    $form['bibdk_custom_search']['elements'][$i]['sort'] = array(
      '#type'           => 'weight',
      '#title'          => t('Weight'),
      '#title_display' => 'invisible',
      '#default_value'  => $element['sort'],
      '#attributes'     => array('class' => array('sort-select', 'sort-select-' . $element['region'])),
    );
    $i++;
  }

  $form['bibdk_custom_search']['elements'][$i] = array(
      '#title'        => 'Add search element',
      '#weight'       => '100',
  );
  $form['bibdk_custom_search']['elements'][$i]['mid'] = array(
    '#type'           => 'hidden',
    '#title'          => t('Element ID'),
    '#title_display'  => 'invisible',
    '#default_value'  => '_new',
  );
  $form['bibdk_custom_search']['elements'][$i]['eid'] = array(
    '#type'           => 'select',
    '#options'        => $list_elements,
    '#title'          => t('Name'),
    '#title_display'  => 'invisible',
    '#default_value'  => '',
  );
  $form['bibdk_custom_search']['elements'][$i]['sort'] = array(
    '#type'           => 'weight',
    '#title'          => t('Weight'),
    '#default_value'  => 10,
    '#attributes'     => array('class' => array('sort-select', 'sort-select-add')),
  );
  $form['bibdk_custom_search']['elements'][$i]['region'] = array(
    '#type'           => 'select',
    '#options'        => array(
                          'add' => t('Select search region'),
                          'main' => t('Main'),
                          'expand' => t('Expand')
    ),
    '#default_value'  => 'add',
    '#title'          => t('Region'),
    '#title_display'  => 'invisible',
    '#attributes'     => array('class' => array('region-select', 'region-select-add')),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save changes'),
  );

  return $form;
}
