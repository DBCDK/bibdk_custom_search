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

      // Autoselect the 'all' values under checkboxes
      $('input[type=checkbox].master').attr('checked', true);
      $('input[type=checkbox]').change(function (e){
          var group = $(this).attr('group');
          if($(this).attr('checked')){
              if ($(this).hasClass('master')){
                  $('[group=' + group + ']').attr('checked', false);
                  $(this).attr('checked', true);
              }
              else{
                  $('[group=' + group + '].master').attr('checked', false)
              }
          }
          else if ($('[group=' + group + ']:checked').length == 0) {
              $('[group=' + group + '].master').attr('checked', true);
          }
      });
  };


  Drupal.behaviors.bibdkCustomSearchCheckboxes = {
    attach: function(context, settings) {
      $('.form-type-checkbox input').change(function() {
        // clear other checkboxes if top-level default is selected.
        if ( $(this).hasClass('default-value') && $(this).is(':checked') ) {
          $(this).closest(".bibdk-custom-search-element").find("input").each(function (i) {
            if ( !$(this).hasClass('default-value') ) {
              $(this).attr('checked', false);
            }
          })
        }
        // clear top-level default if other checkboxes is selected.
        if ( !$(this).hasClass('default-value') && $(this).is(':checked') ) {
          $(this).closest(".bibdk-custom-search-element").find("input").each(function (i) {
            if ( $(this).hasClass('default-value') ) {
              $(this).attr('checked', false);
            }
          })
        }
        // set top-level default as selected, if all other checkboxes are unchecked.
        if ( !$(this).is(':checked') ) {
          var counter = 0;
          $(this).closest(".bibdk-custom-search-element").find("input").each(function (i) {
            if ( $(this).is(':checked') ) {
              counter++;
            }
          })
          if ( !counter ) {
            $(this).closest(".bibdk-custom-search-element").find("input.default-value").attr('checked', true);
          }
        }
      });
    }
  };


/*
  Drupal.behaviors.bibdkCustomSearchCheckboxes = {
    attach: function(context, settings) {
      $('.form-type-checkboxes input').change(function() {
        if ( $(this).val() && $(this).is(':checked') ) {
          alert('foo');
          $(this).closest("div.form-checkboxes").find("input").each(function (i) {
            if ( !$(this).val() ) {
              $(this).attr('checked', false);
            }
          })
        }
        if ( !$(this).val() && $(this).is(':checked') ) {
          alert('bar');
          $(this).closest("div.form-checkboxes").find("input").each(function (i) {
            if ( $(this).val() ) {
              $(this).attr('checked', false);
            }
          })
        }
      });
    }
  };
*/

} (jQuery));

