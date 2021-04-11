#include <iostream>
#include <unistd.h>
#include <array>
#include <string.h>
#include <list>
#include <map>

struct PortConfig
{
	struct flash
	{
		bool configd;
		bool protect;
	};
	
	struct key_value
	{
		std::string key;
		std::string value;
		struct flash fla;
	};

private:
	std::string name;
	struct key_value KeyValue;
	std::list<key_value> key_value_list;
	std::map<std::string,std::list<key_value>> mapData;
public:

	PortConfig(std::string strName = "" , std::string strKey = "",
				std::string strValue = "",bool configTmp = 0, bool procTmp=0
				)
			:name{strName}
			,KeyValue{strKey,strValue , {configTmp, procTmp}}
			,key_value_list{}
			,mapData{}
	{
	}

	void insertData(std::string& name , struct key_value& tmp )
	{
		key_value_list.push_back(tmp);
		//mapData.insert(std::make_pair(name , key_value_list));
		mapData[name] = key_value_list;
	}
	
	std::map<std::string, std::list<key_value>>& getMapData()
	{
		return mapData;
	}
	
};


int main(int argc, char** argv )
{
	std::string str =  "asdfghjkl/qwer"; 
	//const std::string str{"asdfghjkl/qwer"};
	const int MaxPathLength = 50;
	std::array<char , MaxPathLength> arr{};
	
	int size = 0 ;
    size = sizeof(arr);
	std::cout << "arr size: " << size << std::endl;

	auto path = getcwd(arr.data(),arr.size()); 

	std::cout << "work Path :" << path <<std::endl;	

	std::cout << "work path : " << std::string{path != NULL ? path : ""} << std::endl;
	PortConfig::key_value tmp{"num1","98",{1,1}};
	PortConfig::key_value tmp1{"num2","99",{1,0}};

	struct PortConfig PortObject{};
	
	PortObject.insertData(str, tmp);
	PortObject.insertData(str,tmp1);

	auto mapData = PortObject.getMapData();
	
	std::map<std::string,std::list<PortConfig::key_value>>::iterator it ;
	for (it = mapData.begin();it != mapData.end();it++)
	{
		if(it->first == str)
		{
			std::cout <<str << std::endl;
			for(auto lst = it->second.begin(); lst != it->second.end();lst++)
			{
				std::cout << "key: " << lst->key <<" value: " << lst->value ;
				std::cout << " configd: " << lst->fla.configd << " protect: " << lst->fla.protect << std::endl;
			}
		}
	}
	
	return 0;
}
