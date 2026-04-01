




#include "../ui.h"

// #include "ui_S_page01.h"



lv_obj_t * ui_S_page02_screen = NULL;















// 页面切换回调函数
static void back_to_page1_event_cb(lv_event_t *e) {
    lv_disp_load_scr(ui_S_page01_screen);
}

void ui_S_page02_screen_init(void) {
    ui_S_page02_screen = lv_obj_create(NULL);
    
    // 创建返回按钮
    lv_obj_t *back_btn = lv_button_create(ui_S_page02_screen);
    lv_obj_set_size(back_btn, 120, 40);
    lv_obj_align(back_btn, LV_ALIGN_CENTER, 0, 0);
    
    // 创建按钮标签
    lv_obj_t *back_btn_label = lv_label_create(back_btn);
    lv_label_set_text(back_btn_label, "Back to Page 1");
    lv_obj_center(back_btn_label);
    
    // 添加点击事件回调
    lv_obj_add_event_cb(back_btn, back_to_page1_event_cb, LV_EVENT_CLICKED, NULL);
}



void ui_S_page02_screen_destroy(void) {
    if(ui_S_page02_screen) lv_obj_del(ui_S_page02_screen);
    ui_S_page02_screen = NULL;
}

void ui_S_page02_screen_relocalize(void) {
    // Add relocalization code if needed
}