
#ifndef UI_S_PAGE01_H
#define UI_S_PAGE01_H

#ifdef __cplusplus
extern "C" {
#endif


void display_64_values(lv_obj_t *parent, uint16_t values[64], uint64_t color_flags);

extern void ui_S_page01_screen_init(void);
extern void ui_S_page01_screen_destroy(void);
extern void ui_S_page01_screen_relocalize(void);

extern lv_obj_t * ui_S_page01_screen;

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif