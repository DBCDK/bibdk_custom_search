(function($) {

  $(document).ready(function() {
    Drupal.bibdkCustomSearchOptionsSubgroup();
  });

  Drupal.bibdkCustomSearchOptionsSubgroup = function() {
      $('input[type=checkbox]').each(function(index) {
        if ( $(this).attr('data') != 'undefined' ) {
          var sThisValue = $(this).attr('value');
          var sThisData = $(this).attr('data').split('#');
          var sThisDataParent = sThisData[0];
          if ( sThisValue == sThisDataParent ) {
            $(this).attr('data', 'isParent:' + $(this).attr('id'));
          }
          var sThisChildren = sThisData[1].split('|');
          $.each(sThisChildren, function(key, val) {
            $('input[value='+val+']').attr('data', 'hasParent:' + sThisDataParent);
          });
        }
      });
  };

} (jQuery));

