#include <iostream>
#include <fstream>
#include <string>

#include <kml/dom.h>
#include <kml/engine.h>


// pkg-config --cflags --libs libkml
// g++ parse_kml.cpp `pkg-config --cflags --libs libkml`

int main(int argc, char** argv)
{
    // Read in KML file
    const std::string filename = "./my_file.kml";
    std::string kml_buff, kml_error;

    std::ifstream file("./my_file.kml");
    if (!file.is_open()) return -1;

    kml_buff = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    kmldom::ElementPtr root = kmldom::Parse(kml_buff, &kml_error);
    if(!root)
    {
        printf("Failed: %s\n", kml_error.c_str());
        return -1;
    }

    const kmldom::KmlPtr kml = kmldom::AsKml(root);
    if(!kml)
    {
        printf("Failed: %s\n", "kml");
        return -1;
    }

    // Get Document feature
    kmldom::FeaturePtr feature = kml->get_feature();
    if (!feature)
    {
        printf("Failed: %s\n", "kml->get_feature()");
        return -1;
    }
    kmldom::DocumentPtr document = kmldom::AsDocument(feature);
    if(!document)
    {
        printf("Failed: %s\n", "document");
        return -1;
    }

    printf("Type: %d, name: %s\n", feature->Type(), feature->get_name().c_str());
    printf("Type: %d, name: %s, cnt: %ld\n", document->Type(), document->get_name().c_str(), document->get_feature_array_size());
    printf("Doc: %d\n", document->Type() == kmldom::Type_Document);
    //if (feature->IsA())

    //get_schema_array_size
    for (size_t i = 0; i < document->get_schema_array_size(); i++)
    {
        kmldom::SchemaPtr nested_feature = document->get_schema_array_at(i);
        printf("Schema name: %s\n", nested_feature->get_name().c_str());
    }

    // Nested features
    for (size_t i = 0; i < document->get_feature_array_size(); i++)
    {
        kmldom::FeaturePtr nested_feature = document->get_feature_array_at(i);

        if (nested_feature->IsA(kmldom::Type_Folder))
        {
            kmldom::FolderPtr folder = kmldom::AsFolder(nested_feature);
            printf("Folder name: %s\n", folder->get_name().c_str());

            for (size_t i = 0; i < folder->get_feature_array_size(); i++)
            {
                // This is placemark
                kmldom::FeaturePtr child = folder->get_feature_array_at(i);
                printf("Folder name: %s, \n", child->get_name().c_str());

                if (child->IsA(kmldom::Type_Placemark))
                {
                    kmldom::PlacemarkPtr placemark = kmldom::AsPlacemark(child);
                    kmldom::ExtendedDataPtr data = placemark->get_extendeddata();
                    kmldom::GeometryPtr geometry = placemark->get_geometry();

                    if (geometry->IsA(kmldom::Type_Polygon))
                    {
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
                                        printf("Coord %ld: %.2f %.2f\n", i, vec.get_longitude(), vec.get_latitude());
                                    }
                                } else 
                                {
                                    printf("Not LinearRingPtr!\n");
                                }
                            } else 
                            {
                                printf("Not LinearRingPtr!\n");
                            }

                        } else 
                        {
                            printf("Not OuterBoundaryIsPtr!\n");
                        }

                    }else 
                    {
                        printf("Not Type_Polygon!\n");
                    }


                } else 
                {
                    printf("Not placemark!\n");
                }

            }

        } else
        {
            printf("Other name\n");
        }
        
    }


    file.close();







    /*

    std::ifstream file("./my_file.kml");

    std::string buff;
    if(file.is_open())
    {
        buff = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        //printf("%.1000s\n", buff.c_str());
    }

    

 
    */


    return 0;
}