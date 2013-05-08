/**
 * @file
 * Javascript for the wic_nss module dashboard page.
 */

(function ($) {

  Drupal.behaviors.wic_nss = {
    attach: function (context, settings) {
      $('div.assessment-menu li li.expanded').each(function() {
        $(this).removeClass('expanded');
        $(this).addClass('collapsed');
        $(this).find('a:first').after(' <span class="expansion-target">(click here to show sub-forms)</span>');
        $(this).find('ul.menu').hide();
      });
      $('span.expansion-target').click(function() {
        if ($(this).parent().hasClass('collapsed')) {
          $(this).html('(click here to hide sub-forms)');
          $(this).parent().removeClass('collapsed');
          $(this).parent().addClass('expanded');
        }
        else if ($(this).parent().hasClass('expanded')) {
          $(this).html('(click here to show sub-forms)');
          $(this).parent().removeClass('expanded');
          $(this).parent().addClass('collapsed');
        }
        $(this).parent().find('ul.menu:first').slideToggle('fast');
      });
    }
  };

})(jQuery);
