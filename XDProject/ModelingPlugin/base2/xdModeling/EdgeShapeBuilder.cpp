#include "EdgeShapeBuilder.h"
#include "ShapeUtilities.h"
#include "Geom_BoundedMathCurve.hxx"
#include "TransformShapeBuilder.h"

#include <GC_MakeArcOfEllipse.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <gp_Elips.hxx>
#include <TopoDS_Edge.hxx>
#include <BRep_Builder.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <gp_Ax2.hxx>
#include <gp_Circ.hxx>
#include <GeomAPI_PointsToBSpline.hxx>
#include <Geom_BSplineCurve.hxx>
#include <Geom_VectorWithMagnitude.hxx>
#include <gp_Hypr.hxx>
#include <gp_Parab.hxx>
#include <GeomAPI_Interpolate.hxx>
#include <Geom_CylindricalSurface.hxx>
#include <Geom2d_TrimmedCurve.hxx>
#include <gp_Lin2d.hxx>
#include <GCE2d_MakeSegment.hxx>
#include <Geom_ConicalSurface.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepProj_Projection.hxx>
#include <BRepLib.hxx>
#include <BRepTools.hxx>
#include <GeomConvert.hxx>

#include <algorithm>
#include <Geom_BSplineCurve.hxx>
#include <BRep_Tool.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <GeomConvert_ApproxCurve.hxx>


EdgeShapeBuilder::EdgeShapeBuilder()
{
}


EdgeShapeBuilder::~EdgeShapeBuilder()
{
}

bool EdgeShapeBuilder::createLine(const double& xPos1, const double& yPos1, const double& zPos1, const double& xPos2, const double& yPos2, const double& zPos2)
{
    try
    {
        //line
        gp_Pnt p1(xPos1, yPos1, zPos1), p2(xPos2, yPos2, zPos2);
        BRepBuilderAPI_MakeEdge edgeBuilder(p1, p2);
        edgeBuilder.Build();
        if (!edgeBuilder.IsDone())
        {
            return false;
        }

        setShape(edgeBuilder.Shape());

        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool EdgeShapeBuilder::createThreePointArc(std::vector<std::vector<double>>& points)
{
    try
    {
        if (points.size() != 3)
        {
            return false;
        }

        double xCenter, yCenter, zCenter, radius;
        ShapeUtilities::getRadius(points, xCenter, yCenter, zCenter, radius);

        gp_Pnt pStart(points[0][0], points[0][1], points[0][2]);
        gp_Pnt pEnd(points[2][0], points[2][1], points[2][2]);
        gp_Pnt pMiddle(points[1][0], points[1][1], points[1][2]);

        gp_Vec dir1(pMiddle, pStart);
        gp_Vec dir2(pMiddle, pEnd);

        gp_Pnt pCenter(xCenter, yCenter, zCenter);
        gp_Vec dir = dir2.Crossed(dir1);
        gp_Ax2 axes(pCenter, dir);

        gp_Circ circle(axes, radius);

        //line
        BRepBuilderAPI_MakeEdge edgeBuilder(circle, pStart, pEnd);
        edgeBuilder.Build();
        if (!edgeBuilder.IsDone())
        {
            return false;
        }

        setShape(edgeBuilder.Shape());

        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool EdgeShapeBuilder::createAngularArc(std::vector<std::vector<double>>& points, const double& angleInDeg, const double& xAxis, const double& yAxis, const double& zAxis, std::vector<double>& newEndPnt)
{
    try
    {
        if (points.size() != 3)//三个点进来的，中心点，起始点和终止点，但是终止点的位置不定对
        {
            return false;

        }

        //         gp_Pnt pCenter(points[0][0], points[0][1], points[0][2]);
        //         gp_Pnt pStart(points[1][0], points[1][1], points[1][2]);
        gp_Pnt pCenter(points[1][0], points[1][1], points[1][2]);
        gp_Pnt pStart(points[0][0], points[0][1], points[0][2]);

        //         gp_Dir dir(xAxis, yAxis, zAxis);
        Geom_VectorWithMagnitude targetVecWithMag(pCenter, pStart);
        double radius = targetVecWithMag.Magnitude();

        double xRealAxis = xAxis, yRealAxis = yAxis, zRealAxis = zAxis;
        if (angleInDeg < 0)
        {
            xRealAxis *= -1.;
            yRealAxis *= -1.;
            zRealAxis *= -1.;
        }
        gp_Dir dir(xRealAxis, yRealAxis, zRealAxis);
        gp_Ax2 axes(pCenter, dir);
        gp_Pnt pEnd = pStart.Rotated(gp_Ax1(pCenter, dir), abs(angleInDeg)* M_PI / 180.);
        gp_Circ circle(axes, radius);

        newEndPnt.push_back(pEnd.X());
        newEndPnt.push_back(pEnd.Y());
        newEndPnt.push_back(pEnd.Z());

        //line
        BRepBuilderAPI_MakeEdge edgeBuilder(circle, pStart, pEnd);
        edgeBuilder.Build();
        if (!edgeBuilder.IsDone())
        {
            return false;
        }

        setShape(edgeBuilder.Shape());

        return true;
    }
    catch (...)
    {
        return false;
    }

}

bool EdgeShapeBuilder::createPolyLine(std::vector<std::vector<double>>& points)
{
    try
    {
        if (points.size() < 2)
        {
            return false;
        }

        TopoDS_Wire finalWire;
        BRep_Builder B;
        B.MakeWire(finalWire);

        for (int i = 0; i < points.size() - 1; ++i)
        {
            gp_Pnt p1(points[i][0], points[i][1], points[i][2]), p2(points[i + 1][0], points[i + 1][1], points[i + 1][2]);
            BRepBuilderAPI_MakeEdge edgeBuilder(p1, p2);
            edgeBuilder.Build();
            if (!edgeBuilder.IsDone())
            {
                return false;
            }

            B.Add(finalWire, edgeBuilder.Shape());
        }

        setShape(finalWire);

        return true;
    }
    catch (...)
    {
        return false;
    }

    return true;
}

bool EdgeShapeBuilder::createBSpline(std::vector<std::vector<double>>& points)
{
    try
    {
        int num = points.size();
        if (num < 1)
        {
            return false;
        }

        TColgp_Array1OfPnt pointsArray(1, num);
        TColgp_Array1OfPnt finalPointsArray;

        for (int i = 1; i <= num; i++)
        {
            pointsArray.SetValue(i, gp_Pnt(points[i - 1][0], points[i - 1][1], points[i - 1][2]));
        }

        //最后两点一样的话
        if (pointsArray.Value(num - 1).IsEqual(pointsArray.Value(num), Precision::Confusion()))
        {
            finalPointsArray = new TColgp_Array1OfPnt(1, num - 1);
            for (int i = 1; i <= num - 1; i++)
            {
                finalPointsArray.SetValue(i, pointsArray.Value(i));
            }
        }
        //不存在最后两点一样的话
        else
        {
            finalPointsArray = pointsArray;
        }

        GeomAPI_PointsToBSpline aPointToBSpline(finalPointsArray);
        Handle(Geom_BSplineCurve) SPL;
        SPL = aPointToBSpline.Curve();

        BRepBuilderAPI_MakeEdge edgeBuilder(SPL);
        edgeBuilder.Build();
        if (!edgeBuilder.IsDone())
        {
            return false;
        }

       /* Standard_Boolean ok = BRepTools::Write(edgeBuilder.Shape(), "C:\\Users\\Administrator\\Desktop\\cbf\\test.brep");
        if (!ok)
        {
            return false;
        }*/


        setShape(edgeBuilder.Shape());

        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool EdgeShapeBuilder::createInterpolateBSpline(std::vector<std::vector<double>>& points)
{
    try
    {
        int num = points.size();
        if (num < 1)
        {
            return false;
        }

        Handle(TColgp_HArray1OfPnt) pointsArray = new TColgp_HArray1OfPnt(1, num);
        Handle(TColgp_HArray1OfPnt) finalPointsArray;

        for (int i = 1; i <= num; i++)
        {
            pointsArray->SetValue(i, gp_Pnt(points[i - 1][0], points[i - 1][1], points[i - 1][2]));
        }
        //最后两点一样的话
        if (pointsArray->Value(num - 1).IsEqual(pointsArray->Value(num), Precision::Confusion()))
        {
            finalPointsArray = new TColgp_HArray1OfPnt(1, num - 1);
            for (int i = 1; i <= num - 1; i++)
            {
                finalPointsArray->SetValue(i, pointsArray->Value(i));
            }
        }
        //不存在最后两点一样的话
        else
        {
            finalPointsArray = pointsArray;
        }

        GeomAPI_Interpolate aPointToBSpline(finalPointsArray, false, 1e-6);
        aPointToBSpline.Perform();
        if (!aPointToBSpline.IsDone())
        {
            return false;
        }

        Handle(Geom_BSplineCurve) SPL = aPointToBSpline.Curve();

        BRepBuilderAPI_MakeEdge edgeBuilder(SPL);
        edgeBuilder.Build();
        if (!edgeBuilder.IsDone())
        {
            return false;
        }

        setShape(edgeBuilder.Shape());

        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool EdgeShapeBuilder::createMultiTypeSpline(std::vector<std::vector<double>>& points, int type)
{
    try
    {
        if (points.size() <= 1)
            return false;
        else if (points.size() == 2)
            return createPolyLine(points);

        TopoDS_Edge edge;

        int num = points.size();
        if (num < 2)
        {
            return false;
        }

        TColgp_Array1OfPnt pointsArray(1,num);

        for (int i = 1; i <= num; i++)
        {
            pointsArray.SetValue(i, gp_Pnt(points[i - 1][0], points[i - 1][1], points[i - 1][2]));
        }

        if (type == 1)
        {
            /// 均匀B样条，节点向量中的节点值成等差排布
            /// 均匀B样条的基函数呈周期性，即所有的基函数有相同的形状
            /// 每个后续基函数仅仅市前面基函数在新位置上的重复

            Standard_Integer degree(2);
            Standard_Integer PNum = num;
            Standard_Integer KNum = PNum + degree + 1;
            TColStd_Array1OfReal knots(1, KNum);
            for (int i = 0; i < KNum; ++i)
            {
                knots.SetValue(i + 1, i);
            }
            TColStd_Array1OfInteger mults(1, KNum);
            for (int i = 0; i < KNum; ++i)
            {
                mults.SetValue(i + 1, 1);
                std::cout << mults.Value(i + 1) << std::endl;
            }
            Handle(Geom_BSplineCurve) curve = new Geom_BSplineCurve(pointsArray, knots, mults, degree);
            edge = BRepBuilderAPI_MakeEdge(curve);
            
        }
        else if (type == 2)
        {
            /// 准均匀B样条，节点向量中的节点值也是等差排布，但是起点和终点都有k-1的重复度，其中ke为曲线次数。

            Standard_Integer degree(2);
            Standard_Integer PNum = num;
            Standard_Integer KNum = PNum - 1;
            TColStd_Array1OfReal knots(1, KNum);
            for (int i = 0; i < KNum; ++i)
            {
                knots.SetValue(i + 1, i);
            }
            TColStd_Array1OfInteger mults(1, KNum);
            for (int i = 0; i < KNum; ++i)
            {
                if (i == 0 || i == KNum - 1)
                {
                    mults.SetValue(i + 1, degree + 1);
                }
                else
                {
                    mults.SetValue(i + 1, 1);
                }
            }
            Handle(Geom_BSplineCurve) curve = new Geom_BSplineCurve(pointsArray, knots, mults, degree);
            edge = BRepBuilderAPI_MakeEdge(curve);
            
        }
        else if (type == 3)
        {
            /// 分段Bezier曲线
            Standard_Integer degree(2);
            Standard_Integer PNum = num;
            Standard_Integer KNum = PNum - 4;
            TColStd_Array1OfReal knots(1, KNum);
            for (int i = 0; i < KNum; ++i)
            {
                knots.SetValue(i + 1, i);
            }
            TColStd_Array1OfInteger mults(1, KNum);
            for (int i = 0; i < KNum; ++i)
            {
                if (i == 0 || i == KNum - 1)
                {
                    mults.SetValue(i + 1, degree + 1);
                }
                else
                {
                    mults.SetValue(i + 1, degree);
                }
            }
            Handle(Geom_BSplineCurve) curve = new Geom_BSplineCurve(pointsArray, knots, mults, degree);
            edge = BRepBuilderAPI_MakeEdge(curve);
            
        }
        /*else if (type == 4)
        {
            /// 有理B样条曲线
            Standard_Integer degree(2);
            Standard_Integer PNum = 9;
            Standard_Integer KNum = PNum - 1;
            TColStd_Array1OfReal knots(1, KNum);
            for (int i = 0; i < KNum; ++i)
            {
                knots.SetValue(i + 1, i);
            }
            TColStd_Array1OfInteger mults(1, KNum);
            for (int i = 0; i < KNum; ++i)
            {
                if (i == 0 || i == KNum - 1)
                {
                    mults.SetValue(i + 1, degree + 1);
                }
                else
                {
                    mults.SetValue(i + 1, 1);
                }
            }
            Handle(Geom_BSplineCurve) curve = new Geom_BSplineCurve(pointsArray, pointsArrayWeight, knots, mults, degree);
            edge = BRepBuilderAPI_MakeEdge(curve);
            
        }*/

        if (edge.IsNull())
        {
            return false;
        }

        setShape(edge);

        return true;
    }
    catch (...)
    {
        return false;
    }
}

// bool EdgeShapeBuilder::createInterpolateBSpline(std::vector<std::vector<double>>& points)
// {
//     try
//     {
//         int num = points.size();
//         if (num < 1)
//         {
//             return false;
//         }
// 
//         Handle(TColgp_HArray1OfPnt) pointsArray = new TColgp_HArray1OfPnt(1, num);
//         for (int i = 1; i <= num; i++)
//         {
//             pointsArray->SetValue(i, gp_Pnt(points[i - 1][0], points[i - 1][1], points[i - 1][2]));
//         }
// 
//         GeomAPI_Interpolate aPointToBSpline(pointsArray, false, 10e-6);
//         aPointToBSpline.Perform();
//         if (!aPointToBSpline.IsDone())
//         {
//             return false;
//         }
//         Handle(Geom_BSplineCurve) SPL = aPointToBSpline.Curve();
// 
//         BRepBuilderAPI_MakeEdge edgeBuilder(SPL);
//         edgeBuilder.Build();
//         if (!edgeBuilder.IsDone())
//         {
//             return false;
//         }
// 
//         setShape(edgeBuilder.Shape());
// 
//         return true;
//     }
//     catch (...)
//     {
//         return false;
//     }
// }

bool EdgeShapeBuilder::createHyperbola(std::vector<double>& centerP, const double& majorR, const double& minorR, const double& startP,
    const double& endP, const double& xAxis, const double& yAxis, const double& zAxis)
{
    try
    {
        if (centerP.size() != 3)
        {
            return false;
        }

        gp_Pnt pCenter(centerP[0], centerP[1], centerP[2]);
        gp_Dir dir(zAxis, xAxis, yAxis);
        gp_Ax2 axes(pCenter, dir);

        gp_Hypr hypr(axes, majorR, minorR);
        BRepBuilderAPI_MakeEdge edgeBuilder(hypr, startP, endP);
        edgeBuilder.Build();
        if (!edgeBuilder.IsDone())
        {
            return false;
        }

        setShape(edgeBuilder.Shape());

        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool EdgeShapeBuilder::createParabola(std::vector<double>& centerP, const double& focalDistance, const double& startP, const double& endP,
    const double& xAxis, const double& yAxis, const double& zAxis)
{
    try
    {
        if (centerP.size() != 3)
        {
            return false;

        }

        gp_Pnt pCenter(centerP[0], centerP[1], centerP[2]);
        gp_Dir dir(zAxis, xAxis, yAxis);
        gp_Ax2 axes(pCenter, dir);

        gp_Parab parab(axes, focalDistance);
        BRepBuilderAPI_MakeEdge edgeBuilder(parab, startP, endP);
        edgeBuilder.Build();
        if (!edgeBuilder.IsDone())
        {
            return false;
        }

        setShape(edgeBuilder.Shape());

        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool EdgeShapeBuilder::createHelix(
    gp_Pnt startpnt, const double& globalXcenter, const double& globalYcenter, const double& globalZcenter,
    const double& lowerRadius, const double& upperRadius, const double& height, const double& turns,
    const double& rightHanded, const double& globalXAxis, const double& globalYAxis, const double& globalZAxis)
{
    try
    {
        gp_Pnt centerPnt(globalXcenter, globalYcenter, globalZcenter);
        //通用设置
        double maxRadius = upperRadius > lowerRadius ? upperRadius : lowerRadius;
        gp_Pnt center(globalXcenter, globalYcenter, globalZcenter);
        gp_Dir dir(globalXAxis, globalYAxis, globalZAxis);
        gp_Ax2 axes = gp_Ax2(center, dir);
        double angle = atan(abs(upperRadius - lowerRadius) / abs(height));
//         double angle = atan(abs(height) / abs(turns *2.0 * M_PI));
        double slope = cos(angle);

        //造线
        double rightHandedFactor = rightHanded == 1 ? 1 : -1;
        double endX = rightHandedFactor* turns * 2.0 * M_PI;
        double endY = height / slope;
        double endU = sqrt(pow(endX, 2) + pow(endY, 2));
        gp_Lin2d aLine2d= gp_Lin2d(gp_Pnt2d(0.0, 0.0), gp_Dir2d(endX, endY));
        Handle_Geom2d_TrimmedCurve aSegment = GCE2d_MakeSegment(aLine2d, 0.0, M_PI * 2.0).Value();

        //造面
        BRepBuilderAPI_MakeEdge helixBuilder;
        if (lowerRadius == upperRadius)
        {
            if (height < 0)
            {
                axes= gp_Ax2(center, -dir);
            }
            Handle_Geom_CylindricalSurface aCylinder = new Geom_CylindricalSurface(axes, lowerRadius);
            gp_Pnt startpt = aCylinder->Value(0, 0);
            helixBuilder = BRepBuilderAPI_MakeEdge(aSegment, aCylinder, 0.0, endU);
        }
        else
        {
            double angleFactor = height > 0 ? (lowerRadius > upperRadius ? -1 : 1) : (lowerRadius > upperRadius ? 1 : -1);
            Handle_Geom_ConicalSurface aCone = new Geom_ConicalSurface(axes, angleFactor*angle, lowerRadius);
            helixBuilder = BRepBuilderAPI_MakeEdge(aSegment, aCone, 0.0, endU);
        }

        helixBuilder.Build();

        if (!helixBuilder.IsDone())
        {
            return false;
        }
        TopoDS_Shape edge = helixBuilder.Shape(); 

        double StartP, EndP;
        Handle(Geom_Curve) helixCurve = BRep_Tool::Curve(TopoDS::Edge(edge), StartP, EndP);

        BRepLib::BuildCurves3d(edge, 1e-5, GeomAbs_C1, 14, turns * 2);

        //旋转需求
        if (startpnt.X() != -99999.01)
        {
            BRepAdaptor_Curve bpc(TopoDS::Edge(edge));
            gp_Pnt finalstartpnt = bpc.Value(bpc.FirstParameter());

            gp_Vec vec1(centerPnt, startpnt);
            gp_Vec vec2(centerPnt, finalstartpnt);
            double rotateangle = -vec1.AngleWithRef(vec2, dir);

            TransformShapeBuilder tsBuilder;
            if (!tsBuilder.shapeRotate(edge, globalXcenter, globalYcenter, globalZcenter, globalXAxis,
                globalYAxis, globalZAxis, rotateangle * 180 / M_PI, false))
            {
                return false;
            }
            TopoDS_Shape result = tsBuilder.getShape();
            setShape(tsBuilder.getShape());
            return true;
        }

        setShape(edge);

        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool EdgeShapeBuilder::createHelix(const double& centerX, const double& centerY, const double& centerZ, const double& startX, const double& startY, const double& startZ, const double& upperRadius, const double& height, const double& turns, const double& rightHanded, const double& xAxis, const double& yAxis, const double& zAxis)
{
	try
	{
		//通用设置
		gp_Pnt centerPnt(centerX, centerY, centerZ);
		gp_Pnt startPnt(startX, startY, startZ);
		double lowerRadius = centerPnt.Distance(startPnt);
		double maxRadius = upperRadius > lowerRadius ? upperRadius : lowerRadius;
		gp_Dir dir(xAxis, yAxis, zAxis);
		gp_Ax2 axes = gp_Ax2(centerPnt, dir);
		double angle = atan(abs(upperRadius - lowerRadius) / abs(height));
		double slope = cos(angle);

		//造线
		double rightHandedFactor = rightHanded == 1 ? -1 : 1;
		double endX = rightHandedFactor * turns * 2.0 * M_PI;
		double endY = height / slope;
		double endU = sqrt(pow(endX, 2) + pow(endY, 2));
		gp_Lin2d aLine2d = gp_Lin2d(gp_Pnt2d(0.0, 0.0), gp_Dir2d(endX, endY));
		Handle_Geom2d_TrimmedCurve aSegment = GCE2d_MakeSegment(aLine2d, 0.0, M_PI * 2.0).Value();

		//造面
		BRepBuilderAPI_MakeEdge helixBuilder;
		if (lowerRadius == upperRadius)
		{
			if (height < 0)
			{
				axes = gp_Ax2(centerPnt, -dir);
			}
			Handle_Geom_CylindricalSurface aCylinder = new Geom_CylindricalSurface(axes, lowerRadius);
			helixBuilder = BRepBuilderAPI_MakeEdge(aSegment, aCylinder, 0.0, endU);
		}
		else
		{
			double angleFactor = height > 0 ? (lowerRadius > upperRadius ? -1 : 1) : (lowerRadius > upperRadius ? 1 : -1);
			Handle_Geom_ConicalSurface aCone = new Geom_ConicalSurface(axes, angleFactor*angle, lowerRadius);
			helixBuilder = BRepBuilderAPI_MakeEdge(aSegment, aCone, 0.0, endU);
		}

		helixBuilder.Build();

		if (!helixBuilder.IsDone())
		{
			return false;
		}
		TopoDS_Shape edge = helixBuilder.Shape();

		double StartP, EndP;
		Handle(Geom_Curve) helixCurve = BRep_Tool::Curve(TopoDS::Edge(edge), StartP, EndP);

		BRepLib::BuildCurves3d(edge, 1e-5, GeomAbs_C1, 14, turns * 2);

		helixCurve = BRep_Tool::Curve(TopoDS::Edge(edge), StartP, EndP);

		//rotate 
		gp_Pnt finalStartPnt;
		helixCurve->D0(StartP, finalStartPnt);

		gp_Vec vec1(centerPnt, startPnt);
		gp_Vec vec2(centerPnt, finalStartPnt);
		double rotateAngle = vec1.AngleWithRef(vec2, dir);

		TransformShapeBuilder tsBuilder;
		if (!tsBuilder.shapeRotate(edge, centerX, centerY, centerZ, xAxis, yAxis, zAxis, rotateAngle*180.0 / M_PI, false))
		{
			return false;
		}


// 		helixCurve = BRep_Tool::Curve(TopoDS::Edge(edge), StartP, EndP);
// 		gp_Pnt StartPos;
// 		gp_Vec StartDir;
// 		helixCurve->D1(EndP, StartPos, StartDir);


		setShape(tsBuilder.getShape());

		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool EdgeShapeBuilder::createSpiral(
    const double& globalXcenter, const double& globalYcenter, const double& globalZcenter,
    const double& minRadius, const double& maxRadius, const double& turns, const double& rightHanded,
    const double& globalXAxis, const double& globalYAxis, const double& globalZAxis)
{
    try
    {
        gp_Pnt center(globalXcenter, globalYcenter, globalZcenter);
        gp_Dir dir(globalXAxis, globalYAxis, globalZAxis);
        gp_Ax2 axes(center, dir);

        double height = maxRadius > minRadius ? maxRadius : minRadius;
        createHelix(globalXcenter, globalYcenter, globalZcenter,
            minRadius, maxRadius, height, turns, rightHanded,
            globalXAxis, globalYAxis, globalZAxis);

        TopoDS_Edge heilxEdge = TopoDS::Edge(getShape());
        //BRepLib::BuildCurve3d(heilxEdge);
//         ShapeUtilities::shapeCover(heilxEdge, heilxEdge);
//         setShape(heilxEdge);
// 
//         return true;


        gp_Circ circle(axes, 2 * std::max(maxRadius, minRadius));
        BRepBuilderAPI_MakeEdge edgeBuilder(circle);
        edgeBuilder.Build();
        if (!edgeBuilder.IsDone())
        {
            return false;
        }
        TopoDS_Edge edge = edgeBuilder.Edge();

        BRepBuilderAPI_MakeWire wireBuilder(edge);
        wireBuilder.Build();
        if (!wireBuilder.IsDone())
        {
            return false;
        }
        TopoDS_Wire wire = wireBuilder.Wire();

        BRepBuilderAPI_MakeFace faceBuilder(wire);
        faceBuilder.Build();

        BRepProj_Projection Project(heilxEdge, faceBuilder.Shape(), dir);

        if (!Project.IsDone())
        {
            return false;
        }

        setShape(Project.Shape());

        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool EdgeShapeBuilder::createSpiral(const double& globalXcenter, const double& globalYcenter, const double& globalZcenter, const double& startX, const double& startY, const double& startZ, const double& maxRadius, const double& turns, const double& rightHanded, const double& globalXAxis, const double& globalYAxis, const double& globalZAxis)
{
	try
	{
		gp_Pnt center(globalXcenter, globalYcenter, globalZcenter);
		gp_Dir dir(globalXAxis, globalYAxis, globalZAxis);
		gp_Ax2 axes(center, dir);

		gp_Pnt startPnt(startX, startY, startZ);
		double minRadius = center.Distance(startPnt);

		double height = maxRadius > minRadius ? maxRadius : minRadius;
		createHelix(globalXcenter, globalYcenter, globalZcenter,
			minRadius, maxRadius, height, turns, rightHanded,
			globalXAxis, globalYAxis, globalZAxis);

		TopoDS_Edge heilxEdge = TopoDS::Edge(getShape());
		//BRepLib::BuildCurve3d(heilxEdge);
//         ShapeUtilities::shapeCover(heilxEdge, heilxEdge);
//         setShape(heilxEdge);
// 
//         return true;


		gp_Circ circle(axes, 2 * std::max(maxRadius, minRadius));
		BRepBuilderAPI_MakeEdge edgeBuilder(circle);
		edgeBuilder.Build();
		if (!edgeBuilder.IsDone())
		{
			return false;
		}
		TopoDS_Edge edge = edgeBuilder.Edge();

		BRepBuilderAPI_MakeWire wireBuilder(edge);
		wireBuilder.Build();
		if (!wireBuilder.IsDone())
		{
			return false;
		}
		TopoDS_Wire wire = wireBuilder.Wire();

		BRepBuilderAPI_MakeFace faceBuilder(wire);
		faceBuilder.Build();

		BRepProj_Projection Project(heilxEdge, faceBuilder.Shape(), dir);

		if (!Project.IsDone())
		{
			return false;
		}

		setShape(Project.Shape());

		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool EdgeShapeBuilder::createFuncCurve(const std::string xFuncExpress, const std::string yFuncExpress, const std::string zFuncExpress, const std::string varName, const double startU, const double endU)
{
    try
    {
        Handle(Geom_BoundedMathCurve) curve = new Geom_BoundedMathCurve(xFuncExpress, yFuncExpress, zFuncExpress, varName, startU, endU);
// 		Handle(Geom_BSplineCurve) curve2 = GeomConvert::CurveToBSplineCurve(curve);

		GeomConvert_ApproxCurve anApprox(curve, Precision::Confusion(), GeomAbs_C0, 10000, 100);
		if (!anApprox.IsDone())
		{
			return false;
		}

        TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(anApprox.Curve());

        if (edge.IsNull())
        {
            return false;
        }



        setShape(edge);

        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool EdgeShapeBuilder::createEllipseArc(std::vector<double>& centerP, std::vector<double>& axis, const double& majorR,
    const double& minorR, double startAng, double endAng)
{
    try
    {
        if (centerP.size() != 3)
            return false;

        gp_Pnt pCenter(centerP[0], centerP[1], centerP[2]);
        gp_Dir dir(axis[0], axis[1], axis[2]);
        gp_Ax2 axes(pCenter, dir);

        gp_Elips ellipse(axes, majorR, minorR);
        Handle(Geom_TrimmedCurve) anarc = GC_MakeArcOfEllipse(ellipse, startAng, endAng, true);
        TopoDS_Edge Arc = BRepBuilderAPI_MakeEdge(anarc);

        setShape(Arc);
        return true;
    }
    catch (...)
    {
        return false;
    }
}