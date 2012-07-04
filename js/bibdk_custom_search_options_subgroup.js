(function($) {

  $(document).ready(function() {
    Drupal.bibdkCustomSearchOptionsSubgroup();
    $('.bibdk-subgroup').hide();
  });

  Drupal.bibdkCustomSearchOptionsSubgroup = function() {
    $.each(Drupal.settings.expanded, function(key, val) {

      var ExpandTrigger = $('<div data="subgroup-' + key + '" class="toggle-subgroup"> + </div>');
      ExpandTrigger.toggle(function() {
        var subgroup = $(this).attr('data');
        $('#' + subgroup).show();
      }, function() {
        var subgroup = $(this).attr('data');
        $('#' + subgroup).hide();
      });
      $('input[value=' + val.parentElem + ']').after(ExpandTrigger);

      var subElem = $('<div id="subgroup-' + key + '" class="bibdk-subgroup"></div>');
      $.each(val.childElem, function(key2, val2) {
        subElem.append( $('input[value=' + val2 + ']').closest("div") );
      });

      $('input[value=' + val.parentElem + ']').closest("div").after(subElem);
    });
  };

} (jQuery));

