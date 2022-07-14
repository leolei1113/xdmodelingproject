#ifndef _XD_MODELING_DATA_TYPE_H_
#define _XD_MODELING_DATA_TYPE_H_

enum LOGICAL
{
	True = 0,
	False
};

enum BOOLTYPE
{
	Unite = 0,
	Substract,
	Intersect,
	Imprint
};

enum DATUMAXISTYPE
{
	X = 0,
	Y,
	Z
};

enum SLIVERTOLTYPE
{
	scaleFactor,
	edgeLength,
};

struct HealOption
{
	int healingtype{ 1 };
	bool perfmTolStich;
	double stitchTolerance;
	bool stopAfterFirstErr;
	bool isSimplify{ false };
	double simplifyTolerance;
	double MaxGenRadius;
	int simplifyType;
	bool isTightenGaps;
	double maxTightenGaps;
	bool isRmvSmallEdges;
	bool isRmvSmallFaces;
	bool isRmvSliverFaces;
	double smallEdgeTol;
	double smallFaceTols;
	SLIVERTOLTYPE sliverTolType;
	bool UseScalFactor;
	bool UseEdgeWidth;
	double SliverFaceTol;
	bool isRmvHole;
	bool isRmvChamfer;
	bool isRmvBlends;
	double holeMaxRadius;
	double chamferMaxRadius;
	double blendMaxRadius;
	bool useAreaLimit{ false };
	bool useVolumeLimit{ false };
	double faceAreaLimit;
	double volumeLimit;
	bool repairFaceInt{ false };
};

enum DATUMPLANETYPE
{
	YZ = 0,
	XZ,
	XY
};

enum SWEEPDRAFTTYPE
{
	Sweep_Extended = 0,
	Sweep_Round,
	Sweep_Natural
};
enum CHAMFERTYPE
{
	Symmetric = 0,
	Asymmetric
};

enum MULTISEGMENTTYPE
{
	Line = 0,
	Spline,
	ThreePointArc,
	AngularArc
};

enum KEEPSIDE
{
	Positive = 0,
	Negative,
	Both
};

enum SECTIONTYPE
{
	NONE = 0,
	LINE,
	RECTANGLE,
	CIRCULAR,
	ISOSECLESTRAPEZOID,
};

enum ORIENTATION
{
	AUTO = 0,
	XDIRECTION,
	YDIRECTION,
	ZDIRECTION,
};

enum BENDTYPE
{
	Corner,
	Curved,
};

enum PickedPointType
{
	GridPoint = 0,
	FaceCenter,
	ArcCenter,
	EdgeStart,
	EdgeMiddle,
	EdgeEnd,
	EdgeQ14,
	EdgeQ34,
	FacePoint,
	EdgePoint,
	FreePoint
};

enum PLANETYPE
{
	Rectangular = 0,
	Coaxial,
	General
};

enum EDGEVERTEX
{
	Start = 0,
	End
};

enum BODYTYPE
{
	NotBody = 0,
	WireBody,
	SheetBody,
	SolidBody
};

enum SELECTEDENTITYTYPE
{
	NotSelEntity = 0,
	Vertex = 1 << 0,
	Edge = 1 << 1,
	Face = 1 << 2,
	Body = 1 << 3,
};

enum TOPOTYPE
{
	InvalidTopo = 0,
	TopoVertex = 1 << 0,
	TopoEdge = 1 << 1,
	TopoFace = 1 << 2,
	TopoBody = 1 << 3,
	TopoLoop = 1 << 4,
	TopoCoedge = 1 << 5,
	TopoWire = 1 << 6
};


struct PolylineSegment
{
	MULTISEGMENTTYPE Type;
	int StartIndex;
	int NoOfPoints;
	double ArcAngle;
	DATUMAXISTYPE CenterAxis;
};

struct CrossSection
{
	SECTIONTYPE SectionType;
	ORIENTATION Orientation;
	double width;
	double TopWidth;
	double Height;
	int segnum;
	BENDTYPE bendType;
};

enum NURBSTYPE
{
	InterpolatedPoints,
	ControlPoints
};

#endif // _XD_MODELING_DATA_TYPE_H_
