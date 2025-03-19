//
// Created by 张文兵 on 2025/3/19.
//

#ifndef GUI_H
#define GUI_H

namespace GUI {
#ifdef _MSC_VER
    void createMenu(HWND hWnd);
    void createTray(HWND hWnd);
#else
    void createMenu();
    void createTray();
#endif
}

#endif //GUI_H
