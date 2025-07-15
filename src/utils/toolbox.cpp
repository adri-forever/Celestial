#include "utils/toolbox.h"

glm::vec3 parseHexColor(std::string colorstr) {
    //parse string #XXXXXX to hex color (if alpha is provided, will be ignored)
    //will break if you pass an invalid hex string
    glm::vec3 color;

    if ((colorstr.length()>=7) && colorstr[0]=='#') {
        /* Without the #, the string must be of length 6 at least (from the 4th group on everything will be ignored) */
        for (int i=0; i < 3; i++) {
            color[i] = std::stoi(colorstr.substr(2*i+1, 2), 0, 16)/255.;
        }
    }

    return color;
}

glm::vec3 parseHexColor_json(json colordata) {
    glm::vec3 color = glm::vec3(0.f);
    if (colordata.is_string()) {
        color = parseHexColor((std::string)colordata);
    } else if (colordata.is_array() && colordata.size()>=3) {
        for (int i=0; i<3; i++) {
            if (colordata[i].is_number()) { //later smartly detect if we have range 0-255 or 0. 1. range 
                color[i] = (float)colordata[i]/255.f;
            }
        }
    }
    return color;
}


bool checkVector(json data, int size) {
    return data.is_array() && (data.size()>=size || size==-1) && data[0].is_number();
}

bool checkVector(json data) {
    return checkVector(data, -1);
}

glm::dvec3 parseDvec3(json data) {
    glm::dvec3 vec = glm::dvec3(0.);
    if (data.is_array() && data.size()>=3) {
        for (int i=0; i<3; i++) {
            if (data[i].is_number()) {
                vec[i] = (double)data[i];
            }
        }
    }
    return vec;
}