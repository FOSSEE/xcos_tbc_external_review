function getseconds() {
    // take mins remaining (as seconds) away from total seconds remaining
    return secs-Math.round(mins *60);
}
function getminutes() {
    // minutes is seconds divided by 60, rounded down
    mins = Math.floor(secs / 60);
    return mins;
}
function Decrement() {
    if (document.getElementById) {
        minutes = document.getElementById("minutes");
        seconds = document.getElementById("seconds");
        // if less than a minute remaining
        if (seconds < 59) {
            seconds.value = secs;
        } else {
            minutes.innerHTML= getminutes();
            seconds.innerHTML = getseconds();
        }
        secs--;
        
        if(mins == 0 && secs == 0) {
            window.location = modPath + "eligibility-test/end";
        }
        setTimeout('Decrement()',1000);
    }
}
function countdown() {
    setTimeout('Decrement()',1000);
}
(function ($) {
$(document).ready(function() {
    basePath = Drupal.settings.basePath;
    modPath = basePath + "xcos-tbc-external-review/";
    mins = parseInt($("#minutes_remaining").val());
    tmp = parseInt($("#seconds_remaining").val());  
    secs = mins * 60 + tmp;
    countdown();
});
})(jQuery);
