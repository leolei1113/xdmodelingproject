#pragma once

#include <string>
#include <vector>
#include <list>
#include "position.hxx"

class ENTITY;
class FACE;
class EDGE;
class VERTEX;
class ENTITY_LIST;
class xdModelingUtilsData;

class xdModelingUtils
{
public:
	xdModelingUtils();
	~xdModelingUtils();

	static std::string getPid(ENTITY* entity);

	static std::string getStringAttribute(ENTITY* entity, const std::string& attrName);

	static FACE* getFace(ENTITY* owner, const std::string& faceName);

	static EDGE* getEdge(ENTITY* owner, const std::string& edgeName);

	static VERTEX* getVertex(ENTITY* owner, const std::string& vertexName);

	static void* GetEntity(ENTITY* owner, std::vector<std::string>& pidlist);

	int  CreatePML(ENTITY_LIST const& facelist, ENTITY_LIST const&  edgelist,
		ENTITY_LIST const&  vertexlist, int layerCount,
		bool joiningEdges, bool joiningVertices, ENTITY_LIST * bodylist);

	bool static getCenterAndNormal(FACE* face, SPAposition& center, SPAunit_vector& normal);

	bool static isRectangular(FACE* face);

	bool static isCoaxial(FACE* face);


private:
	xdModelingUtils(const xdModelingUtils&) {};
	xdModelingUtils& operator=(const xdModelingUtils&) {};

private:
	xdModelingUtilsData* m_data;

};

