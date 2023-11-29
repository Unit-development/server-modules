//
// Created by Kirill Zhukov on 29.11.2023.
//
#include "BasicReactModule.h"

#include <utility>

BasicReactModule::BasicReactModule(std::string index_path,
                                          std::string index_file_name) : index_path(std::move(index_path)),
                                                                               index_file_name(std::move(index_file_name)) {
    this->native_handler.addHandler(unit::server::request::GET, std::regex(R"(^\/.*\.[a-zA-Z0-9]+$)"),
                                    [&](unit::server::data::HttpRequest&request,
                                        unit::server::data::HttpResponse&response) {
                                        long size = getFileSize(this->index_path);
                                        std::string extension = "application/";
                                        extension.append(getFileExtension(this->index_path) == "js"
                                                             ? "javascript"
                                                             : "css");
                                        char buffer[20];
                                        snprintf(buffer, sizeof(buffer), "%ld", size);
                                        char* myLongAsString = buffer;
                                        response.writeFile(this->index_path);
                                        response.addHeader((char *)":status", (char *)"200");
                                        response.addHeader((char *)"Content-Type", (char *)extension.c_str());
                                        response.addHeader((char *)"Content-Length", myLongAsString);
                                        this->index_path.clear();
                                    });
    this->native_handler.addHandler(unit::server::request::GET, std::regex(R"(/)"),
                                    [&](unit::server::data::HttpRequest&request,
                                        unit::server::data::HttpResponse&response) {
                                        response.writeFile(this->index_path + "/" + this->index_file_name);
                                        response.addHeader((char *)":status", (char *)"200");
                                        response.addHeader((char *)"Content-Type", (char *)"text/html; charset=utf-8");
                                    }
    );
}

void BasicReactModule::handle(unit::server::request::type request_type, const std::string&endpoint,
                              const std::function<void (unit::server::data::HttpRequest&,
                                                        unit::server::data::HttpResponse&)>&function) {
    this->native_handler.addHandler(request_type, std::regex(endpoint), function);
};

std::shared_ptr<unit::server::regex::EndpointHandler<>> BasicReactModule::getNativeHandler() {
    return std::make_shared<unit::server::regex::basic::BasicEndpointHandler>(this->native_handler);
};
