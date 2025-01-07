#include <stdio.h>
#include <string>
#include <array>
#include <curl/curl.h>
#include <map>
#include <unordered_map>

#include "owslib/feature/wfs_base.hpp"


WFSBase::WFSBase(const std::string& url, const std::string& version, int timeout) :
    url_(url), timeout_(timeout), version_(version)
{
    clean_ows_url(url);

};

bool WFSBase::get_feature(const std::string& layer_name, const std::string& filter,
                    const std::string& format, std::string* output) const
{
    std::map<std::string, std::string> m =
    {
        {"service", "WFS"},
        {"version", "1.1.0"},
        {"request", "GetFeature"},
        {"typename", layer_name},
        {"filter", filter},
        {"outputFormat", format}
    };
    bool status{false};

    std::string request_url(clean_url_);

    for (auto it : m)
    {
        request_url.append(it.first + "=" + it.second + "&");
    }
    while(!request_url.empty() && request_url.back() == '&') {request_url.pop_back();}

    // CURL request
    CURL* curl = curl_easy_init();

    if (curl)
    {
        
        CURLcode res;
        std::string result;

        curl_easy_setopt(curl, CURLOPT_URL, request_url.c_str());
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout_);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &WFSBase::write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            status = false;
        } else 
        {
            status = true;
            *output = result;
        }
        curl_easy_cleanup(curl);
    } else 
    {
        status = false;
    }

    return status;
}


size_t WFSBase::write_callback(char* content, size_t size, size_t nmemb, void* userdata)
{
    ((std::string*)userdata)->append((char*)content, nmemb);
    return size * nmemb;
}

void WFSBase::clean_ows_url(std::string url)
{
    /**
     * Convert from: https://lvmgeoserver.lvm.lv/geoserver/publicwfs/
     *               ows?service=wfs&version=2.0.0&request=GetCapabilities
     * to: https://lvmgeoserver.lvm.lv/geoserver/publicwfs/ows?
     */

    if (url.empty()) return;

    std::array<std::string, 3> basic_elements {"service", "version", "request"};

    for (std::string element : basic_elements)
    {
        if (url.empty()) return;

        size_t start = url.find(element);
        if (start != std::string::npos)
        {
            size_t end = url.find('&', start);
            end == std::string::npos ? url.erase(start) : url.erase(start, start - end + 1);
        }
    }

    while(!url.empty() && url.back() == '&') {url.pop_back();}
    
    std::string target = "/ows?";
    size_t pos = url.find(target);
    if (pos != std::string::npos)
    {
        url.replace(pos, target.size(), "/wfs?");
    }
    
    clean_url_ = url;
};

