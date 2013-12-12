(function ($) {
    var CustomSearchList = {
        move: function (element) {
            var from = $(element).attr('data-action-move-from');
            var to = $(element).attr('data-action-move-to');
            $('#' + from + ' option:selected').each(function (i, option) {
                var label = $(option).text();
                var value = $(option).val()
                $('#' + to).append('<option value="' + value + '">' + label + '</option>')
                $('#' + from + ' option[value=' + value + ']').remove();
            });
        },
        save: function (element) {
            var from = $(element).attr('data-action-selected');
            $('#' + from + ' option').each(function (i, option) {
                $('[data-value="' + $(option).val() + '"]').removeClass('visuallyhidden').find('input').attr('checked', 'checked').trigger('change');

            });
            $('.custom-search-list-close').trigger('click');
        },
        close: function (id){
            $(id).addClass('visuallyhidden');
        },
        open : function (id){
            $('.popover').addClass('visuallyhidden');
            $(id).removeClass('visuallyhidden').find('input').select();
        }
    };
    /** Move element from one select to another */
    Drupal.behaviors.custom_search_list = {

        attach: function (context) {
            $('.popover-button-list', context).click(function (e) {
                CustomSearchList.open('#' + $(this).attr('data-id'));
            });
            $('.custom-search-list-action', context).click(function (event) {
                event.preventDefault();
                CustomSearchList.move(this);
            });
            $('.custom-search-list-save', context).click(function (event) {
                event.preventDefault();
                CustomSearchList.save(this);
            });
            $('.custom-search-list-close', context).click(function (event) {
                event.preventDefault();
                CustomSearchList.close('#' + $(this).attr('data-id'));

            });
        },
        detach: function (context) {

        }
    };
}(jQuery));

