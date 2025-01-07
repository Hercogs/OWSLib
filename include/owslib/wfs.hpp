#ifndef OWSLIB_WFS_HPP
#define OWSLIB_WFS_HPP

#include <memory>

// Forward declarations
class WFSBase;

/**
 * @brief Creates web feature service object
 * 
 * @param url
 * @param version
 * @param timeout
 * 
 * @return A unique pointer to the initialized WFSBase object
 */
std::unique_ptr<WFSBase> web_feature_service(
    const std::string& url,
    const std::string version = "1.1.0",
    const int timeout = 30
);

/**
 * @brief Determine if version is in version list.
 * Helper function
 */
bool version_in_list(const std::string& version,
    const char* version_list[], size_t list_size);


#endif