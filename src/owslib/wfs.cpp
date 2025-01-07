#include <iostream>
#include <exception>

#include "owslib/wfs.hpp"
#include "owslib/feature/wfs_base.hpp"


std::unique_ptr<WFSBase> web_feature_service(
    const std::string& url,
    const std::string version,
    const int timeout
)
{
    // Check WFS versions
    const char* v10[] = {"1.0", "1.0.0"};
    const char* v11[] = {"1.1", "1.1.0"};   // Only this version supported by now
    const char* v20[] = {"2.0", "2.0.0"};
    std::string final_version;

    if (version_in_list(version, v10, sizeof(v10) / sizeof(v10[0])))
    {
        final_version = "1.0.0";
        throw std::invalid_argument("Version " + final_version + " not supported yet");
    } else if (version_in_list(version, v11, sizeof(v11) / sizeof(v11[0])))
    {
        final_version = "1.1.0";
    } else if (version_in_list(version, v20, sizeof(v20) / sizeof(v20[0])))
    {
        final_version = "2.0.0";
        throw std::invalid_argument("Version " + final_version + " not supported yet");
    } else
    {
        throw std::invalid_argument("Version " + version + " not valid");
    }

    std::unique_ptr<WFSBase> wfs = std::make_unique<WFSBase>(url, version, timeout);
    return std::move(wfs);
}

bool version_in_list(const std::string& version,
    const char* version_list[], size_t list_size)
{
    for (size_t i = 0; i < list_size; i++)
    {
        if (version == version_list[i]) return true;
    }
    return false;
}

