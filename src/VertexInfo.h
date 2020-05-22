#ifndef COORDINATES_H
#define COORDINATES_H


class VertexInfo {
public:
    VertexInfo(unsigned int id);
    VertexInfo(unsigned int id, float latitude, float longitude);

    unsigned int getId() const;
    float getLatitude() const;
    float getLongitude() const;

    bool operator==(const VertexInfo& otherInfo) const;
    bool operator==(unsigned int otherId) const;
private:
    unsigned int id;
    float latitude, longitude;
};


#endif // COORDINATES_H
