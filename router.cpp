//
//  router.cpp
//  32project4
//
//  Created by Angela Quan on 3/8/24.
//

#include "router.h"
#include "geopoint.h"
#include "geotools.h"
#include "geodb.h"
#include "base_classes.h"
#include "HashMap.h"
#include <stdio.h>
#include <queue>
#include <map>
using namespace std;

Router::Router(const GeoDatabaseBase& geo_db) : geodb(geo_db)
{}

Router:: ~Router()
{}

vector<GeoPoint> Router::route(const GeoPoint& pt1, const GeoPoint& pt2) const
{
    std::vector<GeoPoint> returnMap;
    
    //if target point and endpoint are the same point
    if (pt1.to_string() == pt2.to_string())
    {
        returnMap.push_back(pt1);
        return returnMap;
    }

    //minheap to keep track of points
    priority_queue<Pair, vector<Pair>, myComparator> pq =  priority_queue<Pair, vector<Pair>, myComparator>();
    //hashmap to keep track of points already visited and their previous GeoPoint
    HashMap<GeoPoint> previousPointMap =  HashMap<GeoPoint>();

    //push the first point onto the queue
    pq.push(Pair(pt1, 0));
    GeoPoint prevP = pt1;
    Pair currP = pq.top();

    
    vector<GeoPoint> cMap = geodb.get_connected_points(pt1);
    while (!pq.empty())
    {
        currP = pq.top();
        pq.pop();

        //found endpoint
        if (currP.point.to_string() == pt2.to_string())
            break;
        
        //already visited this data point before
        if (previousPointMap.find(currP.point.to_string()) != nullptr)
            { continue; }
        previousPointMap[currP.point.to_string()] = currP.prevPoint;
      
        
        //load in the points connected to current point
        cMap = geodb.get_connected_points(currP.point);
        for (int i = 0; i < cMap.size(); i++)
        {
           //if target point is connected to current point, push it to the front of the queue
            if (cMap[i].to_string() == pt2.to_string())
            { 
                pq.push(Pair(cMap[i], 0, currP.point));
                previousPointMap[cMap[i].to_string()] = currP.point;
                break;
            }
            
            //determine the heuristic cost by finding distance to prevP and distance to endpoint
            double dist =  distance_earth_miles(cMap[i], prevP) + distance_earth_miles(cMap[i], pt2);
            pq.push(Pair(cMap[i], dist, currP.point));
        }
        prevP = currP.point;
    }
        
    //endpoint found
    if (currP.point.to_string() == pt2.to_string())
    {
        GeoPoint current = currP.point;
        returnMap.push_back(current);
        while (current.to_string() != pt1.to_string())
        {
            current = *previousPointMap.find(current.to_string());
            returnMap.insert(returnMap.begin(), current);
        }
    }
    
    return returnMap;
}


