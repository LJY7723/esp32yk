#line 1 "D:\\BaiduSyncdisk\\esp32yk\\yk2\\1.ControllerSystem\\libraries\\lvgl\\env_support\\rt-thread\\squareline\\lv_ui_entry.c"
/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author        Notes
 * 2022-05-13     Meco Man      First version
 */

#ifdef __RTTHREAD__

void lv_user_gui_init(void)
{
    extern void ui_init(void);
    ui_init();
}

#endif /* __RTTHREAD__ */
