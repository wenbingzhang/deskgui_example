/**
 * deskgui - A powerful and flexible C++ library to create web-based desktop applications.
 *
 * Copyright (c) 2023 deskgui
 * MIT License
 */

#include <deskgui/app.h>
#include <deskgui/resource_compiler.h>

#include "platform/native.h"


using namespace deskgui;
using namespace deskgui::event;

struct MessageDeserialization {
  void process(const WebviewOnMessage& event) { std::cout << event.message << std::endl; }
};

int main() {
  App app("EmbeddedResourcesExample");
  auto window = app.createWindow("window");

  window->setTitle("Embedded web resources example");
  window->setResizable(true);
  window->setMinSize({500, 500});
  window->setSize({800, 800});
  window->center();
  window->setBackgroundColor(249, 203, 103);

  WebviewOptions options;
  options.setOption(WebviewOptions::kRemoteDebuggingPort, 9222);

   Native native;
   native.createMenu();
   native.createTray();

  auto webview = window->createWebview("webview", options);

  webview->loadResources(getCompiledResources("assets"));
  webview->serveResource("index.html");

  // webview->serveResource("src/lenna.png"); //try loading a png!

  window->connect<WindowResize>(
      [&webview](const WindowResize& event) { webview->resize(event.size); });

    // This C++ callback is exposed as a global JavaScript function, `window.counter_value()`.
    // When `window.counter_value()` is called in JavaScript, this callback will be triggered.
    webview->addCallback("counter_value", [](std::string_view message) {
      std::cout << "Counter value message " << message << std::endl;
    });

    webview->addCallback("counter_reset", [=](std::string_view message) {
      std::cout << "Counter reset " << message << std::endl;
      // notify frontend that reset is processed...
      webview->postMessage("Counter reset received on the C++ side! <3");
    });

    // We can listen to all types of messages (including attached callbacks) by connecting to the
    // WebviewOnMessage event.
    MessageDeserialization msgCallback;
    webview->connect<WebviewOnMessage>(&msgCallback, &MessageDeserialization::process);

  webview->connect<WebviewContentLoaded>([&window]() { window->show(); });

  app.run();
}
