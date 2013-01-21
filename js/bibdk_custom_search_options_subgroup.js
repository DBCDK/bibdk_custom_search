(function($) {

  $(document).ready(function() {
    Drupal.bibdkCustomSearchOptionsSubgroup();
  });

  Drupal.bibdkCustomSearchOptionsSubgroup = function() {
      $('fieldset[child]').hide();
      // If child checkbox is checked remove checked from parent
      $('fieldset[child] input').change(function(){
          if ($(this).attr('checked') == true){
              var parentKey = $(this).closest('fieldset[child]').attr('child');
              $('[parent='+parentKey+']').attr('checked', false);
          }
      });
      // If parent checkbox is checked remove checked from children
      $('input[parent]').change(function(){
          if ($(this).attr('checked') == true){
              var childKey = $(this).attr('parent');
              $('fieldset[child='+childKey+'] input').attr('checked', false);
          }
      });
      $('.toggle-subgroup').click(function (e){
          e.preventDefault();
          var childKey = $(this).attr('child');
          $('fieldset[child=' + childKey + ']').toggle();
      });
  };

  Drupal.behaviors.bibdkCustomSearchCheckboxes = {
    attach: function(context, settings) {
      $('.form-type-checkboxes input').change(function() {
        if ( $(this).val() && $(this).is(':checked') ) {
          $(this).closest("div.form-checkboxes").find("input").each(function (i) {
            if ( !$(this).val() ) {
              $(this).attr('checked', false);
            }
          })
        }
        if ( !$(this).val() && $(this).is(':checked') ) {
          $(this).closest("div.form-checkboxes").find("input").each(function (i) {
            if ( $(this).val() ) {
              $(this).attr('checked', false);
            }
          })
        }
      });
    }
  };


} (jQuery));

