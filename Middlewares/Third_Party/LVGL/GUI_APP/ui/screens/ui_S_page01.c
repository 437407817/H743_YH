


#include "../ui.h"

// #include "ui_S_page01.h"



lv_obj_t * ui_S_page01_screen = NULL;



// 键盘事件回调函数
static void keyboard_event_cb(lv_event_t *e) {
    lv_obj_t *ta = lv_event_get_target(e);
    lv_obj_t *kb = lv_event_get_user_data(e);
    lv_keyboard_set_textarea(kb, ta);
    lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);
}

// 点击外部事件回调函数
static void click_outside_event_cb(lv_event_t *e) {
    lv_obj_t *kb = lv_event_get_user_data(e);
    if(!lv_obj_has_flag(kb, LV_OBJ_FLAG_HIDDEN)) {
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
    }
}

// 按钮1点击回调函数
static void btn1_event_cb(lv_event_t *e) {
    printf("Button 1 clicked\n");
}

// 按钮2点击回调函数
static void btn2_event_cb(lv_event_t *e) {
    printf("Button 2 clicked\n");
}

// 按钮3点击回调函数
static void btn3_event_cb(lv_event_t *e) {
    printf("Button 3 clicked\n");
}

// 页面切换回调函数
static void switch_page_event_cb(lv_event_t *e) {
    lv_disp_load_scr(ui_S_page02_screen);
}
// 显示64个数值控件的函数
void display_64_values(lv_obj_t *parent, uint16_t values[64], uint64_t color_flags) {
    // 网格布局参数
    const int grid_size = 8; // 8x8网格
    const int cell_size = 60; // 每个单元格大小
    const int padding = 5; // 单元格间距
    const int max_value = 5000; // 最大值
    
    // 创建容器用于放置所有数值控件
    lv_obj_t *container = lv_obj_create(parent);
    // lv_obj_set_size(container, grid_size * (cell_size + padding) - padding, 
    //                grid_size * (cell_size + padding) - padding);
    lv_obj_set_size(container, lv_pct(100), lv_pct(100));
    lv_obj_align(container, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_flex_flow(container, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(container, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
    
    // 创建64个数值控件
    for (int i = 0; i < 64; i++) {
        // 创建单个数值控件的容器
        lv_obj_t *cell = lv_obj_create(container);
        lv_obj_set_size(cell, cell_size, cell_size);
        lv_obj_set_style_border_width(cell, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_color(cell, lv_color_hex(0xCCCCCC), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_opa(cell, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_all(cell, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
        
        // 检查颜色标志位（0-63位对应64个控件）
        uint64_t bit_mask = (uint64_t)1 << i;
        
        // 设置背景颜色
        if (color_flags & bit_mask) {
            // 蓝色
            lv_obj_set_style_bg_color(cell, lv_color_hex(0x0000FF), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(cell, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
        } else {
            // 白色
            lv_obj_set_style_bg_color(cell, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(cell, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        
        // 添加标号（1-64）
        lv_obj_t *index_label = lv_label_create(cell);
        lv_label_set_text_fmt(index_label, "%d", i + 1); // 从1开始编号
        lv_obj_align(index_label, LV_ALIGN_TOP_LEFT, 2, 2);
        lv_obj_set_style_text_font(index_label, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
        
        // 创建标签显示数值，转换为电压格式（除以1000，显示为X.XXXV）
        lv_obj_t *label = lv_label_create(cell);
        float voltage = values[i] / 1000.0f;
        lv_label_set_text_fmt(label, "%.3fV", voltage);
        lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
        lv_obj_set_style_text_font(label, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    }
}

void ui_S_page01_screen_init(void) {
    ui_S_page01_screen = lv_obj_create(NULL);
        // 创建滚动视图，高度为屏幕的80%
    lv_obj_t *scroll_view = lv_obj_create(ui_S_page01_screen);
    lv_obj_set_size(scroll_view, lv_pct(100), lv_pct(80));
    lv_obj_align(scroll_view, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(scroll_view, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    
    // 示例数据：创建64个uint16数值
    uint16_t test_values[64];
    for (int i = 0; i < 64; i++) {
        // 生成0-5000之间的随机数作为测试数据
        test_values[i] = (uint16_t)(rand() % 5001);
    }
    
    // 示例颜色标志：低1位和最高位为1，其他为0
    // 使用uint64_t可以表示64个控件的颜色状态
    // 这里我们设置第一个和最后一个控件为蓝色，其他为白色
    uint64_t test_color_flags = ((uint64_t)1 << 0) | ((uint64_t)1 << 63); // 第一个和最后一个控件为蓝色
    
    // 调用函数显示64个数值控件，使用滚动视图作为父对象
    display_64_values(scroll_view, test_values, test_color_flags);
    
    // 在滚动视图下方添加控件
    lv_obj_t *btn_container = lv_obj_create(ui_S_page01_screen);
    lv_obj_set_size(btn_container, lv_pct(100), lv_pct(20));
    lv_obj_align(btn_container, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_flex_flow(btn_container, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(btn_container, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    
    // 创建第一个按钮
    lv_obj_t *btn1 = lv_button_create(btn_container);
    lv_obj_set_size(btn1, 120, 40);
    lv_obj_t *btn1_label = lv_label_create(btn1);
    lv_label_set_text(btn1_label, "start");
    lv_obj_center(btn1_label);
    // 添加点击事件回调
    lv_obj_add_event_cb(btn1, btn1_event_cb, LV_EVENT_CLICKED, NULL);
    
    // 创建第二个按钮
    lv_obj_t *btn2 = lv_button_create(btn_container);
    lv_obj_set_size(btn2, 120, 40);
    lv_obj_t *btn2_label = lv_label_create(btn2);
    lv_label_set_text(btn2_label, "stop");
    lv_obj_center(btn2_label);
    // 添加点击事件回调
    lv_obj_add_event_cb(btn2, btn2_event_cb, LV_EVENT_CLICKED, NULL);
    
    // 创建单选框和文本
    lv_obj_t *radio_container = lv_obj_create(btn_container);
    lv_obj_set_flex_flow(radio_container, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(radio_container, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    
    // 创建单选框
    lv_obj_t *radio = lv_checkbox_create(radio_container);
    lv_checkbox_set_text(radio, ""); // 不设置文本，文本将单独创建
    
    // 创建文本（内容为auto）
    lv_obj_t *auto_label = lv_label_create(radio_container);
    lv_label_set_text(auto_label, "auto");
    lv_obj_set_style_pad_left(auto_label, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    
    // 创建键盘并初始隐藏
    lv_obj_t *kb = lv_keyboard_create(ui_S_page01_screen);
    lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN); // 初始隐藏键盘
    // 设置键盘模式为数字模式
    lv_keyboard_set_mode(kb, LV_KEYBOARD_MODE_NUMBER);
    // 设置键盘模式为数字模式

    
    // 创建第一个数值输入框
    lv_obj_t *input1 = lv_textarea_create(btn_container);
    lv_obj_set_size(input1, 100, 40);
    lv_textarea_set_placeholder_text(input1, "Stop Vol");
    lv_textarea_set_max_length(input1, 5); // 限制输入长度
    
    // 创建第二个数值输入框
    lv_obj_t *input2 = lv_textarea_create(btn_container);
    lv_obj_set_size(input2, 100, 40);
    lv_textarea_set_placeholder_text(input2, "Vol Dif");
    lv_textarea_set_max_length(input2, 5); // 限制输入长度
    
    // 添加新按钮
    lv_obj_t *btn3 = lv_button_create(btn_container);
    lv_obj_set_size(btn3, 120, 40);
    lv_obj_t *btn3_label = lv_label_create(btn3);
    lv_label_set_text(btn3_label, "Button 3");
    lv_obj_center(btn3_label);
    // 添加点击事件回调
    lv_obj_add_event_cb(btn3, btn3_event_cb, LV_EVENT_CLICKED, NULL);
    
    // 为输入框添加事件处理，当获得焦点时显示键盘
    lv_obj_add_event_cb(input1, keyboard_event_cb, LV_EVENT_FOCUSED, kb);
    lv_obj_add_event_cb(input2, keyboard_event_cb, LV_EVENT_FOCUSED, kb);
    
    // 为屏幕添加事件处理，当点击屏幕时隐藏键盘
    lv_obj_add_event_cb(ui_S_page01_screen, click_outside_event_cb, LV_EVENT_CLICKED, kb);
    
    // 为键盘添加事件处理，当点击完成按钮时隐藏键盘
    lv_obj_add_event_cb(kb, click_outside_event_cb, LV_EVENT_READY, kb);
    
    // 为键盘添加事件处理，当点击完成按钮时隐藏键盘
    lv_obj_add_event_cb(kb, click_outside_event_cb, LV_EVENT_READY, kb);


// 创建页面切换按钮
lv_obj_t *switch_btn = lv_button_create(btn_container);
lv_obj_set_size(switch_btn, 120, 40);
lv_obj_t *switch_btn_label = lv_label_create(switch_btn);
lv_label_set_text(switch_btn_label, "Go to Page 2");
lv_obj_center(switch_btn_label);

// 添加点击事件回调
lv_obj_add_event_cb(switch_btn, switch_page_event_cb, LV_EVENT_CLICKED, NULL);

    
}

void ui_S_page01_screen_destroy(void) {
    if(ui_S_page01_screen) lv_obj_del(ui_S_page01_screen);
    ui_S_page01_screen = NULL;
}

void ui_S_page01_screen_relocalize(void) {
    // Add relocalization code if needed
}