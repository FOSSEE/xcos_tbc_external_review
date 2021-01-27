// Javascript format string
// First, checks if it isn't implemented yet.
if (!String.prototype.format) {
  String.prototype.format = function() {
    var args = arguments;
    return this.replace(/{(\d+)}/g, function(match, number) { 
      return typeof args[number] != 'undefined'
        ? args[number]
        : match
      ;
    });
  };
}

(function ($) {
$(document).ready(function() {

    var basePath = Drupal.settings.basePath;
    var modPath = basePath + "xcos-tbc-external-review/";
    $ajax_loader = $("#ajax-loader"); 
    /* for "tbc_external_review/comments" page */
  /*  $book = $("#edit-book");
    $chapter = $("#edit-chapter");
    $chapter_wrapper = $("#edit-chapter-wrapper");
    $example = $("#edit-example");
    $example_wrapper = $("#edit-example-wrapper");
    $error_wrapper = $("#comment-error-wrapper");
    $submit = $("#edit-submit");
    $ajax_loader = $("#ajax-loader");

    $book.change(function() {
        var pid = $(this).val();
        $.ajax({
            url: modPath + "ajax/book/"+pid,
            type: "GET",
            success: function(data) {
                $chapter.html(data);
                $chapter_wrapper.show();
            },
        });
    });

    $chapter.change(function() {
        var cid = $(this).val();
        $.ajax({
            url: modPath + "ajax/chapter/"+cid,
            type: "GET",
            success: function(data) {
                $example.html(data);
                $example_wrapper.show();
            },
        });
    });

    $example.change(function() {
        $error_wrapper.show();
        $submit.show();
    });

    /* for "tbc_external_review/manage_comments/#some_number" page 
    $view_comment = $(".view-comment");
    $lightbox_wrapper = $("#lightbox-wrapper");
    $lightbox_inner = $("#lightbox-inner");
    $view_comment.click(function(e) {
        var comment_id = $(this).attr("data-comment");
        
        $.ajax({
            url: modPath + "ajax/comment/" + comment_id,
            type: "GET",
            dataType: "html",
            success: function(data) {
                $lightbox_inner.html(data);
                $lightbox_wrapper.lightbox_me({
                    centered: true
                });
            },
        });
        e.preventDefault();
    });
    */
$('#btnDialog').click(function(){

	  $('#dialog').dialogBox({

	    //OPTIONS
	   

	  })

	});
    /* hide/unhide comments */
    $hide_show= $(".hide-show");
    
    $hide_show.click(function(e) {
     console.log('test');
        var comment_id = $(this).attr("data-comment");
        $t = $(this);
        ($).ajax({
            url: modPath + "ajax/hide-show/" + comment_id,
            type: "GET",
            dataType: "html",
            success: function(data) {
                $tr = $t.parents("tr:first");
                if($tr.hasClass("dull")) {
                    $t.parents("tr:first").removeClass("dull");
                    $t.html("Hide");
                } else {
                    $t.parents("tr:first").addClass("dull");
                    $t.html("Show");
                }
                console.log(data);
            },
        });
        e.preventDefault();
    });

     $hide_show1= $(".hide-show1");
    
    $hide_show1.click(function(e) {
        var preference_id = $(this).attr("manages-comment");
        $t = $(this);
        ($).ajax({
            url: modPath + "ajax/hide-show1/" + preference_id,
            type: "GET",
            dataType: "html",
            success: function(data) {
                $tr = $t.parents("tr:first");
                if($tr.hasClass("dulls")) {
                    $t.parents("tr:first").removeClass("dulls");
                    $t.html("Hide");
                } else {
                    $t.parents("tr:first").addClass("dulls");
                    $t.html("Show");
                }
                console.log(data);
            },
        });
        e.preventDefault();
    });

    /* toggle hide-show */
    $toggler = $(".toggle-hide-show");
    $toggler.click(function() {
        var preference_id = $(this).attr("data-preference");
        
        $.ajax({
            url: modPath + "ajax/toggle/" + preference_id,
            type: "GET",
            success: function(data) {
                $tr = $("tr");
                $tr.each(function() {
                    if(!$(this).hasClass("error-comment")) {
                        if($(this).hasClass("dull")) {
                            $(this).removeClass("dull");
                            $(this).find(".hide-show").html("Hide");
                        } else {
                            $(this).addClass("dull");
                            $(this).find(".hide-show").html("Show");
                            $("thead tr").removeClass("dull");
                        }
                    }
                });
            }
        });
    });
    
   
    var count = 1;
    $more = $jq("#review-completion-form #more");
    $missed = $jq("#review-completion-form #missed-list");

    /* show/hide missed-list */
    $jq("#edit-missing-0").click(function() {
        $more.hide();
        $missed.hide();
        $jq("div").filter(function() {
            return this.id.match(/missed-list-.*/);
        }).hide();
    });
    $jq("#edit-missing-1").click(function() {
        $more.show();
        $missed.show();
        $jq("div").filter(function() {
            return this.id.match(/missed-list-.*/);
        }).show();
    });

    /* review complete form  */
    $more.click(function(e) {
        $dupe = $missed.clone();
        $dupe.attr("id", "missed-list-" + count);
        $dupe.find("select[name='missed_chapter']").attr("id", "missed_chapter_" + count);
        $dupe.find("select[name='missed_chapter']").attr("name", "missed_chapters[]");
        $dupe.find("select[name='missed_example']").attr("id", "missed_example_" + count);
        $dupe.find("select[name='missed_example']").attr("name", "missed_examples[]");
        $dupe.insertBefore($more);
        $dupe.append("<a href='#' class='delete-missed' data-target='{0}'>Delete</a>".format(count));
        count++;
        e.preventDefault();
    });

    $jq("#review-completion-form").on("click", ".delete-missed", function(e) {
        var target = $jq(this).data("target");
        $("#review-completion-form #missed-list-" + target).remove();
        count--;
        e.preventDefault();
    });

    $jq("#review-completion-form #edit-cancel").click(function(e) {
        window.location = modPath + "status";
        e.preventDefault();
    });
    $jq("#status-edit-form #edit-cancel").click(function(e) {
        window.location = modPath + "manage-status";
        e.preventDefault();
    });
    $jq("#add-book-form #edit-cancel").click(function(e) {
        window.location = basePath + "manage-proposal/all";
        e.preventDefault();
    });
    $(document).ajaxStart(function() {
        $ajax_loader.show();
    });
    $(document).ajaxStop(function() {
        $ajax_loader.hide();
    });
    
    
    //xcos-popup for review //
    $(window).load(function(){
    jQuery(document).ready(function ($) {

    $(this).on('click', '#xcos-popup_window', function (e) {
    
        var comment_id = $(this).attr("data-comment");
       
        console.log('ok');
        
         ($).ajax({
            url: modPath + "ajax/xcos-comment/" + comment_id,
            type: "GET",
            dataType: "html",
            success: function(data) {
               $("#xcos-popup-content").html(data);
            },

        });
        e.preventDefault();
        $('html').addClass('overlay');
        $('#example-xcos-popup').addClass('visible');
    });
        
        

   

    $(document).keyup(function (e) {
        if (e.keyCode == 27 && $('html').hasClass('overlay')) {
            clearPopup();
        }
    });

    $('.xcos-popup-exit').click(function () {
        clearPopup();

    });

    $('.xcos-popup-overlay').click(function () {
        clearPopup();
    });

    function clearPopup() {
        $('.xcos-popup.visible').addClass('transitioning').removeClass('visible');
        $('html').removeClass('overlay');

        setTimeout(function () {
            $('.xcos-popup').removeClass('transitioning');
        }, 200);
    }

});
});//]]>  
});
})(jQuery);
