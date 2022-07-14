#include "xdModelingUtils.h"
#include "xdModelingDataType.h"

#include "kernapi.hxx"
#include "ga_api.hxx"
#include "rnd_api.hxx"
#include "at_str.hxx"
#include "ctapi.hxx"
#include "at_pid.hxx"
#include "face.hxx"
#include "edge.hxx"
#include "boolapi.hxx"
#include "intrapi.hxx"
#include "lop_api.hxx"
#include "shl_api.hxx"
#include "queryapi.hxx"
#include "cstrapi.hxx"
#include "license.hxx"
#include "spa_unlock_result.hxx"
#include "ckoutcom.hxx"
#include "swp_opts.hxx"
#include "sweepapi.hxx"
#include "ofstapi.hxx"
#include "getowner.hxx"
#include "pidapi.hxx"
#include "faceqry.hxx"
#include "acistol.hxx"
#include "clearapi.hxx"
#include "surface.hxx"
#include "vertex.hxx"
#include "point.hxx"
#include "curve.hxx"
#include "strdef.hxx"
#include "curdef.hxx"
#include "faceutil.hxx"
#include "curveq.hxx"

namespace
{
	void split(const std::string& orgStr, std::vector<std::string>* sv, const char* separator)
	{
		std::string str = orgStr;
		str += separator;
		std::size_t size = strlen(separator);
		std::size_t n = str.find(separator);
		while (n != -1)
		{
			std::string substr = str.substr(0, n);
			sv->emplace_back(substr);
			str = str.substr(n + size);
			n = str.find(separator);
		}
	}
}

class xdModelingUtilsData
{
public:
	int GetAttrPidIndex(ENTITY* entity);
	int IsValid(ENTITY_LIST const & facelist, BODY*& owner);
	int IsCuboid(ENTITY_LIST const & facelist);
	int GetNonadjFaces(FACE* face, ENTITY_LIST const & facelist,
		ENTITY_LIST* nonadj_facelist);
	int SweepFaceAlongNormal(FACE* face, double thickness, BODY*& outBody);
	int GetRegionIndex(const char* type, SPAvector vec = SPAvector());
	int MakeFaceLayer(ENTITY_LIST const & facelist, double thickness, int layerCount,
		ENTITY_LIST * bodylist);
	int MakeEdgeLayer(ENTITY_LIST const & facelist, ENTITY_LIST const & edgelist, double thickness,
		int layerCount, ENTITY_LIST * bodylist);
	int MakeVertexLayer(ENTITY_LIST const & edgelist, ENTITY_LIST const & vertexlist, double thickness,
		int layerCount, ENTITY_LIST * bodylist);
};

int xdModelingUtilsData::GetAttrPidIndex(ENTITY* entity)
{
	const ATTRIB_PID* attrPid = get_pid_attrib(entity);
	if (!attrPid)
	{
		return -1;
	}
	return attrPid->get_index();
}

int xdModelingUtilsData::IsValid(ENTITY_LIST const & facelist, BODY*& owner)
{
	if (facelist.count() < 1)
	{
		return 1;
	}
	bool isSingleOwner = true;
	bool isOwnerSolid = true;
	int idx = -1;
	for (int i = 0; i < facelist.count(); ++i)
	{
		ENTITY* tempEntity = get_owner(facelist[i]);
		if (!tempEntity || !is_solid_body(tempEntity))
		{
			isOwnerSolid = false;
			break;
		}
		else
		{
			int tempIdx = GetAttrPidIndex(tempEntity);
			if (idx == -1)
			{
				owner = (BODY*)tempEntity;
				idx = tempIdx;
			}
			else if (idx != tempIdx)
			{
				isSingleOwner = false;
				break;
			}
		}
	}
	return (isSingleOwner && isOwnerSolid) ? 0 : 1;
}

int xdModelingUtilsData::IsCuboid(ENTITY_LIST const & facelist)
{
	bool isPlanar = true;
	bool isVorP = true;
	SPAunit_vector preNormal;
	for (int i = 0; i < facelist.count(); ++i)
	{
		if (!is_planar_face(facelist[i]))
		{
			isPlanar = false;
			break;
		}
		SPAunit_vector normal;
		get_face_normal((FACE*)facelist[i], normal);
		if (i != 0 &&
			!perpendicular(normal, preNormal) &&
			!antiparallel(normal, preNormal))
		{
			isVorP = false;
			break;
		}
		else
		{
			preNormal = normal;
		}
	}
	return (isPlanar && isVorP) ? 0 : 1;
}

int xdModelingUtilsData::GetNonadjFaces(FACE* face, ENTITY_LIST const & facelist, ENTITY_LIST* nonadj_facelist)
{
	double tol = SPAresabs.value();
	for (int i = 0; i < facelist.count(); ++i)
	{
		if (face == facelist[i])
		{
			continue;
		}
		SPAposition p1(0, 0, 0), p2(0, 0, 0);
		double min_dist = -1;
		api_check_face_clearance(face, (FACE*)facelist[i], 0, 0, p1, p2, min_dist);
		if (min_dist > tol)
		{
			nonadj_facelist->add(facelist[i]);
		}
	}
	return 0;
}

int xdModelingUtilsData::SweepFaceAlongNormal(FACE* face, double thickness, BODY*& outBody)
{
	SPAtransf tform = get_owner_transf(face);
	//FACE* new_face = NULL;
	//exit_on_bad_outcome(api_offset_face((FACE*)face, 0, new_face));
	ENTITY* new_face = NULL;
	//exit_on_bad_outcome(api_deep_copy_entity(face, new_face));
	api_copy_entity_contents(face, new_face);
	printf("%s successfully created copied face\n", new_face ? "" : "not");

	//exit_on_bad_outcome(api_transform_entity(new_face, tform));
	//printf("successfully move new face\n");

	BODY* new_body = NULL;
	sweep_options swpOptions;
	api_sweep_with_options(new_face, thickness, &swpOptions, new_body);
	printf("%s successfully created swept body\n", new_body ? "" : "not");
	if (new_body)
	{
		outBody = new_body;
	}
	else
	{
		outBody = (BODY*)new_face;
	}
	return 0;
}

int xdModelingUtilsData::GetRegionIndex(const char* type, SPAvector vec)
{
	if (strcmp(type, "FACE") == 0 &&
		(parallel(vec, SPAvector(0, 0, 1)) || antiparallel(vec, SPAvector(0, 0, 1))))
	{
		return 1;
	}
	else if (strcmp(type, "FACE") == 0 &&
		(parallel(vec, SPAvector(0, 1, 0)) || antiparallel(vec, SPAvector(0, 1, 0))))
	{
		return 2;
	}
	else if (strcmp(type, "FACE") == 0 &&
		(parallel(vec, SPAvector(1, 0, 0)) || antiparallel(vec, SPAvector(1, 0, 0))))
	{
		return 4;
	}
	else if (strcmp(type, "EDGE") == 0 &&
		(parallel(vec, SPAvector(1, 0, 0)) || antiparallel(vec, SPAvector(1, 0, 0))))
	{
		return 3;
	}
	else if (strcmp(type, "EDGE") == 0 &&
		(parallel(vec, SPAvector(0, 1, 0)) || antiparallel(vec, SPAvector(0, 1, 0))))
	{
		return 5;
	}
	else if (strcmp(type, "EDGE") == 0 &&
		(parallel(vec, SPAvector(0, 0, 1)) || antiparallel(vec, SPAvector(0, 0, 1))))
	{
		return 6;
	}
	else if (strcmp(type, "VERTEX") == 0)
	{
		return 7;
	}
	else
	{
		return -1;
	}
}

int xdModelingUtilsData::MakeFaceLayer(ENTITY_LIST const & facelist, double thickness, int layerCount, ENTITY_LIST * bodylist)
{
	std::list<int> sweptFaceIdList;
	for (int i = 0; i < facelist.count(); ++i)
	{
		int idx = GetAttrPidIndex(facelist[i]);
		if (std::find(sweptFaceIdList.cbegin(), sweptFaceIdList.cend(), idx)
			!= sweptFaceIdList.end())
		{
			continue;
		}
		ENTITY_LIST nonadj_facelist;
		GetNonadjFaces((FACE*)facelist[i], facelist, &nonadj_facelist);
		printf("find %d non-adjacent faces", nonadj_facelist.count());

		nonadj_facelist.add(facelist[i]);
		nonadj_facelist.reverse();
		for (int j = 0; j < nonadj_facelist.count(); ++j)
		{
			int idx = GetAttrPidIndex(nonadj_facelist[j]);
			if (std::find(sweptFaceIdList.cbegin(), sweptFaceIdList.cend(), idx)
				!= sweptFaceIdList.cend())
			{
				continue;
			}
			sweptFaceIdList.push_back(idx);

			for (int k = 0; k < layerCount; ++k)
			{
				BODY* sweptBody = NULL;
				SweepFaceAlongNormal((FACE*)nonadj_facelist[j], thickness*(layerCount - k) / layerCount, sweptBody);
				if (sweptBody)
				{
					SPAunit_vector normal;
					get_face_normal((FACE*)nonadj_facelist[j], normal);
					int groupIndex = GetRegionIndex("FACE", normal);
					api_add_generic_named_attribute(sweptBody, "PMLGroupIndex", groupIndex);
					bodylist->add(sweptBody);
				}
			}
		}
	}
	return 0;
}

int xdModelingUtilsData::MakeEdgeLayer(ENTITY_LIST const & facelist, ENTITY_LIST const & edgelist, double thickness, int layerCount,
	ENTITY_LIST * bodylist)
{
	std::vector<int> faceIdList;
	faceIdList.resize(facelist.count());
	for (int i = 0; i < facelist.count(); ++i)
	{
		faceIdList[i] = GetAttrPidIndex(facelist[i]);
	}

	std::list<int> sweptEdgeIdList;
	for (int i = 0; i < edgelist.count(); ++i)
	{
		ENTITY_LIST related_facelist;
		api_get_faces(edgelist[i], related_facelist);
		if (related_facelist.count() != 2)
		{
			continue;
		}
		int idx1 = GetAttrPidIndex(related_facelist[0]);
		int idx2 = GetAttrPidIndex(related_facelist[1]);
		if (std::find(faceIdList.cbegin(), faceIdList.cend(), idx1) == faceIdList.end() ||
			std::find(faceIdList.cbegin(), faceIdList.cend(), idx2) == faceIdList.end())
		{
			continue;
		}

		SPAunit_vector normal1, normal2;
		get_face_normal((FACE*)related_facelist[0], normal1);
		get_face_normal((FACE*)related_facelist[1], normal2);

		for (int j = 0; j < layerCount; ++j)
		{
			sweep_options swpOptions;
			BODY* sweptBody = NULL;
			api_sweep_with_options(edgelist[i], thickness*(layerCount - j) / layerCount * normal1,
				&swpOptions, sweptBody);
			BODY* help_body = NULL;
			api_sweep_with_options(sweptBody, thickness*(layerCount - j) / layerCount * normal2,
				&swpOptions, help_body);
			if (sweptBody)
			{
				EDGE* edge = (EDGE*)edgelist[i];
				SPAtransf tform = get_owner_transf(edge);
				SPAvector edgeVector = (edge->end_pos() - edge->start_pos()) * tform;
				int groupIndex = GetRegionIndex("EDGE", edgeVector);
				api_add_generic_named_attribute(sweptBody, "PMLGroupIndex", groupIndex);
				bodylist->add(sweptBody);
			}
		}
	}
	return 0;
}

int xdModelingUtilsData::MakeVertexLayer(ENTITY_LIST const & edgelist, ENTITY_LIST const & vertexlist, double thickness, int layerCount,
	ENTITY_LIST * bodylist)
{
	std::vector<int> edgeIdList;
	edgeIdList.resize(edgelist.count());
	for (int i = 0; i < edgelist.count(); ++i)
	{
		edgeIdList[i] = GetAttrPidIndex(edgelist[i]);
	}

	std::list<int> sweptVertexIdList;
	for (int i = 0; i < vertexlist.count(); ++i)
	{
		VERTEX* vertex = (VERTEX*)vertexlist[i];
		ENTITY_LIST related_edgelist;
		api_get_edges(vertex, related_edgelist);
		if (related_edgelist.count() != 3)
		{
			continue;
		}
		int idx1 = GetAttrPidIndex(related_edgelist[0]);
		int idx2 = GetAttrPidIndex(related_edgelist[1]);
		int idx3 = GetAttrPidIndex(related_edgelist[2]);
		if (std::find(edgeIdList.cbegin(), edgeIdList.cend(), idx1) == edgeIdList.end() ||
			std::find(edgeIdList.cbegin(), edgeIdList.cend(), idx2) == edgeIdList.end() ||
			std::find(edgeIdList.cbegin(), edgeIdList.cend(), idx3) == edgeIdList.end())
		{
			continue;
		}

		SPAtransf tform = get_owner_transf(vertex);
		SPAvector vec1, vec2, vec3;
		EDGE* edge1 = (EDGE*)related_edgelist[0];
		EDGE* edge2 = (EDGE*)related_edgelist[1];
		EDGE* edge3 = (EDGE*)related_edgelist[2];
		vec1 = (
			vertex == edge1->start() ?
			edge1->start_pos() - edge1->end_pos() : edge1->end_pos() - edge1->start_pos())*tform
			;
		vec2 = (
			vertex == edge2->start() ?
			edge2->start_pos() - edge2->end_pos() : edge2->end_pos() - edge2->start_pos())*tform
			;
		vec3 = (
			vertex == edge3->start() ?
			edge3->start_pos() - edge3->end_pos() : edge3->end_pos() - edge3->start_pos())*tform
			;

		for (int j = 0; j < layerCount; ++j)
		{
			EDGE* line1;
			SPAposition pos1 = vertex->geometry()->coords()*tform;
			SPAposition pos2(pos1.x() + vec1.x()*thickness*(layerCount - j) / layerCount / vec1.len(),
				pos1.y() + vec1.y()*thickness*(layerCount - j) / layerCount / vec1.len(),
				pos1.z() + vec1.z()*thickness*(layerCount - j) / layerCount / vec1.len());
			api_mk_ed_line(pos1, pos2, line1);

			sweep_options swpOptions;
			BODY* sweptBody = NULL;
			api_sweep_with_options(line1, thickness*(layerCount - j) / layerCount / vec2.len() *vec2,
				&swpOptions, sweptBody);
			BODY* help_body = NULL;
			api_sweep_with_options(sweptBody, thickness*(layerCount - j) / layerCount / vec3.len() *vec3,
				&swpOptions, help_body);
			if (sweptBody)
			{
				int groupIndex = GetRegionIndex("VERTEX");
				api_add_generic_named_attribute(sweptBody, "PMLGroupIndex", groupIndex);
				bodylist->add(sweptBody);
			}
		}
	}
	return 0;
}


xdModelingUtils::xdModelingUtils()
{
	m_data = new xdModelingUtilsData();
}

xdModelingUtils::~xdModelingUtils()
{
	if (m_data)
	{
		delete m_data;
	}
}

std::string xdModelingUtils::getPid(ENTITY* entity)
{
	bool suc = true;
	ENTITY* cubBody = entity;
	if (get_pid_attrib(cubBody) == nullptr)
	{
		outcome result = api_pidset(cubBody);
	}

	std::string pid;
	if (entity)
	{
		const ATTRIB_PID* PIDNewAtti = get_pid_attrib(entity);
		if (PIDNewAtti != nullptr)
		{
			int iIdx = PIDNewAtti->get_index();
			int iTime = PIDNewAtti->get_time_val();
			int iCopy = PIDNewAtti->get_copy_num();
			pid = std::to_string(iTime) + "_" + std::to_string(iIdx) + "_" + std::to_string(iCopy);
			if (pid == std::string("0_0_1"))
			{
				pid = std::string(PIDNewAtti->get_base_name());
			}
		}
	}
	return pid;
}

std::string xdModelingUtils::getStringAttribute(ENTITY * entity, const std::string & attrName)
{
	std::string attrValue;
	ATTRIB_GEN_NAME *pAttr = nullptr;
	api_find_named_attribute(static_cast<ENTITY*>(entity), attrName.c_str(), pAttr);
	if (pAttr != nullptr)
	{
		attrValue = dynamic_cast<ATTRIB_GEN_STRING*>(pAttr)->value();
	}
	return attrValue;
}

EDGE* xdModelingUtils::getEdge(ENTITY* owner, const std::string& edgeName)
{
	EDGE* reEdge = NULL;
	std::vector<std::string> faceNameList;
	split(edgeName, &faceNameList, "&");
	if (faceNameList.size() == 1)
	{
		ENTITY_LIST body_edgelist;
		api_get_edges_from_all_entities(owner, body_edgelist);
		body_edgelist.init();
		ENTITY* edge = body_edgelist.first();
		while (edge != NULL)
		{
			ATTRIB_GEN_NAME* res = NULL;
			api_find_named_attribute(edge, "ATTRIB_XACIS_NAME", res);
			if (res != NULL)
			{
				const char* name = ((ATTRIB_GEN_STRING*)res)->value();
				if (edgeName == name)
				{
					reEdge = (EDGE*)edge;
					break;
				}
			}
			if (reEdge)
			{
				break;
			}
			edge = body_edgelist.next();
		}
	}
	else if (faceNameList.size() > 1)
	{
		ENTITY_LIST body_facelist;
		ENTITY_LIST face_edgelist;
		api_get_faces_from_all_entities(owner, body_facelist);
		body_facelist.init();
		ENTITY* face = body_facelist.first();
		while (face != NULL)
		{
			ATTRIB_GEN_NAME* res = NULL;
			api_find_named_attribute(face, "ATTRIB_XACIS_NAME", res);
			if (res != NULL)
			{
				const char* name = ((ATTRIB_GEN_STRING*)res)->value();
				if (std::find(faceNameList.cbegin(), faceNameList.cend(), name)
					!= faceNameList.cend())
				{
					if (face_edgelist.count() == 0)
					{
						api_get_edges_from_all_entities(face, face_edgelist);
					}
					else
					{
						ENTITY_LIST temp_edgelist;
						api_get_edges_from_all_entities(face, temp_edgelist);
						face_edgelist.init();
						ENTITY* edge = face_edgelist.first();
						while (edge != NULL)
						{
							if (temp_edgelist.lookup(edge) > -1)
							{
								reEdge = (EDGE*)edge;
								break;
							}
							edge = face_edgelist.next();
						}
					}
				}
			}
			if (reEdge)
			{
				break;
			}
			face = body_facelist.next();
		}
	}
	return reEdge;
}

VERTEX * xdModelingUtils::getVertex(ENTITY * owner, const std::string & vertexName)
{
	std::vector<std::string> pidlist;
	split(vertexName, &pidlist, "#");
	if (pidlist.size() != 2)
	{
		return nullptr;
	}
	EDGE* edge = getEdge(owner, pidlist[0]);
	if (!edge)
	{
		return nullptr;
	}
	return stod(pidlist[1]) == EDGEVERTEX::Start ? edge->start() : edge->end();
}

FACE* xdModelingUtils::getFace(ENTITY * owner, const std::string& faceName)
{
	FACE* reFace = NULL;
	ENTITY_LIST body_facelist;
	api_get_faces_from_all_entities(owner, body_facelist);
	body_facelist.init();
	ENTITY* face = body_facelist.first();
	while (face != NULL)
	{
		ATTRIB_GEN_NAME* res = NULL;
		api_find_named_attribute(face, "ATTRIB_XACIS_NAME", res);
		if (res != NULL)
		{
			const char* name = ((ATTRIB_GEN_STRING*)res)->value();
			if (faceName == name)
			{
				reFace = (FACE*)face;
				break;
			}
		}
		face = body_facelist.next();
	}
	return reFace;
}

void* xdModelingUtils::GetEntity(ENTITY* owner, std::vector<std::string>& pidlist)
{
	if (pidlist.size() < 1)
	{
		return nullptr;
	}

	if (owner && pidlist[0] == "Body")
	{
		return owner;
	}
	else if (owner && pidlist[0] == "Face" && pidlist.size() == 3)
	{
		return getFace(owner, pidlist[2]);
	}
	else if (owner && pidlist[0] == "Edge" && pidlist.size() == 3)
	{
		return getEdge(owner, pidlist[2]);
	}
	else if (owner && pidlist[0] == "Vertex" && pidlist.size() == 4)
	{
		return getVertex(owner, pidlist[2] + "#" + pidlist[3]);
	}
	else if (owner && pidlist[0] == "FacePoint" && pidlist.size() == 4)
	{
		FACE* face = getFace(owner, pidlist[2]);
		std::vector<std::string> uv;
		split(pidlist[3], &uv, "&");
		if (uv.size() == 2)
		{
			SPAtransf tform = get_owner_transf(face);

			SPApar_box par_box;
			sg_get_face_par_box(face, par_box);
			double u = stod(uv[0]) * (par_box.u_range().end_pt() - par_box.u_range().start_pt()) +
				par_box.u_range().start_pt();
			double v = stod(uv[1]) * (par_box.v_range().end_pt() - par_box.v_range().start_pt()) +
				par_box.v_range().start_pt();
			SPApar_pos par_pos(u, v);
			SPAposition PtPos = face->geometry()->equation().eval_position(par_pos) * tform;
			double* coords = new double[3];
			coords[0] = PtPos.x();
			coords[1] = PtPos.y();
			coords[2] = PtPos.z();
			return coords;
		}
	}
	else if (owner && pidlist[0] == "EdgePoint" && pidlist.size() == 4)
	{
		EDGE* edge = getEdge(owner, pidlist[2]);
		SPAtransf tform = get_owner_transf(edge);
		double u = stod(pidlist[3]) * (edge->end_param() - edge->start_param()) + edge->start_param();
		SPAposition PtPos = edge->geometry()->equation().eval_position(u) * tform;
		double* coords = new double[3];
		coords[0] = PtPos.x();
		coords[1] = PtPos.y();
		coords[2] = PtPos.z();
		return coords;
	}
	else if (!owner && pidlist[0] == "FreePoint" && pidlist.size() == 2)
	{
		std::vector<std::string> strCoords;
		split(pidlist[1], &strCoords, "&");
		if (strCoords.size() == 3)
		{
			double* coords = new double[3];
			coords[0] = stod(strCoords[0]);
			coords[1] = stod(strCoords[1]);
			coords[2] = stod(strCoords[2]);
			return coords;
		}
	}

	return nullptr;
}

int xdModelingUtils::CreatePML(ENTITY_LIST const& facelist, ENTITY_LIST const&  edgelist,
	ENTITY_LIST const&  vertexlist, int layerCount,
	bool joiningEdges, bool joiningVertices, ENTITY_LIST * bodylist)
{
	EXCEPTION_BEGIN;
	EXCEPTION_TRY;
	API_BEGIN;
	// 判断FACE LIST是否从属于同一个BODY
	BODY* ownerBody = NULL;
	int isVal = m_data->IsValid(facelist, ownerBody);
	if (isVal == 1 || !ownerBody)
	{
		printf("the selected faces must are owned by one solid body.\n");
		return 1;
	}
	// 判断Owner是否是长方体
	int isBox = m_data->IsCuboid(facelist);
	if (isVal == 1)
	{
		printf("the owner is not a cuboid.\n");
		joiningEdges = false;
		joiningVertices = false;
	}
	// 
	m_data->MakeFaceLayer(facelist, 0.2, layerCount, bodylist);
	if (joiningEdges)
	{
		m_data->MakeEdgeLayer(facelist, edgelist, 0.2, layerCount, bodylist);
	}
	if (joiningVertices)
	{
		m_data->MakeVertexLayer(edgelist, vertexlist, 0.2, layerCount, bodylist);
	}
	API_END;
	EXCEPTION_CATCH_TRUE;
	if (error_no != 0)
	{
		printf("EXCEPTION ERROR: %s\n", find_err_mess(error_no));
	}
	EXCEPTION_END;
	return 0;
}

bool xdModelingUtils::getCenterAndNormal(FACE * face, SPAposition & center, SPAunit_vector & normal)
{
	if (!face)
	{
		return false;
	}
	SPApar_box par_box;
	SPAtransf tform = get_owner_transf(face);
	logical suc = sg_get_face_par_box(face, par_box);
	center = face->geometry()->equation().eval_position(par_box.mid()) * tform;
	normal = face->geometry()->equation().eval_normal(par_box.mid()) * tform;
	return true;
}

bool xdModelingUtils::isRectangular(FACE* face)
{
	if (!face || !is_planar_face(face))
	{
		return false;
	}
	ENTITY_LIST elist;
	api_get_edges_from_all_entities(face, elist);
	if (elist.count() != 4 || !is_linear_edge(elist[0]) || !is_linear_edge(elist[1])
		|| !is_linear_edge(elist[2]) || !is_linear_edge(elist[3]))
	{
		return false;
	}
	
	SPAvector dir_array[4];
	for (int i = 0; i < 4; ++i)
	{
		dir_array[i] = ((EDGE*)elist[i])->start_deriv();
		if (i > 0 && !are_perpendicular(dir_array[i - 1], dir_array[i]))
		{
			return false;
		}
	}

	return true;
}

bool xdModelingUtils::isCoaxial(FACE* face)
{
	if (!face || !is_planar_face(face))
	{
		return false;
	}
	ENTITY_LIST elist;
	api_get_edges_from_all_entities(face, elist);
	if (elist.count() != 2 || !is_circular_edge(elist[0]) || !is_circular_edge(elist[1])
		|| !is_closed_curve((EDGE*)elist[0]) || !is_closed_curve((EDGE*)elist[1]))
	{
		return false;
	}
	SPAposition center1, center2;
	get_curve_center((EDGE*)elist[0], center1);
	get_curve_center((EDGE*)elist[1], center2);
	if (!is_equal(center1,center2))
	{
		return false;
	}
	return true;
}


