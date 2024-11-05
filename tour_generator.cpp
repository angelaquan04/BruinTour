//
//  tour_generator.cpp
//  32project4
//
//  Created by Angela Quan on 3/11/24.
//

#include "tour_generator.h"
#include "base_classes.h"
#include <vector>
#include "geotools.h"

std::string TourGenerator:: angle(GeoPoint p1, GeoPoint p2) const
{
    double angle = angle_of_line(p1, p2);
    if (angle >= 337.5)
        return "east";
    else if (angle >= 292.5)
        return "southeast";
    else if (angle >= 247.5)
        return "south";
    else if (angle >= 202.5)
        return "southwest";
    else if (angle >= 157.5)
        return "west";
    else if (angle >= 112.5)
        return "northwest";
    else if (angle >= 67.5)
        return "north";
    else if (angle >= 22.5)
        return "northeast";
    else if (angle >= 0)
        return "east";
    return "ERROR NO DIR";
}

std::string TourGenerator::turn(GeoPoint p1, GeoPoint p2, GeoPoint p3) const
{
    double turn = angle_of_turn(p1, p2, p3); 
    if (turn >= 180 && turn <= 359)
        return "right";
    if (turn >= 1 && turn < 180)
        return "left";
    return "none";
}

TourGenerator:: TourGenerator(const GeoDatabaseBase& geodb, const RouterBase& router): gdb(geodb), rtr(router)
{}
TourGenerator:: ~TourGenerator()
{}
std::vector<TourCommand> TourGenerator :: generate_tour(const Stops& stops) const
{
    std::vector<TourCommand> tcmd;
    std::vector<GeoPoint> points;
    GeoPoint poiGP, nextpoiGP, start, end, nStart, nEnd;
    std::string poi, talk, nextPoi, nextTalk, dir, street, nextStreet;
    for (int i = 0; i < stops.size(); i++)
    {
        stops.get_poi_data(i, poi, talk);
        TourCommand curr = TourCommand();
        if (!gdb.get_poi_location(poi, poiGP))
            return std::vector<TourCommand>();
        curr.init_commentary(poi, talk);
        tcmd.push_back(curr);
        if (i != stops.size() -1 )
        {
            stops.get_poi_data(i + 1, nextPoi, nextTalk);
            gdb.get_poi_location(poi, poiGP);
            gdb.get_poi_location(nextPoi, nextpoiGP);
            points = rtr.route(poiGP, nextpoiGP);
            for (int i = 1; i < points.size(); i++)
            {
                
                start = points[i-1];
                end = points[i];
                street = gdb.get_street_name(start, end);
                dir = angle(start, end);
                TourCommand temp = TourCommand();
                temp.init_proceed(dir, street , distance_earth_miles(start, end), start, end);
                tcmd.push_back(temp);
                
                if (i != points.size() - 1)
                {
                    nStart = end;
                    nEnd = points[i+1];
                    nextStreet = gdb.get_street_name(nStart, nEnd);
                    if (nextStreet != street)
                    {
                        temp = TourCommand();
                        temp.init_turn(turn(start, end, nEnd), nextStreet);
                        if (temp.get_direction() == "none")
                            continue;
                        tcmd.push_back(temp);
                    }
                }
            }
        }
    }
    return tcmd;
}
