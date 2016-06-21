<?php

/**
 * Hook implementations for ting_openformat
 */

/**
 * Implements hook_ting_openformat_conditions (for bibliotek.dk)
 *
 * @param $query
 * @return array
 */
function bibdk_custom_search_ting_openformat_conditions($query) {
  return bibdk_custom_search_get_submitted_searchcodes($_REQUEST);
}

/**
 * Implements hook_bib_open_format_search_conditions_request (for netpunkt)
 *
 * @param $query
 * @return array
 */
function bibdk_custom_search_bib_open_format_search_conditions_request($query){
  return bibdk_custom_search_get_submitted_searchcodes($_REQUEST);
}
