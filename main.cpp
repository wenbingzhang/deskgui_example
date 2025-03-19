/**
 * deskgui - A powerful and flexible C++ library to create web-based desktop applications.
 *
 * Copyright (c) 2023 deskgui
 * MIT License
 */

#include <deskgui/app.h>
#include <deskgui/resource_compiler.h>

#include "gui/gui.h"
#include  <cpr/cpr.h>


using namespace deskgui;
using namespace deskgui::event;

struct MessageDeserialization
{
    void process(const WebviewOnMessage& event) { std::cout << event.message << std::endl; }
};

int main()
{
    // cpr::Response r = cpr::Get(cpr::Url{"http://www.baidu.com"});
    // std::cout << r.status_code << std::endl;
    // std::cout << r.header["content-type"] << std::endl;
    // std::cout << r.text << std::endl;


    App app("EmbeddedResourcesExample");
    auto window = app.createWindow("window");

    window->setTitle("Embedded web resources example");
    window->setResizable(true);
    window->setMinSize({600, 340});
    window->setSize({1200, 680});
    window->center();

    WebviewOptions options;
    options.setOption(WebviewOptions::kRemoteDebuggingPort, 9222);

    GUI::createMenu();
    GUI::createTray();

    auto webview = window->createWebview("webview", options);
    window->setBackgroundColor(14, 14, 14);

    webview->loadResources(getCompiledResources("assets"));
    webview->serveResource("index.html");

    // webview->serveResource("src/lenna.png"); //try loading a png!

    window->connect<WindowResize>(
        [&webview](const WindowResize& event) { webview->resize(event.size); });

    webview->addCallback("sys_window", [=](std::string_view message)
    {
        if (message.compare("show"))
        {
            window->show();
        }
    });

    // We can listen to all types of messages (including attached callbacks) by connecting to the
    // WebviewOnMessage event.
    MessageDeserialization msgCallback;
    webview->connect<WebviewOnMessage>(&msgCallback, &MessageDeserialization::process);

    // webview->connect<WebviewContentLoaded>([&window]() { window->show(); });

    app.run();
}
