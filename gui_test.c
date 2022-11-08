#include "gui_test.h"

#include "e_widget.h"
#include "e_widget_text.h"
#include "e_widget_window.h"
#include "e_widget_button.h"
#include "e_widget_combobox.h"
#include "e_widget_list.h"
#include "e_widget_range.h"
#include "e_widget_entry.h"
#include "e_widget_entry_area.h"
#include "e_widget_image.h"
#include "engine.h"

#include "e_math.h"
#include "tools.h"

#include "resource.h"

EWidgetWindow window;
EWidgetWindow window2;
EWidgetButton button;
EWidgetCombobox combobox;
EWidgetRange range;
EWidgetList list;
EWidgetEntry entry;
EWidgetEntry entry2;
EWidgetEntryArea entry_area;
EWidgetImage img;
EWidget el1;
EWidget el2;
EWidget el3;
EWidgetText wt;

vec2 mouse, temp ;

void TempPress(EWidget* widget, void* args)
{

    double xpos, ypos;

    EngineGetCursorPos(&xpos, &ypos);
    mouse.x = xpos;
    mouse.y = ypos;

    temp = Transform2DGetPosition(widget);
}

void TempMove(EWidget* widget, void* args)
{

    vec2 te;
    double xpos, ypos;

    EngineGetCursorPos(&xpos, &ypos);
    te.x = xpos;
    te.y = ypos;

    te = v2_sub(te, mouse);
    te = v2_add(temp, te);

    Transform2DSetPosition(widget, te.x, te.y);

}

void GUIInit(){

    vec2 position = {100, 100};
    vec2 size = {400, 300};

    DrawParam dParam = {};

    WindowWidgetInit(&window, "Temp window", size, dParam, position);

    ButtonWidgetInit(&button, "Pressss", (vec4){ 0.4, 0.4, 0.4, 1.0}, &window);
    Transform2DSetPosition(&button, 0, 80);
    Transform2DSetScale(&button, 60, 20);

    ListWidgetInit(&list, 60, 20, &window);
    ListWidgetSetColor(&list, (vec4){0.5, 0.1, 0.1, 1.0});
    Transform2DSetPosition(&list, 200, 0);
    Transform2DSetScale(&list, 60, 170);

    ListWidgetAddItem(&list, "Temp 1");
    ListWidgetAddItem(&list, "Temp 2");
    ListWidgetAddItem(&list, "Temp 3");
    ListWidgetAddItem(&list, "Temp 4");
    ListWidgetAddItem(&list, "Temp 5");
    ListWidgetAddItem(&list, "Temp 6");
    ListWidgetAddItem(&list, "Temp 7");
    ListWidgetAddItem(&list, "Temp 8");

    ListWidgetRemoveItem(&list, 2);
    ListWidgetRemoveItem(&list, 3);
    ListWidgetRemoveItem(&list, 4);

    ComboboxWidgetInit(&combobox, &window);
    ComboboxWidgetAddItem(&combobox, "Temp 1");
    ComboboxWidgetAddItem(&combobox, "Temp 2");
    ComboboxWidgetAddItem(&combobox, "Temp 3");
    ComboboxWidgetAddItem(&combobox, "Temp 4");

    Transform2DSetPosition(&combobox, 500, 0);

    RangeWidgetInit(&range, (vec2){60, 20}, 20, 80, &window);
    Transform2DSetPosition(&range, 350, 50);

    EntryWidgetInit(&entry, 9, &window);
    Transform2DSetScale(&entry, 60, 20);
    Transform2DSetPosition(&entry, 350, 100);

    EntryWidgetInit(&entry2, 9, &window);
    Transform2DSetScale(&entry2, 60, 20);
    Transform2DSetPosition(&entry2, 350, 200);

    EntryAreaWidgetInit(&entry_area, 9, &window);
    Transform2DSetScale(&entry_area, 100, 200);
    Transform2DSetPosition(&entry_area, 350, 300);

    WidgetInit(&el1, dParam, &window);
    el1.color = (vec4){0.1, 0.8, 0.1, 1.0};

    Transform2DSetPosition(&el1, 0, 0);
    Transform2DSetScale(&el1, 50, 50);

    WidgetConnect(&el1, GUI_TRIGGER_MOUSE_PRESS, TempPress, NULL);
    WidgetConnect(&el1, GUI_TRIGGER_MOUSE_MOVE, TempMove, NULL);

    ToolsAddStrings(dParam.filePath, 256, path, "/textures/texture.png");
    ImageWidgetInit(&img, dParam, &el1);
    img.widget.color = (vec4){1.0, 1.0, 1.0, 1.0};
    Transform2DSetScale(&img, 100, 100);

    memset(dParam.filePath, 0, 256);

    WidgetInit(&el2, dParam, &el1);
    el2.color = (vec4){0.1, 0.1, 0.3, 1.0};

    Transform2DSetPosition(&el2, 0, 0);
    Transform2DSetScale(&el2, 20, 20);

    WidgetConnect(&el2, GUI_TRIGGER_MOUSE_PRESS, TempPress, NULL);
    WidgetConnect(&el2, GUI_TRIGGER_MOUSE_MOVE, TempMove, NULL);

    TextWidgetInit(&wt, 9,  dParam, &el2);
    wt.tData.font.color = (vec3){0.7, 0.1, 0.1};

    Transform2DSetPosition(&wt, 0, 20);

    TextWidgetSetText(&wt, "Welkome in city 17!");

    position.x = 0;
    position.y = 0;
    size.x = 400;
    size.y = 300;
    WindowWidgetInit(&window2, "Temp window2", size, dParam, position);

    WidgetInit(&el3, dParam, &window2);
    el3.color = (vec4){0.1, 0.1, 0.3, 1.0};

    Transform2DSetPosition(&el3, 0, 0);
    Transform2DSetScale(&el3, 20, 20);

    WindowWidgetHide(&window);
    WindowWidgetHide(&window2);
}


void GUIUpdate(){

    WindowWidgetUpdate(&window2);

    WindowWidgetUpdate(&window);

}

void GUIDraw(){
   WindowWidgetDraw(&window);

   WindowWidgetDraw(&window2);
}

void GUIDestroy(){
    WindowWidgetDestroy(&window);

    WindowWidgetDestroy(&window2);
}
