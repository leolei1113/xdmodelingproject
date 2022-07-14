/*! \file ShapeUtilities.h
*   \brief ShapeUtilities���ͷ�ļ�
*   \author HST
*   \date 2020.10.8
*/

#ifndef SHAPEUTILITIES_H
#define SHAPEUTILITIES_H

#include "xdModelingDataType.h"
#include "xdModelingHeader.h"

#include <TopoDS_Shape.hxx>
#include <TopoDS_ListOfShape.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopoDS_Edge.hxx>
#include <Geom_Plane.hxx>
#include <TopTools_ListOfShape.hxx>

#include <vector>

enum CoinsideType
{
    Containing,
    Contained,
    PartCoinside,
    NotConside,
    Same,
    Neither
};

/*! \class ShapeUtilities
*   \brief ���ڴ��һЩ����TopoDS_Shape�ľ�̬����
 */
class XDMODELING_API ShapeUtilities
{
public:
    /*! \fn ShapeUtilities()
    *  \brief ShapeUtilities�Ĺ��캯��.
    */
    ShapeUtilities();

    /*! \fn ~ShapeUtilities()
    *  \brief ShapeUtilities����������.
    */
    ~ShapeUtilities();

    /*! \fn static bool shapeFilter(TopoDS_Shape& origShape, TopAbs_ShapeEnum targetType)
    *  \brief ����ģ�͵Ľӿڣ��������˽ṹ�н���һ�������˵Ĳ㼶��ֱ���ﵽĿ��㼶�������δ�ﵽ֮ǰ���Ѿ��ж���������򷵻�false.
    *  \param origShape ��Ҫ���й��˵�TopoDS_Shape����.
    *  \param targetType Ŀ�����˼���.
    *  \param filterShape �����������.
    *  \return �����Ƿ���˳ɹ�.
    */
    static bool shapeFilter(const TopoDS_Shape& origShape, TopAbs_ShapeEnum targetType, TopoDS_Shape& filterShape);

    /*! \fn static bool shapeCover(const TopoDS_Shape& origShape, TopoDS_Shape& coveredShape)
    *  \brief ģ����������һ���Ľӿ�.
    *  \param origShape ��Ҫ����������TopoDS_Shape����.
    *  \param coveredShape �����������.
    *  \return �����Ƿ������ɹ�.
    */
    static bool shapeCover(const TopoDS_Shape& origShape, TopoDS_Shape& coveredShape);

    /*! \fn static TopoDS_ListOfShape findEdgeOwnerFaces(const TopoDS_Shape& bdy, const TopoDS_Shape& edge)
    *  \brief ��body�в���edge��������ƽ��.
    *  \param bdy ����body��TopoDS_Shape����.
    *  \param edge ��Ҫ���в��ҵ�edge��TopoDS_Shape����.
    *  \return ���ز��ҵ�������ƽ��.
    */
    static TopoDS_ListOfShape findEdgeOwnerFaces(const TopoDS_Shape& bdy, const TopoDS_Shape& edge);

    /*! \fn static TopoDS_Shape shapeTransformFromLocalToGlobal(double* localCoor, const TopoDS_Shape& shape, const bool& isCopy = false)
    *  \brief ���ݾֲ�����ϵ��ģ��ת����ȫ������ϵ�еĽӿ�.
    *  \param localCoor �ֲ�����ϵ����.
    *  \param shape �޸�λ�õ�ģ��.
    *  \param isCopy �Ƿ񿽱���Ĭ�ϲ�����
    *  \return ���λ���޸ĵ�ģ��.
    */
    static TopoDS_Shape shapeTransformFromLocalToGlobal(double* localCoor, const TopoDS_Shape& shape, const bool& isCopy = false);

    /*! \fn static TopoDS_Shape shapeTransformFromGlobalToLocal(double* localCoor, const TopoDS_Shape& shape, const bool& isCopy = false)
    *  \brief ���ݾֲ�����ϵ��ģ��ת�����ֲ�����ϵ�еĽӿ�.
    *  \param localCoor �ֲ�����ϵ����.
    *  \param shape �޸�λ�õ�ģ��.
    *  \param isCopy �Ƿ񿽱���Ĭ�ϲ�����
    *  \return ���λ���޸ĵ�ģ��.
    */
    static TopoDS_Shape shapeTransformFromGlobalToLocal(double* localCoor, const TopoDS_Shape& shape, const bool& isCopy = false);

    /*! \fn static void vectorTransformFromLocalToGlobal(double* localCoor, const double& sourceVecX, const double& sourceVecY, const double& sourceVecZ, double& xVec, double& yVec, double& zVec)
    *  \brief �������Ӿֲ�����ϵת��ȫ���еĽӿ�.
    *  \param sourceVecX ת��ǰ����x�������.
    *  \param sourceVecY ת��ǰ����y�������.
    *  \param sourceVecZ ת��ǰ����z�������.
    *  \param xVec ת��������x�������.
    *  \param yVec ת��������y�������.
    *  \param zVec ת��������z�������.
    */
    static void vectorTransformFromLocalToGlobal(double* localCoor, const double& sourceVecX, const double& sourceVecY, const double& sourceVecZ,
        double& xVec, double& yVec, double& zVec);

    /*! \fn static void pntTransformFromLocalToGlobal(double* localCoor, const double& sourceX, const double& sourceY, const double& sourceZ, double& x, double& y, double& z)
    *  \brief ��������Ӿֲ�����ϵת��ȫ���еĽӿ�.
    *  \param sourceX ת��ǰ������x�������.
    *  \param sourceY ת��ǰ������y�������.
    *  \param sourceZ ת��ǰ������z�������.
    *  \param x ת���������x�������.
    *  \param y ת���������y�������.
    *  \param z ת���������z�������.
    */
    static void pntTransformFromLocalToGlobal(double* localCoor,
        const double& sourceX, const double& sourceY, const double& sourceZ,
        double& x, double& y, double& z);

    /*! \fn static void getAxis(const DATUMAXISTYPE& axis, double& xAxis, double& yAxis, double& zAxis)
    *  \brief ����ö�����ͻ�ȡĿ�����x��y��z�����Ľӿ�.
    *  \param axis ������ö��.
    *  \param x ����������x����.
    *  \param y ����������y����.
    *  \param z ����������z����.
    */
    static void getAxis(const DATUMAXISTYPE& axis, double& xAxis, double& yAxis, double& zAxis, double isanti);

    /*! \fn static void getRadius(const std::vector<std::vector<double>>& points, double& xCenter, double& yCenter, double& zCenter, double& radius)
    *  \brief �����������Բ�ĺͰ뾶�Ľӿ�.
    *  \param points ��������.
    *  \param xCenter Բ��x����.
    *  \param yCenter Բ��y����.
    *  \param zCenter Բ��z����.
    *  \param radius �뾶.
    */
    static void getRadius(const std::vector<std::vector<double>>& points, double& xCenter, double& yCenter, double& zCenter, double& radius);


    /*! \fn static bool isShapePlannar(const TopoDS_Shape& shape, gp_Pln& plane)
    *  \brief �ж�ģ���Ƿ���ƽ��Ľӿڣ�����ȡ��ƽ��.
    *  \param shape �����жϵ�ģ��.
    *  \param plane �����ƽ��ʱ���صļ���ƽ��.
    *  \return �Ƿ���ƽ��ģ��.
    */
    static bool isShapePlannar(const TopoDS_Shape& shape, gp_Pln& plane);

	/*! \fn static int getNumTopoType(const TopoDS_Shape& shape, const TopAbs_ShapeEnum& type)
	*  \brief ��ȡģ�͵�ĳ�����˵���ľ.
	*  \param shape ���л�ȡ��ģ��.
	*  \param type ��Ҫ�жϵ���������.
	*  \return ������Ŀ.
	*/
	static int getNumTopoType(const TopoDS_Shape& shape, const TopAbs_ShapeEnum& type);

    /*! \fn static bool FindConnectedEdges(const TopoDS_Shape& F, const TopoDS_Vertex& V, TopoDS_Edge& E1, TopoDS_Edge& E2)
    *  \brief ��ȡģ���У������������.
    *  \param F ���в��ҵ�ģ��.
    *  \param V ���ģ������.
    *  \param edges �����߼���.
    *  \return �Ƿ����ҵ�������.
    */
    static bool FindConnectedEdges(const TopoDS_Shape& F/*orig const TopoDS_Face& F*/, const TopoDS_Vertex& V, TopTools_ListOfShape& edges);

    /*! \fn static bool PlanarBaryCalculate(const TopoDS_Shape& planarShape, gp_XYZ& bary)
    *  \brief �򵥻�ȡģ�����ĵķ�����ͨ��ƾ�ݶ�������.
    *  \param planarShape ���д����ģ��.
    *  \param bary �������ģ������.
    *  \return �Ƿ�ɹ�����.
    */
    static bool PlanarBaryCalculate(const TopoDS_Shape& planarShape, gp_XYZ& bary);

    static bool GetCircleCenter(Handle(Geom_Curve) curve, gp_Pnt& pnt);

    static void GetAllNonCompoundShapes(const TopoDS_Shape& orgShape, TopTools_ListOfShape& collections);

    static void GetAllNonCompoundShapes(const TopoDS_Shape& orgShape, TopTools_ListOfShape& solids, TopTools_ListOfShape& faces, TopTools_ListOfShape& edges);

    static void GetAllFaces(const TopoDS_Shape& orgShape, TopTools_ListOfShape& collections);

    static void GetShapeTypeRange(const TopTools_ListOfShape& shapes, TopAbs_ShapeEnum& lower, TopAbs_ShapeEnum& upper);

    static Handle(Geom_Surface) GetBasicSurface(Handle(Geom_Surface) surf, GeomAbs_SurfaceType& type);

    static Handle(Geom_Curve) GetBasicCurve(Handle(Geom_Curve) curve);

    static bool GetPntUVonFace(TopoDS_Shape& face, gp_Pnt& pnt, double& UValue, double& VValue);

    static bool GetPntUVonFace(TopoDS_Shape face, TopoDS_Shape edge, gp_Pnt& pnt, double& UValue, double& VValue);

    static bool GetPntUVonEdge(TopoDS_Shape edge, gp_Pnt& pnt, double& UValue);

    static bool IsShapeGeomSame(const TopoDS_Shape shape1, const TopoDS_Shape shape2, const TopAbs_ShapeEnum type);

    static void FindAllCells(const TopoDS_Shape shape, TopTools_ListOfShape& collections);

    static bool ConvertBoundedCurve(TopoDS_Shape edge, Handle(Geom_BoundedCurve)& BndCurve);

    static bool SameDimension(const TopTools_ListOfShape collections, int& minDim, int& maxDim);

    static bool FaceNormal(const TopoDS_Face face, const double& u, const double& v, gp_Dir& dir);

    static bool FaceMiddleNormal(const TopoDS_Face face, gp_Dir& dir);

    static bool EdgeIsLinear(const TopoDS_Edge edge);

    static bool ReverseFaceNormal4CellsBuild(const TopTools_ListOfShape cellsArgs, const TopTools_ListOfShape cellsTools,
        TopTools_ListOfShape& newcellsArgs, TopTools_ListOfShape& newcellsTools);

    static CoinsideType IsEdgeCoincide(const TopoDS_Shape blankedge, const TopoDS_Shape tooledge, const double tolerance);

    static void BoxEFSort(const TopoDS_Shape box, TopTools_IndexedMapOfShape& faces, TopTools_IndexedMapOfShape& edges);

    static void GetMultishellSolids(const TopTools_ListOfShape solids, TopTools_ListOfShape& singleShellSolid, TopTools_ListOfShape& multiShellSolid);

    static void UnifyAllFaceNormal(const gp_Dir refDir, TopTools_ListOfShape& cellsArgs);

    static void FindAllCellsWithoutCheck(const TopoDS_Shape shape, TopTools_ListOfShape& collections);

    static bool IsShapePlannarNumericalSolution(const TopoDS_Shape shape, gp_Pln& plane, double precision);

    static bool BuildSurfaceEdge(TopoDS_Shape owner, TopoDS_Shape preshape, TopoDS_Shape curshape, std::tuple<double, double> preValue,
        std::tuple<double, double> curValue, TopoDS_Edge& targetedge);

    static bool GetUVPoint(double normalizedT, TopoDS_Edge edge, TopoDS_Face face,
        bool& firstpcurve, bool& secondpcurve, gp_Pnt2d& p);

    static void GetUVPoint(const double& normalizedU, const double& normalizedV, TopoDS_Face face, gp_Pnt2d& p);

    static bool shapeFilter(TopoDS_Shape& origshape, TopAbs_ShapeEnum targettype,
        TopoDS_Shape& filtershape);

    static bool GetPntUonEdge(TopoDS_Shape edge, gp_Pnt pnt, double& uvalue);
};

#endif
