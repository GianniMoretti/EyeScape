// JavaScript Document






(function ($) { //risposta a chiamata in init
    console.log("JQUERY: " + $); //tipo risposta su web di jquery


    $.fn.todo = function (options) { //In jQuery, the fn property is just an alias to the prototype property. The jQuery identifier (or $) is just a constructor function, and all instances created with it, inherit from the constructor's prototype.
        console.log("CALL PLUGIN TODO"); //bo
        var defaults = {
            serverURL: "example.com/server_page_url", //bo TODO da capire!
        }
        options = $.extend(defaults, options); //The extends keyword is used in class declarations or class expressions to create a class that is a child of another class.
        // cosa cazzo serve porco dio
        console.log("OPTIONS: " + defaults['serverURL']); // logga col server?

        // for each item in the wrapped set
        return this.each(function (i, obj) {
            console.log("INITIALIZE PLUGIN " + i);

            // cache "this."
            var $this = $(this);

            // Wrap "this" in a div with a class of "plugin_wrapper"
            $this.wrap('<div class="plugin_wrapper" />');

            $this.addClass('aquarium-container');

            $('<p class="text-center">I miei acquari</p>' +
                '<div class="container text-center">\n' +
                '  <div class="row row-cols-2" id="append-div">\n' +
                '    <div class="col"><button type=\'button\' class=\'btn btn-primary\'>Acquario 1</button></div>\n' +
                '    <div class="col"><button type=\'button\' class=\'btn btn-primary\'>Acquario 2</button></div>\n' +
                '    <div class="col"><button type=\'button\' class=\'btn btn-primary\'>Acquario 3</button></div>\n' +
                '    <div class="col"><button type=\'button\' class=\'btn btn-primary\'>Acquario 4</button></div>\n' +
                '  </div>\n' +
                '</div>').insertBefore($this);


            var $submitButton = $('.btn', $this.parent());

            $submitButton.on("click", function (event) {
                var request_type = "insert";
                var text = "prova";
                $.ajax({
                    type: "POST",
                    url: "../Server/Actions.php",
                    data: {"text": text, "action": request_type},
                    dataType:'JSON',
                    success: function(response){
                        var html = "<div class='col'><button type='button' class='btn btn-secondary'>ah! GAYY</button></div>";
                        $("#append-div").append(html);
                    },
                    error : function(response) {
                        console.log(response);
                    }
                });
            });

            loadToDo($this);

        });


        function loadToDo($el) {
            var $this = $el;
            console.log("loadToDo");
            request_type = "load";


            var request = $.ajax({
                url: options.serverURL,
                type: "POST",
                //contentType: 'application/json; charset=utf-8',
                data: {"action": request_type},
                dataType: "json",
                //headers: {"Content-type" :"application/x-www-form-urlencoded"},
            });

            request.done(function (data) {
                //console.log("done: " + msg)
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
            var todos = data["todos"];
            var $toDoList = $("<ul class='todo-list'>");
            var html = "";


            if (todos.length > 0) {

                $this.append($toDoList);

                $(todos).each(function (index, object) {

                    html += "<li data-id='todo_" + object['id'] + "'" + ((object['completed'] === "1") ? "class='completed'>" : ">") + "<span class='todo_text'>" + object['text']
                        + "</span> <span class='deleter'>x</span></li>\n";
                });

                $toDoList.append($(html));
                //addDeleteListener();
                //addUpdateListener();*/
            } else {
                htmlS = "<p>Add a to do!</p>";
                $this.html(htmlS);
            }

            // aggiunta listener sui nuovi elementi
            $($this).on('click', '.deleter', function () {

                var id = $(this).parent().data("id");
                var $parent = $(this).parents(".todo_plugin");
                alert("delete class: " + id)
                deleteToDo($parent, id);
            });

            $($this).on('dblclick', '.todo_text', function () {

                var id = $(this).parent().data("id");
                var $parent = $(this).parents(".todo_plugin");
                var $firstFather = $(this).parent();
                var status = !$firstFather.hasClass('completed');
                alert("update id: " + status)
                updateToDo($parent, id, status);
            });


        }

        function updateToDo($el, id, status) {
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
        }


        function deleteToDo($el, id) {
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
                    var toDoList = $('.todo-list li', $el);
                    if (!(toDoList.length > 0)) {

                        $el.html("<p>Add a to do!</p>");
                    }
                });


            }
        }


    }

})(jQuery);