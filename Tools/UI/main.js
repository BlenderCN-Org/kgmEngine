var id_num = 0;

var selected = null;

var guis = []

function newId()
{
	return id_num++;
}

function gui_options(gui)
{
    if (gui == null || selected == gui)
        return;

    $("#options").empty();

    $table = $("<table id='tboptions'></table>");
    $("#options").append($table);

    $table.append("<tr><td>x</td><td>" + gui.position().left + "</td></tr>");
    $table.append("<tr><td>y</td><td>" + gui.position().top + "</td></tr>");

    selected = gui;
}

function new_gui()
{
    var id = "gui" + newId();
	var $gui = $ ("<div class='kgm_base kgm_gui'>gui</div>");
	$("#palette").append($gui);
    $gui.draggable();
    $gui.onclick(function(){
        gui_options($gui);
    });

    var g = { target: $gui,
              id: id
            };

    guis.push(g);
}

function new_menu()
{
	var id = 'menu' + newId();
	var $menu = $ ("<div id='" + id +"' class='kgm_base kgm_menu'>menu</div>");
	$("#palette").append($menu);
    $menu.menu()
	$menu.draggable();
    $menu.click(function(){
        gui_options($menu);
    });

    var g = { target: $menu,
              id: id
            };

    guis.push(g);
}

function new_list()
{
    var id = "list" + newId();
	var $list = $ ("<select id='" + id + "' class='kgm_base kgm_list' size='5'></select>");
	$("#palette").append($list);
    $list.draggable({cancel: false});
    $list.click(function(){
        gui_options($list);
    });

    var g = { target: $list,
              id: id
            };

    guis.push(g);
}

function new_text()
{
    var id = "text" + newId();
	var $text = $ ("<input id='" + id + "' class='kgm_base kgm_text' type='text' value='Text'></input>");
	$("#palette").append($text);
    $text.draggable({cancel: false});
    $text.click(function(){
    selected = $text;
        gui_options($text);
    });

    var g = { target: $text,
              id: id
            };

    guis.push(g);
}

function new_check()
{
    var id = "check" + newId();
    var $div = $("<div id='div" + id + "'></div>");
    var $check = $ ("<input id='" + id + "' class='ui-checkboxradio kgm_base kgm_check' type='checkbox' value='Check'>Check</input>");
    $div.append($check);
    $("#palette").append($div);
    $div.draggable();
    $check.click(function(){
        gui_options($check);
    });

    var g = { target: $check,
              id: id
            };

    guis.push(g);
}

function new_label()
{
    var id = "label" + newId();
    var $label = $("<div id='" + id + "' class='ui-state-highlight kgm_base kgm_label'>Label</div>");
    $("#palette").append($label);
    $label.draggable();
    $label.click(function(){
        gui_options($label);
    });

    var g = { target: $label,
              id: id
            };

    guis.push(g);
}

function new_button()
{
    var id = "button" + newId();
	var $button = $ ("<button id='" + id + "' class='ui-button kgm_base kgm_button'>Button</button>");
	$("#palette").append($button);
    $button.draggable({cancel: false});
    $button.click(function(){
        gui_options($button);
    });

    var g = { target: $label,
              id: id
            };

    guis.push(g);
}

function new_progress()
{
    var id = "progress" + newId();
	var $progress = $ ("<div id='" + id + "' class='kgm_base kgm_progress'></div>");
	$("#palette").append($progress);
    $progress.progressbar({value: 45});
    $progress.draggable();
    $progress.click(function(){
        gui_options($progress);
    });

    var g = { target: $progress,
              id: id
            };

    guis.push(g);
}

function on_palette()
{
    selected = null;
}

function kgm_init()
{
  //$("#new_gui").click(new_gui);
  $("#new_menu").click(new_menu);
  $("#new_list").click(new_list);
  $("#new_text").click(new_text);
  $("#new_check").click(new_check);
  $("#new_label").click(new_label);
  $("#new_button").click(new_button);
  $("#new_progress").click(new_progress);

  $("#palette").click(on_palette);
}
