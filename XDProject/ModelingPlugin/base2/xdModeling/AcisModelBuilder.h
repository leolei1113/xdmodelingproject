#pragma once
#include "GeomModelBuilder.h"

class EDGE;
class BODY;

class XDMODELING_API AcisModelBuilder :
	public GeomModelBuilder
{
public:
	AcisModelBuilder();

	virtual ~AcisModelBuilder();

	virtual void builderBegin();

	virtual void builderEnd();

	virtual bool addModelAttribute(void* modelEntity, const std::string& propertyName, const int& value);

	virtual bool addModelAttribute(void* modelEntity, const std::string& propertyName, const std::string& value);

	virtual bool addModelAttribute(void* modelEntity, const std::string& propertyName, const double& value);

	virtual bool addModelAttribute(void* modelEntity, double dRed, double dGreen, double dBlue);

	virtual double tolerance();

	// None used
	void HandlePrimitiveAnnotations();

	// None used
	void HandleCopyAnnotations();

	// None used
	void HandleSplitAnnotations();

	// None used
	static std::string CreateEdgeNameFromEntity(void* pSelEdge);

public:
	//create functions
	//vertex
	virtual bool createVertex(const double& xPos, const double& yPos, const double& zPos,
		const char* name, std::string& newEntry, void*& newShapeEntry);

	//line
	virtual bool createLine(const double& xPos1, const double& yPos1, const double& zPos1,
		const double& xPos2, const double& yPos2, const double& zPos2,
		const char* name, std::string& newEntry, void*& newShapeEntry);

	//plane
	virtual bool createPlane(const double& xPos1, const double& yPos1, const double& zPos1,
		const double& xPos2, const double& yPos2, const double& zPos2,
		const char* name, std::string& newEntry, void*& newShapeEntry);

	//3D geometry
	virtual bool createCuboid(double* wcsMatrix, const double& xStart, const double& yStart, const double& zStart,
		const double& xSize, const double& ySize, const double& zSize,
		const char* name, std::string& newEntry, void*& newShapeEntry);

	virtual bool createCylinder(double* wcsMatrix, const double& botCenterX, const double& botCenterY, const double& botCenterZ,
		const double& raidus, const double& height, const DATUMAXISTYPE& axis,
		const char* name, std::string& newEntry, void*& newShapeEntry);

	virtual bool createSphere(double* wcsMatrix, const double& xCenter, const double& yCenter, const double& zCenter, const double& raidus,
		const char* name, std::string& newEntry, void*& newShapeEntry);

	virtual bool createCone(double* wcsMatrix, const double& botCenterX, const double& botCenterY, const double& botCenterZ,
		const double& lowerRadius, const double& upperRadius, const double& height, const DATUMAXISTYPE& axis,
		const char* name, std::string& newEntry, void*& newShapeEntry);

	virtual bool createRegularPolyhedron(double* wcsMatrix, const double& botCenterX, const double& botCenterY, const double& botCenterZ,
		const double& startX, const double& startY, const double& startZ,
		const double& height, const int& numSides, const DATUMAXISTYPE& axis,
		const char* name, std::string& newEntry, void*& newShapeEntry);

	virtual bool createTorus(double* wcsMatrix, const double& centerX, const double& centerY, const double& centerZ,
		const double& minorRadius, const double& majorRadius, const DATUMAXISTYPE& axis,
		const char* name, std::string& newEntry, void*& newShapeEntry);

	virtual bool createFlare(double* wcsMatrix, const double& botCenterX, const double& botCenterY, const double& botCenterZ,
		const double& lowerWidth, const double& lowerDepth, const double& upperWidth, const double& upperDepth,
		const double& height, const DATUMAXISTYPE& axis,
		const char* name, std::string& newEntry, void*& newShapeEntry);

	virtual bool createEllipsoid(double* wcsMatrix, const double& centerX, const double& centerY, const double& centerZ,
		const double& majRadius, const double& ratio, const DATUMAXISTYPE& axis,
		const char* name, std::string& newEntry, void*& newShapeEntry);

	//2D geometry
	virtual bool createRectangle(double* wcsMatrix, const double& startX, const double& startY, const double& startZ,
		const double& width, const double& height, const DATUMAXISTYPE& axis, const LOGICAL& isCovered,
		const char* name, std::string& newEntry, void*& newShapeEntry);

	virtual bool createCircle(double* wcsMatrix, const double& centerX, const double& centerY, const double& centerZ,
		const double& radius, const DATUMAXISTYPE& axis, const LOGICAL& isCovered,
		const char* name, std::string& newEntry, void*& newShapeEntry);

	virtual bool createRegularPolygon(double* wcsMatrix, const double& centerX, const double& centerY,
		const double& centerZ, const double& startX, const double& startY, const double& startZ,
		const int& numSides, const DATUMAXISTYPE& axis, const LOGICAL& isCovered,
		const char* name, std::string& newEntry, void*& newShapeEntry);

	virtual bool createEllipse(double* wcsMatrix, const double& centerX, const double& centerY, const double& centerZ,
		const double& majRadius, const double& ratio, const DATUMAXISTYPE& axis, const LOGICAL& isCovered,
		const char* name, std::string& newEntry, void*& newShapeEntry);

	virtual bool createEquationSurface(double* wcsMatrix, const char* xuvFunction, const char* yuvFunction, const char* zuvFunction,
		const double& startU, const double& strEndU, const double& startV, const double& strEndV,
		const char* name, std::string& newEntry, void*& newShapeEntry);

	//1D geometry
	virtual bool createUnusedEntity(const char* name, std::string& newEntry, void*& newShapeEntry);

	virtual bool createPolyline(double* wcsMatrix, std::vector<std::vector<double>>& points, std::vector<PolylineSegment>& segments,
		const LOGICAL& isClosed, const LOGICAL& isCovered,
		const char * name, std::string & newEntry, void *& resEntity);

	virtual bool createHelix(double* wcsMatrix, const double& centerX, const double& centerY, const double& centerZ,
		const double& startX, const double& startY, const double& startZ,
		/*const double& lowerRadius, */const double& upperRadius, const double& height,
		const double& turns, const double& rightHanded, const DATUMAXISTYPE& axis,
		const char* name, std::string& newEntry, void*& newShapeEntry);

	virtual bool createSpiral(double* wcsMatrix, const double& centerX, const double& centerY, const double& centerZ,
		const double& startX, const double& startY, const double& startZ,
		/*const double& minRadius, */const double& maxRadius, const double& turns, const double& rightHanded,
		const DATUMAXISTYPE& axis, const char* name, std::string& newEntry, void*& newShapeEntry);

	virtual bool createEquationCurve(double* wcsMatrix, const char* xtFunction, const char* ytFunction,
		const char* ztFunction, const double& startT, const double& strEndT,
		const char* name, std::string& newEntry, void*& newShapeEntry);

	//boolean
	virtual bool makeBoolean(const BOOLTYPE& boolType, const LOGICAL& isKeep, const std::list<void*>& entryList,
		const char* name, std::string& newEntry, void*& newShape);

	//virtual bool createBooleanCut(const std::list<std::string>& entryList, const char* name, std::string& newEntry, void*& newShapeEntry);

	//virtual bool createBooleanCommon(const std::string& mainEntry, const std::string& toolEntry, const char* name, std::string& newEntry, void*& newShapeEntry);

	//virtual bool createBooleanFuse(const std::list<std::string>& entryList, const char* name, std::string& newEntry, void*& newShapeEntry);

	//virtual bool createBooleanFillet(void* entry, const std::list<std::string>& faceNames, const double& radius, const char* name, std::string& newEntry, void*& newShapeEntry);

	//virtual bool createBooleanNonReg(const std::list<std::string>& entryList, const char* name, std::string& newEntry, void*& newShapeEntry);

	//transform
	virtual bool translate(void* entry, double* wcsMatrix, const double& vectorX, const double& vectorY, const double& vectorZ,
		const int& totalNumber, const LOGICAL& isAttach, const char* name, std::string& newEntry, void*& resEntity);

	virtual bool rotate(void* entry, double* wcsMatrix, const double& axisPosX, const double& axisPosY, const double& axisPosZ,
		const double& axisVecX, const double& axisVecY, const double& axisVecZ, const double& angle,
		const int& totalNumber, const LOGICAL& isAttach, const char* name, std::string& newEntry, void*& resEntity);

	virtual bool mirror(void* entry, double* wcsMatrix, const double& basePosX, const double& basePosY, const double& basePosZ,
		const double& normalVecX, const double& normalVecY, const double& normalVecZ, const LOGICAL& isCopy,
		const char* name, std::string& newEntry, void*& resEntity);

	virtual bool scale(void* entry, double* wcsMatrix, const double& basePosX, const double& basePosY, const double& basePosZ,
		const double& scaleX, const double& scaleY, const double& scaleZ, const LOGICAL& isCopy,
		const char* name, std::string& newEntry, void*& resEntity);

	//extend
	virtual bool sweepAlongVector(void* entry, double* wcsMatrix, const double& vectorX, const double& vectorY,
		const double& vectorZ, const double& draftAngle, const SWEEPDRAFTTYPE& draftType,
		const char* name, std::string& newEntry, void*& resEntity);

	virtual bool sweepAroundAxis(void* entry, double* wcsMatrix, const double& axisPosX, const double& axisPosY,
		const double& axisPosZ, const double& axisVecX, const double& axisVecY, const double& axisVecZ,
		const double& angle, const double& draftAngle, const SWEEPDRAFTTYPE& draftType,
		const char* name, std::string& newEntry, void*& resEntity);

	virtual bool sweepAlongPath(void* profileEntry, void* pathEntry, const double& twistAngle,
		const double& draftAngle,
		const SWEEPDRAFTTYPE& draftType, const char* name, std::string& newEntry, void*& resEntity);

	virtual bool thicken(void* entry, const double& thickness, const LOGICAL& isDoubleSides,
		const char* name, std::string& newEntry, void*& resEntity);

	//blend
	virtual bool fillet(const std::list<void*>& edgesOrVertices,
		const double& radius, const double& setback,
		const char* name, std::string& newEntry, void*& resEntity);

	virtual bool chamfer(const std::list<void*>& edgesOrVertices,
		const double& setback, const double& lDistance, const double& rDistance,
		const CHAMFERTYPE& chamferType,
		const char* name, std::string& newEntry, void*& resEntity);

	//surface
	virtual bool moveFaces(double* wcsMatrix,const std::list<void*>& entityList,
		const double& vectorX, const double& vectorY, const double& vectorZ,
		const char* name, std::string& newEntry, void*& resEntity);

	virtual bool offsetFaces(const std::list<void*>& entityList,
		const double& distance, const char* name, std::string& newEntry, void*& resEntity);

	virtual bool copyFaces(void* entity,
		const char* name, std::string& newEntry, void*& resEntity);

	virtual bool createEntityPIDForEntity(void* curBdy, std::string& newTag);

	virtual bool createCloneEntity(void* curBdy, const char* name,
		std::string& newEntry, void*& resEntity);

	virtual bool split(void* entry, double* wcsMatrix, const double& basePosX, const double& basePosY, const double& basePosZ,
		const double& normalVecX, const double& normalVecY, const double& normalVecZ, const KEEPSIDE& keepSide,
		const char* name, std::string& newEntry, void*& resEntity);

	virtual bool stitch(const std::list<void*>& entryList, const char* name, std::string& newEntry, void*& newShape);

	virtual bool connect(const std::list<void*>& entryList, const char* name, std::string& newEntry, void*& newShape);

	virtual bool cover(const std::list<void*>& entryList, const char* name, std::string& newEntry, void*& newShape);

	virtual bool sliceByFaces(void* entry, const std::list<void*>& tooFacelList, const char* name, std::string& newEntry,
		void*& newShape);

	virtual bool detachEdges(void* entry, const std::list<void*>& edgeList, LOGICAL isCreated, const char* name,
		std::string& newEntry, void*& newShape);

	virtual bool createPoint(double* wcsMatrix, const double& dCoordX, const double& dCoordY, const double& dCoordZ,
		const char* name, std::string& newEntry, void*& newShape);

	//hard point
	virtual bool createHardPoint(const std::list<void*>& edges,
		const double& dUValue, const char* name, std::string& newEntry, void*& resEntity);
};

