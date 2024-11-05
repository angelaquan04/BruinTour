//
//  geodb.h
//  32project4
//
//  Created by Angela Quan on 3/8/24.
//
#include "HashMap.h"
#include "base_classes.h"
//#include <vector>
#ifndef geodb_h
#define geodb_h

//check how to do this
class GeoPoint;

class GeoDatabase: public GeoDatabaseBase
{
public:
GeoDatabase();
virtual ~GeoDatabase();
virtual bool load(const std::string& map_data_file);
virtual bool get_poi_location(const std::string& poi,
GeoPoint& point) const;
virtual std::vector<GeoPoint> get_connected_points(const GeoPoint& pt)
const;
virtual std::string get_street_name(const GeoPoint& pt1,
const GeoPoint& pt2) const;
    
private:
    HashMap<std::string>* streetNames;
    HashMap<GeoPoint>* poiMap;
    HashMap<std::vector<GeoPoint>>* connectionMap;
    
};


#endif /* geodb_h */
