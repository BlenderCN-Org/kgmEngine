var id_num = 0;

var gui_selected = null;

var guis = []

function newId()
{
	return id_num++;
}

function gui_sizer_show()
{
    $('#gui_size').show();
}

function gui_sizer_hide()
{
    //$('#gui_size').hide();
}

function gui_sizer_move(x, y)
{
    $('#gui_size').css('left', x);
    $('#gui_size').css('top', y);
}

function gui_sizer_drag(e)
{

}

function gui_get_by_target(g)
{
    var i;

    for(i = 0; i < guis.length; i++)
    {
        var c = guis[i];

        if (c.target == g)
            return c;
    }

    return null;
}

function gui_options(gui)
{
    if (gui == null || gui_selected == gui)
        return;

    gui_selected = gui;

    var gui_data = gui_get_by_target(gui);

    $("#options").empty();

    $table = $("<table id='tboptions'></table>");
    $("#options").append($table);

    $table.append("<tr><td>x</td><td><input size='15' id='gui_x' type='text' value='" + gui.position().left + "'/></td></tr>");
    $table.append("<tr><td>y</td><td><input size='15' id='gui_y' type='text' value='" + gui.position().top + "'/></td></tr>");
    $table.append("<tr><td>w</td><td><input size='15' id='gui_w' type='text' value='" + gui.width() + "'/></td></tr>");
    $table.append("<tr><td>h</td><td><input size='15' id='gui_h' type='text' value='" + gui.height() + "'/></td></tr>");
    $table.append("<tr><td>handler</td><td><input size='15' id='gui_handler' type='text' value='" +
                  gui_data.handler + "'/></td></tr>");

    if(gui_data.type != 'list' && gui_data.type != 'menu' && gui_data.type != 'progress')
        $table.append("<tr><td>text</td><td><input size='15' id='gui_text' type='text' value='" +
                      gui_data.text + "'/></td></tr>");

    $('#gui_x').on('input', function(){
        gui_selected.offset().left = parseInt($('#gui_x').val());
    });
    $('#gui_y').on('input', function(){
        gui_selected.offset().top = parseInt($('#gui_y').val());
    });
    $('#gui_w').on('input', function(){
        gui_selected.width(parseInt($('#gui_w').val()));
    });
    $('#gui_h').on('input', function(){
        gui_selected.height(parseInt($('#gui_h').val()));
    });
    $('#gui_text').on('input', function(){
        var c = gui_get_by_target(gui_selected);
        c.text = $('#gui_text').val();

        if (c.type == 'text')
            c.target.val(c.text);
        else if (c.type == 'button')
            c.target.text(c.text);
    });
    $('#gui_handler').on('input', function(){
        var c = gui_get_by_target(gui_selected);
        c.handler = $('#gui_handler').val();
    });

    var x = $('#palette').position().x + gui.position().left + gui.width();
    var y = $('#palette').position().y + gui.position().top  + gui.height();

    gui_sizer_move(x, y);
    gui_sizer_show();
}

function gui_list_menu_add()
{
    $("#gui_list_menu").hide();
    $("#gui_list_menu_add_dialog").dialog('open');
}

function gui_list_menu_remove()
{
    $("#gui_list_menu").hide();

    gui_selected.each(function(i, el){
        for (var j = el.options.length; j > 0; j--)
        {
            var o = this.options[j - 1];

            if (o.selected)
            {
                $(o).remove();
            }
        }
    });
}

function gui_list_menu_dragging()
{
    $("#gui_list_menu").hide();

    var c = gui_get_by_target(gui_selected);

    if (c.dragging == undefined)
    {
        c.dragging = true;
    }
    else
    {
        c.dragging = !c.dragging;
    }

    if (c.dragging)
    {
        c.target.draggable({cancel: false});
        c.target.draggable('enable');
        $("#gui_list_menu_dragging").text('undrag');
    }
    else
    {
        c.target.draggable();
        c.target.draggable('disable');
        $("#gui_list_menu_dragging").text('drag');
    }
}

function gui_list_add_text()
{
    gui_selected.append("<option>" + $("#gui_list_text_for_add").val() + "</option>");
    $("#gui_list_menu_add_dialog").dialog('close');
}

function gui_list_remove_text()
{
}

function gui_list_context(e)
{
    if (gui_selected == null)
        return;

    $("#gui_list_menu").show();
    $("#gui_list_menu").css('left', e.clientX);
    $("#gui_list_menu").css('top', e.clientY);

}

function gui_menu_context(e)
{
    alert('Implementing');
}

function new_gui()
{
    var id = "gui" + newId();
	var $gui = $ ("<div class='kgm_base kgm_gui'>gui</div>");
	$("#palette").append($gui);
    $gui.draggable();
    $gui.click(function(){
        gui_options($gui);
    });

    var g = { target: $gui,
              id: id,
              type: 'gui'
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

    $menu.contextmenu(gui_menu_context);

    var g = { target: $menu,
              id: id,
              type: 'menu'
            };

    guis.push(g);
}

function new_list()
{
    var id = "list" + newId();
	var $list = $ ("<select id='" + id + "' class='kgm_base kgm_list'size='5'></select>");
	$("#palette").append($list);
    $list.draggable(); //{cancel: false}
    $list.click(function(){
        gui_options($list);
    });
    $list.contextmenu(gui_list_context);

    var g = { target: $list,
              id: id,
              type: 'list'
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
              id: id,
              type: 'text'
            };

    guis.push(g);
}

function new_check()
{
    var id = "check" + newId();
    var $div = $("<div id='div" + id + "'  class='ui-checkboxradio kgm_base kgm_check'></div>");
    $div.append($ ("<input class='' type='checkbox'>Check</input>"));
    $("#palette").append($div);
    $div.draggable({cancel: false});
    $div.click(function(){
        gui_options($div);
    });

    var g = { target: $div,
              id: id,
              type: 'check'
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
              id: id,
              type: 'label'
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

    var g = { target: $button,
              id: id,
              type: 'button'
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
              id: id,
              type: 'progress'
            };

    guis.push(g);
}

function on_palette()
{
    selected = null;

    gui_sizer_hide();
}

function kgm_init()
{
    //$("#new_gui").click(new_gui);
    //$("#new_menu").click(new_menu);
    $("#new_list").click(new_list);
    $("#new_text").click(new_text);
    $("#new_check").click(new_check);
    $("#new_label").click(new_label);
    $("#new_button").click(new_button);
    $("#new_progress").click(new_progress);

    $("#palette").click(on_palette);

    $("#gui_size").draggable({
        drag: gui_sizer_drag
    });
    gui_sizer_hide();

    $("#gui_list_menu").menu();
    $("#gui_list_menu_add").click(gui_list_menu_add);
    $("#gui_list_menu_remove").click(gui_list_menu_remove);
    $("#gui_list_menu_dragging").click(gui_list_menu_dragging);
    $("#gui_list_menu_add_dialog").dialog();
    $("#gui_list_menu_add_dialog").dialog('close');
    $("#gui_list_add_text").click(gui_list_add_text);
}
