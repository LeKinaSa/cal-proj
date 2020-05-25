#include "PosInfo.h"

PosInfo::PosInfo(unsigned int id) : id(id) {}

PosInfo::PosInfo(unsigned int id, float latitude, float longitude) :
        id(id), x(latitude), y(longitude) {}

unsigned int PosInfo::getId() const {
    return id;
}

float PosInfo::getX() const {
    return x;
}

float PosInfo::getY() const {
    return y;
}

bool PosInfo::operator==(const PosInfo& otherInfo) const {
    return id == otherInfo.id;
}

bool PosInfo::operator==(unsigned int otherId) const {
    return id == otherId;
}

std::ostream& operator<<(std::ostream& out, const PosInfo& v) {
    out << "(" << v.x << ", " << v.y << ")";
    return out;
}