(function($) {

  $(document).ready(function() {
    Drupal.bibdkSearchPagesToggle();
  });


  Drupal.behaviors.bibdkSearchPagesToggleType = {
    attach: function(context, settings) {
      $('#edit-element-value-type').change(function() {
        Drupal.bibdkSearchPagesToggle();
      });
    }
  };

  Drupal.bibdkSearchPagesToggle = function() {
    $('#edit-element-value-options').hide();
    $('.form-item-element-value-search-code').hide();
    $('.form-item-element-value-default-value').hide();
    if ( !$('#edit-element-value-type').val() ) {
      return false;
    }
    if ( $('#edit-element-value-type').val() == 'textfield' ) {
      $('#edit-element-value-options').hide();
      $('.form-item-element-value-search-code').show();
      $('.form-item-element-value-default-value').show();
    } else {
      $('#edit-element-value-options').show();
      $('.form-item-element-value-search-code').hide();
      $('.form-item-element-value-default-value').hide();
    }
  };

} (jQuery));

