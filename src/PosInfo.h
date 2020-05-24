#ifndef POS_INFO_H
#define POS_INFO_H

#include <iostream>

class PosInfo {
public:
    PosInfo(unsigned int id);
    PosInfo(unsigned int id, float latitude, float longitude);

    unsigned int getId() const;
    float getX() const;
    float getY() const;

    bool operator==(const PosInfo& otherInfo) const;
    bool operator==(unsigned int otherId) const;
    friend std::ostream & operator<<(std::ostream & out, const PosInfo & v);
private:
    unsigned int id;
    float x, y;
};


#endif // POS_INFO_H
