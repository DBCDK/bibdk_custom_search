<?php

/**
 * Hook implementations for ting_openformat
 */

/**
 * Implements hook_ting_openformat_conditions
 *
 * @param $query
 * @return array
 */
function bibdk_custom_search_ting_openformat_conditions($query) {
  return bibdk_custom_search_get_submitted_searchcodes($_REQUEST);
}
