function refresh()
{
    $('.refresh').each(function() {
        var url = $(this).attr('rel');
        var n = (new Date()).getTime();
        $(this).attr('src', url+'?'+n);
    });
}

$(document).ready(function() {
    setInterval(refresh, 100);
});
