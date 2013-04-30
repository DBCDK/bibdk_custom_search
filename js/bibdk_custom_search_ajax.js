(function ($) {
    Drupal.getSearchPage = function(element){

        var type = element.find('input').attr('data-type');
        var searchString = '10000 x hest'; //TODO get current searchstring - if any

        element.addClass('ajax-progress');
        element.append('<span class="throbber">&nbsp;</span>');

        $.ajax({
            url:Drupal.settings.basePath + 'frontpage/searchpage_callback',
            type:'GET',
            data:{
                type:type,
                req:searchString
            },
            dataType:'json',
            success:Drupal.response
        });
    };

    Drupal.response = function(response){
        console.log(response);
        $('#search-form').html(response.form);
        $('body').addClass(response.bodyclass);
        Drupal.attachBehaviors('#search-form', Drupal.settings);
        Drupal.bibdkCustomSearchOptionsSubgroup(); //XXX instead of calling across scripts it should all be collected into one (bibdk_custom_search_options_subgroup.js)

        window.history.pushState({}, '', Drupal.settings.basePath + response.request); //TODO correct destination
        return false;
    };

    Drupal.behaviors.search = {
        attach:function (context) {
            $('#search-form .horizontal-nav li', context).click(function (e) {
                e.preventDefault();
                Drupal.getSearchPage($(this));
            });
        }
    };
}(jQuery));
