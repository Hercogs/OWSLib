#include <cstring>
#include <unordered_map>

#include "ogc_filters/filters.hpp"

std::string lvmgeo_filter(const char* kadastrs, const char* kvartals, const char* nogabals)
{
    std::string filter_template =
        "<ogc:And xmlns:ogc=\"http://www.opengis.net/ogc\">"
            "<ogc:PropertyIsEqualTo xmlns:ogc=\"http://www.opengis.net/ogc\">"
                "<ogc:PropertyName>kadastrs</ogc:PropertyName>"
                "<ogc:Literal>{kadastrs}</ogc:Literal>"
            "</ogc:PropertyIsEqualTo>"
            "<ogc:PropertyIsEqualTo xmlns:ogc=\"http://www.opengis.net/ogc\">"
                "<ogc:PropertyName>kvart</ogc:PropertyName>"
                "<ogc:Literal>{kvart}</ogc:Literal>"
            "</ogc:PropertyIsEqualTo>"
            "<ogc:PropertyIsEqualTo xmlns:ogc=\"http://www.opengis.net/ogc\">"
                "<ogc:PropertyName>nog</ogc:PropertyName>"
                "<ogc:Literal>{nog}</ogc:Literal>"
            "</ogc:PropertyIsEqualTo>"
        "</ogc:And>"
    ;
    
    size_t pos;

    pos = filter_template.find("{kadastrs}");
    if (pos != std::string::npos) filter_template.replace(pos, strlen("{kadastrs}"), kadastrs);
    pos = filter_template.find("{kvart}");
    if (pos != std::string::npos) filter_template.replace(pos, strlen("{kvart}"), kvartals);
    pos = filter_template.find("{nog}");
    if (pos != std::string::npos) filter_template.replace(pos, strlen("{nog}"), nogabals);

    return clean_filter(filter_template);
}

std::string clean_filter(const std::string& in_filter)
{
    const std::unordered_map<char, std::string> special_chars = {
        {' ', "+"},
        {'!', "%21"},
        {'#', "%23"},
        {'$', "%24"},
        {'&', "%26"},
        {'\'', "%27"},
        {'(', "%28"},
        {')', "%29"},
        {'*', "%2A"},
        {'+', "%2B"},
        {',', "%2C"},
        {'<', "%3C"},
        {'>', "%3E"},
        {'"', "%22"},
        {'/', "%2F"},
        {':', "%3A"},
        {';', "%3B"},
        {'=', "%3D"},
        {'?', "%3F"},
        {'@', "%40"}
    };

    std::string out_filter;

    for (char c : in_filter)
    {
        std::unordered_map<char, std::string>::const_iterator it = special_chars.find(c);
        if (it != special_chars.end())
        {
            out_filter += it->second;
        } else 
        {
            out_filter += c;
        }
    }

    return out_filter;
}



