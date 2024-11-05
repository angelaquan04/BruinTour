//
//  geodb.cpp
//  32project4
//
//  Created by Angela Quan on 3/8/24.
//

#include "HashMap.h"
#include "geodb.h"
#include "geotools.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

GeoDatabase :: GeoDatabase()
{
    streetNames = new HashMap<string>();
    poiMap = new HashMap<GeoPoint>();
    connectionMap = new HashMap<vector<GeoPoint>>();
}
GeoDatabase:: ~GeoDatabase()
{
    delete streetNames;
    delete poiMap;
    delete connectionMap;
    
}
bool GeoDatabase:: load(const std::string& map_data_file)
{
    ifstream infile(map_data_file);
      // Test for failure to open
    if ( ! infile)
    {
        cerr << "Cannot open file!" << endl;
        return false;
    }
     
    string line;
    while (getline(infile, line))
    {
       
        istringstream iss(line);
        GeoPoint start, end, mid;
        string street, cords, lat1, lon1, lat2, lon2;
        int pois;
        
        
        if (!getline(iss, street))
            continue;
       // cerr << "start of endpoints: " << street << endl;
        
        infile >> lat1 >> lon1 >> lat2 >> lon2;
       // cerr << "lat1: " << lat1 << ", lon1: " << lon1 << ", lat 2" << lat2 << ", lon2: " << lon2 << endl;
        
        infile >> pois;
       // cerr << "pois: " << pois << endl;
        
        start = GeoPoint(lat1, lon1);
        end = GeoPoint(lat2, lon2);
        
        //add points to connection map
        (*connectionMap)[start.to_string()].push_back(end);
        (*connectionMap)[end.to_string()].push_back(start);
        
        if (pois == 0)
        {
            //add street segment to street map
            (*streetNames)[start.to_string() + ";" + end.to_string()] = street;
        }
        else //poi on the street
        {
           
            mid = midpoint(start, end);
            
            //add street segments to street map
            (*streetNames)[start.to_string() + ";" + mid.to_string()] = street;
            (*streetNames)[mid.to_string() + ";" + end.to_string()] = street;
            (*streetNames)[start.to_string() + ";" + end.to_string()] = street;

            //add midpoint as connected point to start and end
            (*connectionMap)[start.to_string()].push_back(mid);
            (*connectionMap)[end.to_string()].push_back(mid);
            (*connectionMap)[mid.to_string()].push_back(start);
            (*connectionMap)[mid.to_string()].push_back(end);
            
            
            getline(infile, line);
            string poiName, poiLat, poiLon;
            for (int i = 0; i < pois; i++)
            {
                getline(infile, line);
                poiName = line.substr(0, line.find('|'));
                istringstream iss(line.substr(line.find('|') + 1));
                iss >> poiLat >> poiLon;
                GeoPoint interestP = GeoPoint(poiLat, poiLon);
                (*poiMap)[poiName] = interestP;
                
                //add street/path from midpoint to poi
                (*streetNames)[mid.to_string() + ";" + interestP.to_string()] = "a path";
                
                //connect poi to midpoint and vice versa
                (*connectionMap)[mid.to_string()].push_back(interestP);
                (*connectionMap)[interestP.to_string()].push_back(mid);
            }
        }
    }
    return true;
    
}

bool GeoDatabase:: get_poi_location(const std::string& poi, GeoPoint& point) const
{
    if (poiMap->find(poi) != nullptr)
    {
        point = (*poiMap)[poi];
        return true;
    }
    return false;
}

std::vector<GeoPoint> GeoDatabase::get_connected_points(const GeoPoint& pt) const
{
    std::vector<GeoPoint> connected = std::vector<GeoPoint>();
    for (int i = 0; i < (*connectionMap)[pt.to_string()].size(); i++)
        connected.push_back((*connectionMap)[pt.to_string()][i]);
    return connected;
}

std::string GeoDatabase::get_street_name(const GeoPoint& pt1, const GeoPoint& pt2) const
{
    string key1, key2;
    key1 = pt1.to_string() + ";" + pt2.to_string();
    
    if (streetNames->find(key1) == nullptr)
    {
        key2 = pt2.to_string() + ";" + pt1.to_string();
        if (streetNames->find(key2) != nullptr)
            return (*streetNames)[key2];
    }
    else if (streetNames->find(key1) != nullptr)
        return (*streetNames)[key1];
    
    cerr << "ERROR COORDINATES: " << key1 << endl;
    return "ERROR NO STREET FOUND";
}
