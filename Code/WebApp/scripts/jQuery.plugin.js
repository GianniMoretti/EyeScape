// JavaScript Document

(function($){ //risposta a chiamata in init
    console.log("JQUERY: " + $); //tipo risposta su web di jquery


    $.fn.todo = function(options) { //In jQuery, the fn property is just an alias to the prototype property. The jQuery identifier (or $) is just a constructor function, and all instances created with it, inherit from the constructor's prototype.
        console.log("CALL PLUGIN TODO"); //bo
        var defaults = {
            serverURL : "example.com/server_page_url", //bo
        }
        options = $.extend(defaults, options); //The extends keyword is used in class declarations or class expressions to create a class that is a child of another class.
        // cosa cazzo serve porco dio
        console.log("OPTIONS: " + defaults['serverURL']); // logga col server?

        // for each item in the wrapped set
        return this.each(function(i, obj) {
            console.log("INITIALIZE PLUGIN " + i);

            // cache "this."
            var $this = $(this);

            // Wrap "this" in a div with a class of "plugin_wrapper"
            $this.wrap('<div class="plugin_wrapper" />');

            $this.addClass('to-do-list-container');

            $('<h2>Il mio titolo del cazzo</h2>' +
                '<textarea class="todo_textarea"></textarea>' +
                '<input type="submit" value="bottone del cazzo miao" class="to_do_submit" />').insertBefore($this);

            var $submitButton = $('.to_do_submit', $this.parent());

            $submitButton.on("click", function(event){
                alert("To Do Submitted");
                sendToDo($this);
            });

            loadToDo($this);

        });


        function sendToDo($el) { // richiesta di aggiunta che viene effettuata nel php
            console.log($el);
            var $this = $el;
            console.log("sendToDo");
            request_type = "insert"; //******************************************
            var $todoText = $this.parent().find('.todo_textarea');
            var todoText = $todoText.val();
            console.log("TODOTEXT: " + todoText);
            $todoText.val("");

            if (todoText.length > 2) {

                var request = $.ajax({
                    url: options.serverURL,
                    type: "POST",
                    data: {"text" : todoText, "action" : request_type},
                    dataType: "json",
                });

                request.done(function(data) {
                    console.log("REQUEST.DONE: " + data)
                    handleInsert(data, $this);
                });

                request.fail(
                    function(jqXHR, textStatus) {
                        alert( "Request failed: " + textStatus );
                    });

            }

        } // end sendToDo


        function handleInsert(data, $el) {
            console.log("to do added");
            var $this = $el;
            // trova tutti i todo, in questo caso uno
            var todos = data["todos"];

            // crea una variabile per la risposta
            var html="";

            if (
                !($(".todo-list", $this).length > 0)
            ) {
                var toDoList = $('<ul class="todo-list"></ul>');
                $this.append(toDoList);
            }

            if ($('p', $this).length > 0) {
                $('p', $this).remove();
            }

            html += "<li data-id='todo_" + todos[0]['id'] + "'><span class='todo_text'>" + todos[0]['text'] + "</span> <span class='deleter'>x</span></li>\n";
            $('.todo-list', $this).prepend(html);




        } // handleInsert

        function loadToDo($el) {
            var $this = $el;
            console.log("loadToDo");
            request_type = "load";


            var request = $.ajax({
                url: options.serverURL,
                type: "POST",
                //contentType: 'application/json; charset=utf-8',
                data: {"action" : request_type},
                dataType: "json",
                //headers: {"Content-type" :"application/x-www-form-urlencoded"},
            });

            request.done(function(data) {
                //console.log("done: " + msg)
                handleLoad(data, $this);
            });

            request.fail(
                function(jqXHR, textStatus) {
                    alert( "Request failed: " + textStatus );
                });
        }


        function handleLoad(data, $el) {
            console.log("handleLoad");

            $this = $el;
            var todos = data["todos"];
            var $toDoList= $("<ul class='todo-list'>");
            var html = "";


            if (todos.length > 0) {

                $this.append($toDoList);

                $(todos).each(function(index, object) {

                    html += "<li data-id='todo_" + object['id'] + "'" + ((object['completed'] === "1") ? "class='completed'>" : ">")  + "<span class='todo_text'>" + object['text']
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
            $($this).on('click', '.deleter', function() {

                var id = $(this).parent().data("id");
                var $parent = $(this).parents(".todo_plugin");
                alert("delete class: " + id)
                deleteToDo($parent, id);
            });

            $($this).on('dblclick', '.todo_text', function() {

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
                data: {"action" : request_type, "status" : status, "id": id},
                dataType: "json",
                //headers: {"Content-type" :"application/x-www-form-urlencoded"},
            });

            request.done(function(data) {
                //console.log("done: " + msg)
                handleUpdate(data, $this);
            });

            request.fail(
                function(jqXHR, textStatus) {
                    alert( "Request failed: " + textStatus );
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
                data: {"action" : request_type, "id" : id},
                dataType: "json",
                //headers: {"Content-type" :"application/x-www-form-urlencoded"},
            });

            request.done(function(data) {
                //console.log("done: " + msg)
                handleDelete(data, $this);
            });

            request.fail(
                function(jqXHR, textStatus) {
                    alert( "Request failed: " + textStatus );
                });
        }

        function handleDelete(data, $el) {
            if (data['deleted'] == true) {

                var elemToBeDeleted = $('[data-id="' + data['id'] + '"]', $el); //$("." + data['id'], $el);
                elemToBeDeleted.fadeOut(500, function() {
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