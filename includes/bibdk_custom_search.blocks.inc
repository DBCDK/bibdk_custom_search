<?php

/**
 * Implements hook_block_info().
 */
function bibdk_custom_search_block_info() {
  $blocks['search_pages_navigation'] = array(
    'info' => t('Netpunkt - Search pages navigation'),
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
      $block['content'] = _bibdk_custom_search_pages_navigation();
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