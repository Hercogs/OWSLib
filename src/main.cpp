#ifdef OWSLIB_STANDALONE

#include <iostream>
#include <stdio.h>
#include <memory>
#include <fstream>
#include <array>

#include "owslib/wfs.hpp"
#include "owslib/feature/wfs_base.hpp"
#include "ogc_filters/filters.hpp"

#include "utility/kml_utils.hpp"

int main(int argc, char** argv)
{
    const std::string url = "https://lvmgeoserver.lvm.lv/geoserver/publicwfs/ows?service=wfs&"
            "version=2.0.0&request=GetCapabilities";

    std::unique_ptr<WFSBase> wfs = web_feature_service(url, "1.1.0", 10);

    const std::string layer_name = "publicwfs:vmdpubliccompartments";
    const std::string filter = lvmgeo_filter("74940010044", "9", "4");
    const std::string format = "KML";

    std::string output;
    bool status;

    status = wfs->get_feature(layer_name, filter, format, &output);

    if(status)
    {
        //std::cout << output << std::endl;
        printf("Saving KML file:\n");
        std::ofstream my_file("./my_file.kml");
        // if(my_file) TODO
        my_file << output;
        my_file.close();

    } else
    {
        printf("Request failed!\n");
    }

    KMLUtils kml("my_file.kml");

    printf("Error msg: %s\n", kml.get_error_msg().c_str());
    printf("Error cnt: %ld\n", kml.get_number_of_placemarks());

    std::vector<std::array<double, 3>> res;
    status = kml.get_coordinates_of_placemark(0, &res);
    printf("Res cnt: %ld\n", res.size());

    return 0;
}

#endif
