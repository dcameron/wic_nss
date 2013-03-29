/**
 * @file
 * Javascript for the wic_nss module dashboard page.
 */

(function ($) {

  $(document).ready(function () {
    $('div.assessment-menu li li.expanded').each(function() {
      $(this).removeClass('expanded');
      $(this).addClass('collapsed');
      $(this).find('a:first').after(' <span class="expansion-target">(show sub-forms)</span>');
      $(this).find('ul.menu').hide();
    });
    $('span.expansion-target').click(function() {
      if ($(this).parent().hasClass('collapsed')) {
        $(this).html('(hide sub-forms)');
        $(this).parent().removeClass('collapsed');
        $(this).parent().addClass('expanded');
      }
      else if ($(this).parent().hasClass('expanded')) {
        $(this).html('(show sub-forms)');
        $(this).parent().removeClass('expanded');
        $(this).parent().addClass('collapsed');
      }
      $(this).parent().find('ul.menu:first').slideToggle('fast');
    }
    );
  });

})(jQuery);
