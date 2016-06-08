<?php

/**
 * Implements hook_block_info().
 */
function bibdk_custom_search_block_info() {

  $blocks['search_pages_navigation'] = array(
    'title' => '<none>',
    'info' => t('Custom Search - Search pages navigation'),
  );

  $blocks['custom_search_block'] = array(
    'title' => '<none>',
    'info' => t('Custom Search - Search form'),
  );

  return $blocks;
}

/**
 * Implements hook_block_view().
 */
function bibdk_custom_search_block_view($delta='') {
  $block = array();

  switch($delta) {
    case 'search_pages_navigation' :
      $block['subject'] = t('Custom search page navigation');
      $block['title'] = '<none>';
      $block['content'] = _bibdk_custom_search_pages_navigation();
      break;
    case 'custom_search_block' :
      $block['subject'] = t('Custom search form');
      $block['title'] = '<none>';
      $block['content'] = drupal_get_form('search_block_form');
      break;
  }

  return $block;
}


/**
 * Get searchpage navigation render array
 * @return
 *   render array
 */
function _bibdk_custom_search_pages_navigation() {
  $block = array();

  $page_id = (isset($_REQUEST['page_id'])) ? $_REQUEST['page_id'] : ((isset($_GET['q'])) ? $_GET['q'] : NULL);
  $pages = _bibdk_custom_search_get_pages_db();

  // Gets the searchpages navigationbar if pages exists
  if ($pages) {
    $block['searchpages'] = _bibdk_custom_search_get_searchpage_buttons($page_id, $pages);
  }

  return $block;
}