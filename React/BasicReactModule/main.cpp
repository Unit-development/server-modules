//
// Created by Kirill Zhukov on 29.11.2023.
//

#include "Server.h"
#include "BasicReactModule.h"

int main() {
    unit::server::handler::Server server("../config/config.toml");
    BasicReactModule basic_react_module("../static", "sample1.html");
    server.addModule(basic_react_module);
    server.start();
}