#include "VertexInfo.h"


VertexInfo::VertexInfo(unsigned int id) : id(id) {}

VertexInfo::VertexInfo(unsigned int id, float latitude, float longitude) :
        id(id), latitude(latitude), longitude(longitude) {}

unsigned int VertexInfo::getId() const {
    return id;
}

float VertexInfo::getLatitude() const {
    return latitude;
}

float VertexInfo::getLongitude() const {
    return longitude;
}

bool VertexInfo::operator==(const VertexInfo& otherInfo) const {
    return id == otherInfo.id;
}

bool VertexInfo::operator==(unsigned int otherId) const {
    return id == otherId;
}

std::ostream & operator<<(std::ostream & out, const VertexInfo & v) {
    out << "(" << v.getLatitude() << ", " << v.getLongitude() << ")";
    return out;
}