#ifndef __DOJSON_HPP__
#define __DOJSON_HPP__

#include "json.h"

string map2jsonstr(const map<string,string>& map_info)
{
    Json::Value jObject;
    for (map<string, string>::const_iterator iter = map_info.begin( ); iter != map_info.end( ); ++iter)
    {
        jObject[iter->first] = iter->second;
    }
    return jObject.toStyledString();
}

string itoa_self(int i)
{
    stringstream ss;
    ss << i;
    return ss.str();
}
 
map<string,string> jsonstr2map(const string& json)
{
    Json::Reader reader;
    Json::Value value;
    map<string, string> maps;
 
    if (json.length() > 0)
    {
        if (reader.parse(json, value))
        {
            Json::Value::Members members = value.getMemberNames();
            for (Json::Value::Members::iterator it = members.begin(); it != members.end(); it++)
            { 
                Json::ValueType vt = value[*it].type();
                switch (vt)
                {
                case Json::stringValue:
                    {
                        maps.insert(pair<string, string>(*it, value[*it].asString()));
                        break;
                    }
                case Json::intValue:
                    {
                        int intTmp = value[*it].asInt();
                        maps.insert(pair<string, string>(*it, itoa_self(intTmp)));
                        break;
                    }
                case Json::arrayValue:
                    {
                        std::string strid;
                        for (unsigned int i = 0; i < value[*it].size(); i++)
                        {
                            strid +=value[*it][i].asString();
                            strid +=",";
                        }
                        if(!strid.empty())
                        {
                            strid = strid.substr(0,strid.size()-1);
                        }
                        maps.insert(pair<string, string>(*it, strid));
                        break;
                    }
                default:
                    {
                        break;
                    }
                }//end switch
            }//end for
        }//end if
    }
 
    return maps;
}

#endif