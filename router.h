//
//  router.h
//  32project4
//
//  Created by Angela Quan on 3/8/24.
//

#include "HashMap.h"
#include "base_classes.h"
#include "geotools.h"
#ifndef router_h
#define router_h

class Router: public RouterBase
{
public:
Router(const GeoDatabaseBase& geo_db);
virtual ~Router();
virtual std::vector<GeoPoint> route(const GeoPoint& pt1,
const GeoPoint& pt2) const;

private:
    const GeoDatabaseBase& geodb;
    struct Pair
    {
        Pair(GeoPoint point_, double dist) : point(point_), distance(dist) {}
        Pair(GeoPoint point_, double dist, GeoPoint pp) : point(point_), distance(dist), prevPoint(pp) {}
        GeoPoint point;
        double distance;
        GeoPoint prevPoint;
 
    };

    class myComparator
    {
    public:
        int operator()(const Pair& p1, const Pair& p2)
        { return p1.distance > p2.distance; }
    };
};

#endif /* router_h */
