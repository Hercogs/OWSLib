#ifndef UTILITY_KML_UTILS_HPP
#define UTILITY_KML_UTILS_HPP

#include <string>
#include <vector>
#include <array>

#include <kml/dom.h>
#include <kml/engine.h>

#include "utility/common_utils.hpp"

class KMLUtils
{
    public:
        /**
         * @brief Constructor of class
         * @param kml_file Either path to the file or content of the file
         */
        KMLUtils(const std::string& kml_file); // pass either path to *kml file or content

        /* Setters */

        /* Getters */

        /**
         * @brief Get las error message
         */
        std::string get_error_msg() const;

        /**
         * @brief Check wether passed *kml file was valid
         */
        bool get_is_kml_valid() const;

        /**
         * @brief Get number of placemarks in passed *kml file
         */
        size_t get_number_of_placemarks() const;

        /**
         * @brief Get coordinates of specific placemark. It will only works
         * if placemark geometry type is Polygon. 
         * @param idx requested placemark index 
         * @param coordinates storage for result
         * @return boolean wether operation was succesfull
         */
        bool get_coordinates_of_placemark(size_t idx,
                                          std::vector<owslib::Coordinate>* coordinates);
        
        /**
         * @brief Get coordinates of specific placemark. It will only works
         * if placemark geometry type is Polygon. 
         * @param idx requested placemark index 
         * @param coordinates storage for result
         * @return boolean wether operation was succesfull
         */
        bool get_coordinates_of_placemark(size_t idx,
                                          std::vector<std::array<double, 3>>* coordinates);



        /* Variables */

    protected:

    private:
        /* Variables */

        bool kml_valid_ {false};
        std::string error_msg_;
        std::vector<kmldom::PlacemarkPtr> placemarks_;

        /* Functions */

        /**
         * @brief Parse *kml file
         * @param kml_file
         * @return boolean if parsing was succesful
         */
        bool parse_kml_file(const std::string& kml_file);

        /**
         * @brief Check wether "file_path" is path of the *kml file
         */
        bool is_kml_file_path(const std::string& file_path,
                              const std::string suffix=".kml") const;

};

#endif