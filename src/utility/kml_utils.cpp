#include <iostream>
#include <string>
#include <fstream>

#include <kml/dom.h>
#include <kml/engine.h>

#include "utility/kml_utils.hpp"

KMLUtils::KMLUtils(const std::string& kml_file)
{
    kml_valid_ = KMLUtils::parse_kml_file(kml_file);
}

std::string KMLUtils::get_error_msg() const
{
    return error_msg_;
}

bool KMLUtils::get_is_kml_valid() const
{
    return kml_valid_;
}

size_t KMLUtils::get_number_of_placemarks() const
{
    return KMLUtils::placemarks_.size();
}

bool KMLUtils::get_coordinates_of_placemark(size_t idx,
                            std::vector<owslib::Coordinate>* coordinates)
{
    if (!coordinates->empty()){
        error_msg_ = "Result vector not empty";
        return false;
    }

    if (idx >= get_number_of_placemarks())
    {
        error_msg_ = "Index out of range";
        return false;
    }

    kmldom::GeometryPtr geometry = placemarks_[idx]->get_geometry();
    if (!geometry->IsA(kmldom::Type_Polygon))
    {
        error_msg_ = "Geometry is not polygon";
        return false;
    }

    kmldom::PolygonPtr polygon = kmldom::AsPolygon(geometry);
    if (polygon->has_outerboundaryis())
    {
        kmldom::OuterBoundaryIsPtr bondary = polygon->get_outerboundaryis();
        if (bondary->has_linearring())
        {
            kmldom::LinearRingPtr ring = bondary->get_linearring();
            if (ring->has_coordinates())
            {
                kmldom::CoordinatesPtr coords = ring->get_coordinates();
                for (size_t i = 0; i < coords->get_coordinates_array_size(); i++)
                {
                    kmlbase::Vec3 vec = coords->get_coordinates_array_at(i);
                    struct owslib::Coordinate coordinate{
                        .longitude = vec.get_longitude(),
                        .latitude = vec.get_latitude(),
                        .altitude = 0
                    };
                    coordinates->push_back(coordinate);
                }
                return true;
            }
        }
    }

    error_msg_ = "Polygon decoding failed";
    return false;
}

bool KMLUtils::get_coordinates_of_placemark(size_t idx,
                std::vector<std::array<double, 3>>* coordinates)
{
    if (!coordinates->empty()){
        error_msg_ = "Result vector not empty";
        return false;
    }

    std::vector<owslib::Coordinate> result;
    bool status = get_coordinates_of_placemark(idx, &result);
    if (!status) return false;

    for (auto r : result)
    {
        std::array<double, 3> arr{r.longitude, r.latitude, r.altitude};
        coordinates->push_back(arr);
    }

    return status;
}

bool KMLUtils::parse_kml_file(const std::string& kml_file)
{
    std::string kml_buff;

    if (is_kml_file_path(kml_file))
    {
        // Check if file path exists
        std::ifstream infile(kml_file);
        if (infile.good() && infile.is_open())
        {
            kml_buff = std::string(std::istreambuf_iterator<char>(infile),
                        std::istreambuf_iterator<char>());
        } else 
        {
            error_msg_ = "Invalid KML file path: " + kml_file;
            return false;
        }
    } else {
        kml_buff = kml_file;
    }

    kmldom::ElementPtr root = kmldom::Parse(kml_buff, &error_msg_);
    if(!root)
    {
        return false;
    }

    const kmldom::KmlPtr kml = kmldom::AsKml(root);
    if(!kml)
    {
        error_msg_ = "kmldom::AsKml() failed";
        return false;
    }

    // Get Document feature
    kmldom::FeaturePtr feature = kml->get_feature();
    if (!feature)
    {
        error_msg_ = "kml->get_feature(document) failed";
        return false;
    }
    kmldom::DocumentPtr document = kmldom::AsDocument(feature);
    if(!document)
    {
        error_msg_ = "kmldom::AsDocument() failed";
        return false;
    }

    // Nested features
    for (size_t i = 0; i < document->get_feature_array_size(); i++)
    {
        kmldom::FeaturePtr nested_feature = document->get_feature_array_at(i);
        if (!nested_feature->IsA(kmldom::Type_Folder)) continue;
        
        // Folder
        kmldom::FolderPtr folder = kmldom::AsFolder(nested_feature);
        for (size_t i = 0; i < folder->get_feature_array_size(); i++)
        {
            kmldom::FeaturePtr child = folder->get_feature_array_at(i);
            if (!child->IsA(kmldom::Type_Placemark)) continue;

            // Placemark
            kmldom::PlacemarkPtr placemark = kmldom::AsPlacemark(child);
            placemarks_.push_back(placemark);
        }
    }

    return true;
}

bool KMLUtils::is_kml_file_path(const std::string& file_path,
                                const std::string suffix) const
{
    return file_path.size() >= suffix.size() && \
        file_path.compare(
            file_path.size() - suffix.size(), suffix.size(), suffix
        ) == 0;
}

