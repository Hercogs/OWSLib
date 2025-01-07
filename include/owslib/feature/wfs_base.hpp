#ifndef OWSLIB_WFS_BASE_HPP
#define OWSLIB_WFS_BASE_HPP

#include <stdio.h>
#include <string>

class WFSBase
{
    public:
        /**
         * @brief Constructor of WFS object. 
         * @param url web feature service url
         * @param version web feature service version
         * @param timeout web feature service timeout for request
         */
        WFSBase(const std::string& url, const std::string& version, int timeout);

        /**
         * @brief function to get requested feature
         * @param layer_name web feature service layer name
         * @param filter OGC filter
         * @param format output format [KML, ...]
         * @param output storage for result
         * 
         * @return booleans wether request was succesful
         */
        bool get_feature(const std::string& layer_name, const std::string& filter,
                    const std::string& format, std::string* output) const;
    
    protected:

    private:
        std::string clean_url_;
        const std::string url_;
        const int timeout_;
        const std::string version_;

        /**
         * @brief Helper function to prepare WFS url for request
         * @param url url to clean
         */
        void clean_ows_url(std::string url);

        /**
         * @brief Helper function for curl request to store result
         * @param content data received from server
         * @param size size of one data element
         * @param nmemb number of elements, each of size "size"
         * @param userdata pointer to user defined data
         * @return number of byted received
         */
        static size_t write_callback(char* content, size_t size, size_t nmemb, void* userdata);

};



#endif
