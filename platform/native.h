//
// Created by 张文兵 on 2025/2/14.
//

#ifndef APP_NATIVE_H
#define APP_NATIVE_H

class Native {
public:
#ifdef _MSC_VER
    Native(HWND hWnd);  // 构造函数
#else
    Native();  // 构造函数
#endif
    ~Native(); // 析构函数

    // 创建菜单
    void createMenu();
    void createTray();

private:
#ifdef _MSC_VER
    HWND m_hWnd;
#endif
};

#endif // APP_NATIVE_H
