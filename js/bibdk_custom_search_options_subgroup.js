(function($) {

  $(document).ready(function() {
    Drupal.bibdkCustomSearchOptionsSubgroup();
    $('.bibdk-subgroup').hide();
  });

  Drupal.bibdkCustomSearchOptionsSubgroup = function() {
    $.each(Drupal.settings.expanded, function(element, elementGroup) {
      $.each(elementGroup, function(key, val) {

        // create toggle element and bind an event handler to it
        var ExpandTrigger = $('<div data="subgroup-' + val.elementId + '-' + val.parentValueId + '" class="toggle-subgroup"> + </div>');
        ExpandTrigger.toggle(function() {
          var subgroup = $(this).attr('data');
          $('#' + subgroup).show('fast');
        }, function() {
          var subgroup = $(this).attr('data');
          $('#' + subgroup).hide('fast');
        });
        $('#edit-bibdk-custom-search-element-' + val.elementId + ' input[value=' + val.parentValue + ']').after(ExpandTrigger);

        // uncheck child option if a parent option is selected.
        $('#edit-bibdk-custom-search-element-' + val.elementId + ' input[value=' + val.parentValue + ']').change(function() {
          if ( $(this).is(':checked') ) {
            $.each(val.childElem, function(key2, val2) {
              $('#edit-bibdk-custom-search-element-' + val.elementId + ' input[value=' + val2 + ']').attr('checked', false);
            });
          }
        });

        // uncheck parent option if a child option is selected.
        $.each(val.childElem, function(key2, val2) {
          $('#edit-bibdk-custom-search-element-' + val.elementId + ' input[value=' + val2 + ']').change(function() {
            if ( $(this).is(':checked') ) {
              $('#edit-bibdk-custom-search-element-' + val.elementId + ' input[value=' + val.parentValue + ']').attr('checked', false);
            }
          });
        });

        // move child options into a div under parent.
        var subElem = $('<div id="subgroup-' + val.elementId + '-' + val.parentValueId + '" class="bibdk-subgroup"></div>');
        $.each(val.childElem, function(key2, val2) {
          subElem.append( $('#edit-bibdk-custom-search-element-' + val.elementId + ' input[value=' + val2 + ']').closest("div") );
        });

        $('#edit-bibdk-custom-search-element-' + val.elementId + ' input[value=' + val.parentValue + ']').closest("div").after(subElem);
      });
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

