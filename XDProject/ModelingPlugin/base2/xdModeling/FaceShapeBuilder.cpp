#include "FaceShapeBuilder.h"
#include "EdgeShapeBuilder.h"
#include "SolidShapeBuilder.h"
#include "OCCBasicTools.h"
#include "ShapeUtilities.h"
#include "Geom_BoundedMathSurface.hxx"
#include "SweepShapeBuilder.h"

#include <TopoDS_Face.hxx>
#include <gp_Ax3.hxx>
#include <gp_Pln.hxx>
#include <gp_Circ.hxx>
#include <gp_Elips.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Face.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <GC_MakeArcOfCircle.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRepBuilderAPI_MakeShell.hxx>
#include <BRep_Builder.hxx>
#include <GeomConvert_ApproxSurface.hxx>
#include <Precision.hxx>
#include <Geom_BSplineSurface.hxx>
#include <Geom_SphericalSurface.hxx>
#include <GeomConvert.hxx>
#include <Geom_BSplineSurface.hxx>
#include <GeomAPI_PointsToBSplineSurface.hxx>


FaceShapeBuilder::FaceShapeBuilder()
{
}


FaceShapeBuilder::~FaceShapeBuilder()
{
}

//need to check
bool FaceShapeBuilder::createRectFace(const double& xPos, const double& yPos, const double& zPos, const double& xAxis, const double& yAxis, const double&zAxis, const double& width, const double& height)
{
    try
    {
        //plane
        gp_Pln plane(gp_Pnt(xPos, yPos, zPos), gp_Dir(xAxis, yAxis, zAxis));
        BRepBuilderAPI_MakeFace faceBuilder(plane, 0, width, 0, height);
        faceBuilder.Build();
        if (!faceBuilder.IsDone())
        {
            return false;
        }

        setShape(faceBuilder.Shape());

        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool FaceShapeBuilder::createCircleFace(const double& xCenter, const double& yCenter, const double& zCenter, const double& xAxis, const double& yAxis, const double&zAxis, const double& radius)
{
    try
    {
        //circle
        gp_Pnt pnt(xCenter, yCenter, zCenter);
        gp_Dir dir(xAxis, yAxis, zAxis);
        gp_Ax2 axes(pnt, dir);

        gp_Circ circle(axes, radius);
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
        if (!faceBuilder.IsDone())
        {
            return false;
        }

        setShape(faceBuilder.Shape());

        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool FaceShapeBuilder::createSectorFace(
	const double& xCenter, const double& yCenter, const double& zCenter,
	const double& xAxis, const double& yAxis, const double&zAxis,
	const double& radius, const double& startAng, const double& endAng)
{
	try
	{
		//circle
		gp_Pnt center(xCenter, yCenter, zCenter);
		gp_Dir dir(xAxis, yAxis, zAxis);
		gp_Ax2 axes(center, dir);

		gp_Circ Circ(axes, radius);

		gp_Pnt startPnt = center.Translated(gp_Vec(axes.XDirection()) * radius).Rotated(gp_Ax1(center, dir), startAng * M_PI / 180);
		gp_Pnt endPnt = center.Translated(gp_Vec(axes.XDirection()) * radius).Rotated(gp_Ax1(center, dir), endAng * M_PI / 180);


		Handle(Geom_TrimmedCurve) AnArc = GC_MakeArcOfCircle(Circ, startPnt, endPnt, Standard_True);
		TopoDS_Edge Arc = BRepBuilderAPI_MakeEdge(AnArc);
		TopoDS_Edge StartLine = BRepBuilderAPI_MakeEdge(center, startPnt);
		TopoDS_Edge EndLine = BRepBuilderAPI_MakeEdge(center, endPnt);


		BRepBuilderAPI_MakeWire wireBuilder;
		wireBuilder.Add(Arc);
		wireBuilder.Add(StartLine);
		wireBuilder.Add(EndLine);
		wireBuilder.Build();

		if (!wireBuilder.IsDone())
		{
			return false;
		}
		TopoDS_Wire wire = wireBuilder.Wire();

		BRepBuilderAPI_MakeFace faceBuilder(wire);
		faceBuilder.Build();
		if (!faceBuilder.IsDone())
		{
			return false;
		}

		setShape(faceBuilder.Shape());

		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool FaceShapeBuilder::createEllipseFace(const double& xCenter, const double& yCenter, const double& zCenter, const double& xAxis, const double& yAxis, const double&zAxis, const double& majorRadius, const double& minorRadius)
{
    try
    {
        //circle
        gp_Pnt pnt(xCenter, yCenter, zCenter);
        gp_Dir dir(xAxis, yAxis, zAxis);
        gp_Ax2 axes(pnt, dir);


		BRepBuilderAPI_MakeEdge edgeBuilder;
		if (majorRadius > minorRadius)
		{
			gp_Elips ellipse(axes, majorRadius, minorRadius);
			edgeBuilder = BRepBuilderAPI_MakeEdge(ellipse);
		}
		else if (majorRadius < minorRadius)
		{
			gp_Elips ellipse(axes, minorRadius, majorRadius);
			edgeBuilder = BRepBuilderAPI_MakeEdge(ellipse);
		}
		else
		{
			gp_Circ circle(axes, majorRadius);
			edgeBuilder = BRepBuilderAPI_MakeEdge(circle);
		}

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
        if (!faceBuilder.IsDone())
        {
            return false;
        }

		gp_Trsf rotate;
		if (majorRadius < minorRadius)
		{
			rotate.SetRotation(gp_Ax1(pnt, dir), M_PI_2);
		}
		setShape(BRepBuilderAPI_Transform(faceBuilder.Shape(), rotate));

        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool FaceShapeBuilder::createCircleFace(const double& xCenter, const double& yCenter, const double& zCenter,
	const double& xAxis, const double& yAxis, const double& zAxis, const double& radius)
{
	try
	{
		gp_Pnt pnt(xCenter, yCenter, zCenter);
		gp_Dir dir(xAxis, yAxis, zAxis);
		gp_Ax2 axes(pnt, dir);

		gp_Circ circle(axes, radius);
		BRepBuilderAPI_MakeEdge edgeBuilder(circle);
		edgeBuilder.Build();
		if (!edgeBuilder.IsDone())
		{
			return false;
		}
		TopoDS_Edge edge = edgeBuilder.Edge();

		BRepBuilderAPI_MakeWire wireBuilder(edge);
		wireBuilder.Build();
		if(!wireBuilder.IsDone())
			return false;
		TopoDS_Wire wire = wireBuilder.Wire();

		BRepBuilderAPI_MakeFace faceBuilder(wire);
		faceBuilder.Build();
		if (!faceBuilder.IsDone())
			return false;
		TopoDS_Face face = faceBuilder.Face();

		setShape(face);
		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool FaceShapeBuilder::createCylinderFace(const double& xBottomCenter, const double& yBottomCenter, const double& zBottomCenter, const double& xAxis, const double& yAxis, const double& zAxis, const double& raidus, const double& height)
{
    try
    {
        SolidShapeBuilder solidBuilder;
        if (!solidBuilder.createCylinder(xBottomCenter, yBottomCenter, zBottomCenter, xAxis, yAxis, zAxis, raidus, height))
        {
            return false;
        }

        TopoDS_Shape targetShape;
        for (TopExp_Explorer exp(solidBuilder.getShape(), TopAbs_FACE); exp.More(); exp.Next())
        {
            TopoDS_Face face = TopoDS::Face(exp.Current());
            BRepAdaptor_Surface brepAdaptorSurface(face, Standard_True);
            GeomAbs_SurfaceType faceType = brepAdaptorSurface.GetType();
            if (faceType == GeomAbs_Cylinder)
            {
                targetShape = face;
                break;
            }
        }

        if (targetShape.IsNull())
        {
            return false;
        }
        
        setShape(targetShape);

        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool FaceShapeBuilder::createConeFace(const double& botCenterX, const double& botCenterY, const double& botCenterZ, const double& lowerRadius, const double& upperRadius, const double& height, const double& xAxis, const double& yAxis, const double& zAxis)
{
    try
    {
        try
        {
            SolidShapeBuilder solidBuilder;
            if (!solidBuilder.createCone(botCenterX, botCenterY, botCenterZ, lowerRadius, upperRadius, height, xAxis, yAxis, zAxis))
            {
                return false;
            }

            TopoDS_Shape targetShape;
            for (TopExp_Explorer exp(solidBuilder.getShape(), TopAbs_FACE); exp.More(); exp.Next())
            {
                TopoDS_Face face = TopoDS::Face(exp.Current());
                BRepAdaptor_Surface brepAdaptorSurface(face, Standard_True);
                GeomAbs_SurfaceType faceType = brepAdaptorSurface.GetType();
                if (faceType == GeomAbs_Cone || faceType == GeomAbs_Cylinder)
                {
                    targetShape = face;
                    break;
                }
            }

            if (targetShape.IsNull())
            {
                return false;
            }

            setShape(targetShape);

            return true;
        }
        catch (...)
        {
            return false;
        }
    }
    catch (...)
    {
        return false;
    }
}

bool FaceShapeBuilder::createPolygon(const std::vector<std::vector<double>>& pPos)
{
	try
	{
		if (pPos.size() < 3)
		{
			return false;
		}

		std::vector<gp_Pnt> pntlist;
		for (int i = 0; i < pPos.size(); ++i)
		{
			pntlist.push_back(gp_Pnt(pPos[i][0], pPos[i][1], pPos[i][2]));
		}
		TopoDS_Face face = OCCBasicTools::GetFaceFromPntList(pntlist);

		if (!face.IsNull())
		{
			return false;
		}


		setShape(face);

		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool FaceShapeBuilder::createRegularPolygon(
	const double& xBotCenter, const double& yBotCenter, const double& zBotCenter, 
	const double& xBotStart, const double& yBotStart, const double& zBotStart,
	const int& numSides, double XAxis, double YAxis, double ZAxis)
{
	try
	{
		//create RegularPolyhedron
		gp_Pnt center(xBotCenter, yBotCenter, zBotCenter);
		gp_Pnt start(xBotStart, yBotStart, zBotStart);
		gp_Dir dir(XAxis, YAxis, ZAxis);

		//if (gp_Dir(gp_Vec(center, start)).Dot(dir) != 0)
		//{
		//	return false;
		//}

		if (numSides < 3)
		{
			return false;
		}

		std::vector<gp_Pnt> PntList;
		gp_Trsf rotate;
		for (int i = 0; i < numSides; ++i)
		{

			PntList.push_back(start.Rotated(gp_Ax1(center, dir), 2 * i * M_PI / numSides));
		}

		TopoDS_Face Face = OCCBasicTools::GetFaceFromPntList(PntList);

		if (Face.IsNull())
		{
			return false;
		}

		setShape(Face);

		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool FaceShapeBuilder::createParabolaFace(std::vector<double>& centerP, const double& focalDistance, const double& startP, const double& endP, const double& xAxis, const double& yAxis, const double& zAxis)
{
	try
	{
		double finalEndP = endP; 
		double finalStartP = startP;
		if (startP*endP < 0)
		{
			if (abs(endP) <= abs(startP))
			{
				finalEndP = 0.;
			}
			else
			{
				finalStartP = 0;
			}
		}

		EdgeShapeBuilder esBuilder;
		if (!esBuilder.createParabola(centerP, focalDistance, finalStartP, finalEndP, xAxis, yAxis, zAxis))
		{
			return false;
		}
		TopoDS_Shape parabola = esBuilder.getShape();

		//rotate
		SweepShapeBuilder ssBuilder;
		if (!ssBuilder.revolOperation(parabola, centerP[0], centerP[1], centerP[2],  xAxis, yAxis, zAxis, 360.))
		{
			return false;
		}
		m_shape = ssBuilder.getShape();

		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool FaceShapeBuilder::createFuncSurface(const std::string xFuncExpress, const std::string yFuncExpress, const std::string zFuncExpress, const std::string varUName, const std::string varVName, const double startU, const double endU, const double startV, const double endV)
{
	try
	{
		Handle(Geom_BoundedMathSurface) surface = new Geom_BoundedMathSurface(xFuncExpress, yFuncExpress, zFuncExpress, varUName, varVName, startU, endU, startV, endV);
// 		Handle(Geom_SphericalSurface) surface = new Geom_SphericalSurface(gp_Ax3(gp_Pnt(0,0,0),gp_Dir(0,0,1)), 5);

		Handle(Geom_BSplineSurface) surfa2 = GeomConvert::SurfaceToBSplineSurface(surface);
// 		GeomConvert_ApproxSurface anApprox(surface, 0.001, GeomAbs_C0, GeomAbs_C0, 3, 3, 100, 0);
// 		if (!anApprox.IsDone())
// 		{
// 			return false;
// 		}

		BRepBuilderAPI_MakeFace faceBuilder(surfa2, startU, endU, startV, endV, Precision::Confusion());
		faceBuilder.Build();
		if (!faceBuilder.IsDone())
		{
			return false;
		}

		setShape(faceBuilder.Shape());

		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool FaceShapeBuilder::createClosedFace(TopoDS_Wire closedWire)
{
    try
    {
        if (!closedWire.Closed())
        {
            return false;
        }

        BRepBuilderAPI_MakeFace faceBuilder(closedWire);
        faceBuilder.Build();
        if (!faceBuilder.IsDone())
        {
            return false;
        }

        TopoDS_Shape targetShape = faceBuilder.Shape();

        if (targetShape.IsNull())
        {
            return false;
        }

        setShape(targetShape);

        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool FaceShapeBuilder::createNurbsSurface(const TColgp_Array2OfPnt& Poles, const TColStd_Array2OfReal& Weights, const TColStd_Array1OfReal& UKnots, const TColStd_Array1OfReal& VKnots, const TColStd_Array1OfInteger& UMults, const TColStd_Array1OfInteger& VMults, const Standard_Integer UDegree, const Standard_Integer VDegree)
{
	try
	{
		Handle(Geom_BSplineSurface) surfa2 = new Geom_BSplineSurface(Poles, Weights, UKnots, VKnots, UMults, VMults, UDegree, VDegree);

		double startU, endU, startV, endV;
		surfa2->Bounds(startU, endU, startV, endV);

		BRepBuilderAPI_MakeFace faceBuilder(surfa2, startU, endU, startV, endV, Precision::Confusion());
		faceBuilder.Build();
		if (!faceBuilder.IsDone())
		{
			return false;
		}

		TopoDS_Shape targetShape = faceBuilder.Shape();

		if (targetShape.IsNull())
		{
			return false;
		}

		setShape(targetShape);

		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool FaceShapeBuilder::createInterpolateBSplineSurface(TColgp_Array2OfPnt poles)
{
	Handle(Geom_BSplineSurface) surfa2 = GeomAPI_PointsToBSplineSurface(poles);

	double ufst, ulst, vfst, vlst;
	surfa2->Bounds(ufst, ulst, vfst, vlst);

	BRepBuilderAPI_MakeFace mkface(surfa2, ufst, ulst, vfst, vlst, 0.00001);
	mkface.Build();
	if (!mkface.IsDone())
		return false;
	TopoDS_Shape targetshape = mkface.Shape();
	if (targetshape.IsNull())
		return false;
	setShape(targetshape);
	return true;
}