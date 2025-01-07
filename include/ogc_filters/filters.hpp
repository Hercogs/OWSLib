#ifndef OGC_FILTERS_FILTERS_HPP
#define OGC_FILTERS_FILTERS_HPP

#include <string>

/**
 * @brief Creates filter for LVMGEO web feature service
 * 
 * @return created filter
 */
std::string lvmgeo_filter(const char* kadastrs, const char* kvartals, const char* nogabals);

/**
 * @brief This function cleans up filter by substituting speacial chars
 * 
 * @param in_filter content to filter
 * @return filtered content
 */
std::string clean_filter(const std::string& filter);


#endif