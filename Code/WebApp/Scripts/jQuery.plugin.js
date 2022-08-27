(function ($) {
    console.log("JQUERY: " + $);


    $.fn.todo = function (options) {
        console.log("CALL PLUGIN TODO");
        var defaults = {
            serverURL: "example.com/server_page_url",
        }

        options = $.extend(defaults, options);
        console.log("OPTIONS: " + defaults['serverURL']);

        return this.each(function (i, obj) {
            console.log("INITIALIZE PLUGIN " + i);

            var $this = $(this);
            $this.wrap('<div class="plugin_wrapper" />');

            $this.addClass('aquarium-container');

            $('<h2>My Aquariums</h2>' +
                '<textarea class="aquarium_name_textarea"></textarea>' +
                '<input type="submit" value="add Aquarium" class="aquarium_name_submit" />').insertBefore($this);
            var $submitButton = $('.aquarium_name_submit', $this.parent());

            $submitButton.on("click", function (event) {
                //alert("To Do Submitted");
                sendAquarium($this);
            });

            loadAquarium($this);

        });


        function sendAquarium($el) {
            console.log($el);
            var $this = $el;
            console.log("sendAquarium");
            request_type = "insert";
            var $aquariumText = $this.parent().find('.aquarium_name_textarea');
            var aquariumText = $aquariumText.val();

            console.log("TODOTEXT: " + aquariumText);
            $aquariumText.val("");

            if (aquariumText.length > 2) {
                var request = $.ajax({
                    url: options.serverURL,
                    type: "POST",
                    data: {"name": aquariumText, "action": request_type},
                    dataType: "json",
                });

                request.done(function (data) {
                    console.log("REQUEST.DONE: " + data)
                    handleInsert(data, $this);
                });

                request.fail(
                    function (jqXHR, textStatus) {
                        alert("Request failed: " + textStatus);
                    });

            }

        }


        function handleInsert(data, $el) {
            console.log("aquarium added");
            var $this = $el;

            var aquariums = data["aquariums"];

            var html = "";

            if (
                !($(".aquarium-list", $this).length > 0)
            ) {
                var aquariumList = $('<ul class="aquarium-list"></ul>');
                $this.append(aquariumList);
            }

            if ($('p', $this).length > 0) {
                $('p', $this).remove();
            }

            html += "<li data-id='aquarium_" + aquariums[0]['id'] + "'><span class='aquarium_name_text'>" + aquariums[0]['name'] + "</span> <span class='deleter'>x</span></li>\n";//lo span 1 contiene il testo span 2 contiene la x per fare il delete
            $('.aquarium-list', $this).prepend(html);


        }

        function loadAquarium($el) {
            var $this = $el;
            console.log("loadAquarium");
            request_type = "load";

            var request = $.ajax({
                url: options.serverURL,
                type: "POST",
                data: {"action": request_type},
                dataType: "json",
            });

            request.done(function (data) {
                handleLoad(data, $this);
            });

            request.fail(
                function (jqXHR, textStatus) {
                    alert("Request failed: " + textStatus);
                });
        }


        function handleLoad(data, $el) {
            console.log("handleLoad");

            $this = $el;
            var aquariums = data["aquariums"];
            var $aquariumList = $("<ul class='aquarium-list'>");
            var html = "";


            if (aquariums.length > 0) {

                $this.append($aquariumList);

                $(aquariums).each(function (index, object) {

                    html += "<li data-id='aquarium_" + object['id'] + "'" + "<span class='aquarium_text'>" + object['name']
                        + "</span> <span class='deleter'>x</span></li>\n";
                });

                $aquariumList.append($(html));

            } else {
                htmlS = "<p>Add an Aquarium</p>";
                $this.html(htmlS);
            }

            $($this).on('click', '.deleter', function () {

                var id = $(this).parent().data("id");
                var $parent = $(this).parents(".aquarium_plugin");
                alert("delete class: " + id)
                deleteAquarium($parent, id);
            });
        }

       /* function updateAquarium($el, id, status) {
            var $this = $el;
            console.log("updateToDo");
            request_type = "update";

            var request = $.ajax({
                url: options.serverURL,
                type: "POST",
                //contentType: 'application/json; charset=utf-8',
                data: {"action": request_type, "status": status, "id": id},
                dataType: "json",
                //headers: {"Content-type" :"application/x-www-form-urlencoded"},
            });

            request.done(function (data) {
                //console.log("done: " + msg)
                handleUpdate(data, $this);
            });

            request.fail(
                function (jqXHR, textStatus) {
                    alert("Request failed: " + textStatus);
                });

        }


        function handleUpdate(data, $el) {
            if (data['updated'] == true) {
                var elemToBeUpdated = $('[data-id="' + data['id'] + '"]', $el);
                elemToBeUpdated.toggleClass("completed");
            }
        }*/


        function deleteAquarium($el, id) {
            var $this = $el;
            console.log("deleteToDo");
            request_type = "delete";


            var request = $.ajax({
                url: options.serverURL,
                type: "POST",
                //contentType: 'application/json; charset=utf-8',
                data: {"action": request_type, "id": id},
                dataType: "json",
                //headers: {"Content-type" :"application/x-www-form-urlencoded"},
            });

            request.done(function (data) {
                //console.log("done: " + msg)
                handleDelete(data, $this);
            });

            request.fail(
                function (jqXHR, textStatus) {
                    alert("Request failed: " + textStatus);
                });
        }

        function handleDelete(data, $el) {
            if (data['deleted'] == true) {

                var elemToBeDeleted = $('[data-id="' + data['id'] + '"]', $el); //$("." + data['id'], $el);
                elemToBeDeleted.fadeOut(500, function () {
                    $(this).remove();
                    var aquariumList = $('.aquarium-list li', $el);
                    if (!(aquariumList.length > 0)) {

                        $el.html("<p>Add an Aquarium!</p>");
                    }
                });


            }
        }


    }

})(jQuery);