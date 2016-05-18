<?php

/**
 * Hook implementations for bib_open_format
 */

/**
 * Implements hook_bib_open_format_search_conditions_alter
 *
 * @param $conditions
 * @return array
 */
function bibdk_custom_search_bib_open_format_search_conditions_alter(&$conditions) {
  dpm(__FUNCTION__);
  $submitted_searchcodes = bibdk_custom_search_get_submitted_searchcodes($_REQUEST);
  if (!empty($_REQUEST['qe'])) {
    $conditions['qe'] = $_REQUEST['qe'];
  }
  $conditions += $submitted_searchcodes;
  return($conditions);
}
