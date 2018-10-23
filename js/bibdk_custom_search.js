(function($) {

  /**
   * If we are on devicesize large and up request and insert the advanced
   * search panel
   */
  function getAdvancedSearchPanel() {
      if(typeof($(window.document).foundation) != 'undefined') {
          if (window.matchMedia(window.Foundation.media_queries.large).matches && Drupal.settings.bibdk_custom_search && !Drupal.settings.bibdk_custom_search.advancedSearchIsLoaded) {
              var url = Drupal.settings.basePath + Drupal.settings.pathPrefix + 'bibdk_custom_search/ajax/get_search_panel';
              jQuery.get(url, {page_id: 'bibdk_frontpage'})
                  .done(function (data, response) {
                      Drupal.settings.bibdk_custom_search.advancedSearchIsLoaded = true;
                      var $new = $('#search-advanced-panel', data);
                      $('#search-advanced-panel').replaceWith($new);
                      Drupal.attachBehaviors($new, Drupal.settings);
                      onLoad.setFocus();
                  })
                  .fail(function () {
                      throw new Error('An error happend while loading search pages');
                  })
          }
      }
  };

  var CustomSearch = {
    toggleAdvancedSearchPanel: function(context){
      // Toggle advanced search options
      $('#custom-search--advanced-toggle', context).once('custom-search-advanced-toggle', function () {
        $(this).click(function (e) {
          e.preventDefault();
          $(this).toggleClass('toggled');
          $('.toggle-text', this).toggleClass('hidden');
          $('#search-advanced').toggleClass('hidden');
          // Move to first input field in expanded search, if activated.
          if($('#custom-search--advanced-toggle').hasClass('toggled') === true) {
            $('#edit-advanced').removeClass('is-visible');
            $('form#search-block-form input[name="search_block_form"]').not('.page-search form#search-block-form input[name="search_block_form"], .page-vejviser form#search-block-form input[name="search_block_form"]').focus();
          }
          else {
            $('#edit-advanced').addClass('is-visible');
            $('form#search-block-form').find('.bibdk-custom-search-element input[type=text], .bibdk-custom-search-element textarea').filter(':visible:first').focus();
          }
        });
      });
    }
  };

  Drupal.behaviors.bibdk_custom_search = {
    attach: function(context) {
      getAdvancedSearchPanel();
      CustomSearch.toggleAdvancedSearchPanel(context);
    }
  };
}(jQuery));
