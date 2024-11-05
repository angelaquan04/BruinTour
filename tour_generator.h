//
//  tour_generator.h
//  32project4
//
//  Created by Angela Quan on 3/11/24.
//

#include "base_classes.h"
#include "geotools.h"
#include <vector>
#ifndef tour_generator_h
#define tour_generator_h
class TourGenerator: public TourGeneratorBase
{
public:
TourGenerator(const GeoDatabaseBase& geodb, const RouterBase& router);
virtual ~TourGenerator();
virtual std::vector<TourCommand> generate_tour(const Stops& stops) const;
    
private:
    const GeoDatabaseBase& gdb;
    const RouterBase& rtr; 
    std::string angle(GeoPoint p1, GeoPoint p2) const;
    std::string turn(GeoPoint p1, GeoPoint p2, GeoPoint p3) const;

};
#endif /* tour_generator_h */
