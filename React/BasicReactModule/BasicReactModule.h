//
// Created by Kirill Zhukov on 29.11.2023.
//

#ifndef BASICREACTMODULE_H
#define BASICREACTMODULE_H

#include "HttpResponse.h"
#include "types.h"
#include "EndpointHandler.h"
#include "HandlerModule.h"
#include "BasicEndpointHandler.h"

static std::string getFileExtension(const std::string& fileName) {
    size_t dotPos = fileName.find_last_of('.');
    if (dotPos != std::string::npos) {
        return fileName.substr(dotPos + 1);
    }
    return ""; // Return empty string if no extension found
}

static long getFileSize(const std::string& fileName) {
    std::ifstream file(fileName, std::ifstream::ate | std::ifstream::binary);
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << fileName << std::endl;
        return -1; // Return -1 or throw an exception if file can't be opened
    }
    return file.tellg();
}

class BasicReactModule : public unit::server::module::HandlerModule {
public:
    BasicReactModule(std::string index_path, std::string index_file_name);
    ~BasicReactModule() override = default;
public:
    void handle(unit::server::request::type request_type, const std::string&endpoint,
                        const std::function<void (unit::server::data::HttpRequest&, unit::server::data::HttpResponse&)>& function) override;

    std::shared_ptr<unit::server::regex::EndpointHandler<>> getNativeHandler() override;
private:
    unit::server::regex::basic::BasicEndpointHandler native_handler;
    std::string index_path;
    std::string index_file_name;
};

#endif // BASICREACTMODULE_H