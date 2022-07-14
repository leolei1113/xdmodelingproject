#include "ShapeUtilities.h"

#include <tuple>
#include <gp_Mat.hxx>
#include <gp_Pnt.hxx>
#include <gp_Vec.hxx>
#include <Geom_VectorWithMagnitude.hxx>
#include <TopoDS_Iterator.hxx>
#include <TopExp_Explorer.hxx>
// #include <BRepBuilderAPI_GTransform.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <TopoDS.hxx>
#include <BRepBuilderAPI_MakeSolid.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRep_Tool.hxx>
#include <GeomLProp_SLProps.hxx>
#include <GeomLProp_CLProps.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <BRepLProp_CLProps.hxx>
#include <BRep_Tool.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRepBuilderAPI_FindPlane.hxx>
#include <TopExp.hxx>
#include <ChFi2d_ConstructionError.hxx>
#include <BRepTools_WireExplorer.hxx>
#include <BRepBuilderAPI_MakeShell.hxx>
#include <BRep_Builder.hxx>
#include <GeomLib_IsPlanarSurface.hxx>
#include <GProp_PEquation.hxx>
#include <GeomAPI_ProjectPointOnCurve.hxx>

#include <BRepGProp_Face.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <GeomAbs_SurfaceType.hxx>
#include <Geom_ToroidalSurface.hxx>
#include <Geom_SurfaceOfRevolution.hxx>
#include <Geom_SurfaceOfLinearExtrusion.hxx>
#include <Geom_OffsetSurface.hxx>
#include <ShapeAnalysis_Surface.hxx>
#include <BRep_TEdge.hxx>
#include <BRep_CurveOnClosedSurface.hxx>

#include <Geom2d_Line.hxx>
#include <BRepLib_MakeEdge.hxx>
#include <BRepLib.hxx>
#include <BRepTools.hxx>

ShapeUtilities::ShapeUtilities()
{
}


ShapeUtilities::~ShapeUtilities()
{
}

bool ShapeUtilities::shapeFilter(const TopoDS_Shape& origShape, TopAbs_ShapeEnum targetType, TopoDS_Shape& filterShape)
{
    if (origShape.NbChildren() == 1&& origShape.ShapeType()<targetType)
    {
        TopoDS_Iterator iter(origShape);
        return shapeFilter(iter.Value(), targetType, filterShape);
    }
    else if (origShape.ShapeType() == targetType)
    {
        filterShape = origShape;
        return true;
    }
    else
    {
        return false;
    }

}

bool ShapeUtilities::shapeCover(const TopoDS_Shape& origShape, TopoDS_Shape& coveredShape)
{
    BRep_Builder B;
//     BRepBuilderAPI_MakeShape *builder = nullptr;
    switch (origShape.ShapeType())
    {
    case TopAbs_EDGE:
    {
        BRepBuilderAPI_MakeWire builder(TopoDS::Edge(origShape));
        builder.Build();
        if (!builder.IsDone())
        {
            return false;
        }
        coveredShape = builder.Shape();
        break;
    }
    case TopAbs_WIRE:
    {
        BRepBuilderAPI_MakeFace builder(TopoDS::Wire(origShape));
        builder.Build();
        if (!builder.IsDone())
        {
            return false;
        }
        coveredShape = builder.Shape();
        break;
    }
    case TopAbs_FACE:
    {
        TopoDS_Shell target;
        B.MakeShell(target);
        B.Add(target, origShape);
        coveredShape = target;
        break;
    }
    case TopAbs_SHELL:
    {
        BRepBuilderAPI_MakeSolid builder(TopoDS::Shell(origShape));
        builder.Build();
        if (!builder.IsDone())
        {
            return false;
        }
        coveredShape = builder.Shape();
        break;
    }
    case TopAbs_SOLID:
    {
        TopoDS_Compound target;
        B.MakeCompound(target);
        B.Add(target, origShape);
        coveredShape = target;
        break;
    }
    default:
        return false;
    }

    return true;
}

TopoDS_ListOfShape ShapeUtilities::findEdgeOwnerFaces(const TopoDS_Shape& bdy, const TopoDS_Shape& edge)
{
    TopoDS_ListOfShape faceList;

    TopExp_Explorer faceExp(bdy, TopAbs_FACE);
    for (; faceExp.More(); faceExp.Next())
    {
        TopExp_Explorer edgeExp(edge, TopAbs_EDGE);
        for (; edgeExp.More(); edgeExp.Next())
        {
            if (edgeExp.Value().IsSame(edge))
            {
                faceList.Append(faceExp.Value());
                break;
            }
        }
    }

    return faceList;
}

TopoDS_Shape ShapeUtilities::shapeTransformFromLocalToGlobal(double* localCoor, const TopoDS_Shape& shape, const bool& isCopy)
{
    gp_Trsf trsf;
    trsf.SetValues(localCoor[0], localCoor[4], localCoor[8], localCoor[12], 
        localCoor[1], localCoor[5], localCoor[9], localCoor[13], 
        localCoor[2], localCoor[6], localCoor[10], localCoor[14]);
    BRepBuilderAPI_Transform transformer(trsf);

//     gp_Mat transformMat(
//         localCoor[0], localCoor[4], localCoor[8],
//         localCoor[1], localCoor[5], localCoor[9],
//         localCoor[2], localCoor[6], localCoor[10]);
// 
//     gp_XYZ localOriginPnt(localCoor[12], localCoor[13], localCoor[14]);
// 
//     gp_GTrsf trsf(transformMat, localOriginPnt);
//     BRepBuilderAPI_GTransform transformer(trsf);
    transformer.Perform(shape, isCopy);
    if (transformer.IsDone())
    {
        return transformer.Shape();
    }
    else
    {
        return TopoDS_Shape();
    }
}

TopoDS_Shape ShapeUtilities::shapeTransformFromGlobalToLocal(double* localCoor, const TopoDS_Shape& shape, const bool& isCopy)
{
    gp_Trsf trsf;
    trsf.SetValues(localCoor[0], localCoor[1], localCoor[2], -localCoor[12],
        localCoor[4], localCoor[5], localCoor[6], -localCoor[13],
        localCoor[8], localCoor[9], localCoor[10], -localCoor[14]);
    BRepBuilderAPI_Transform transformer(trsf);

//     gp_Mat transformMat(
//         localCoor[0], localCoor[4], localCoor[8],
//         localCoor[1], localCoor[5], localCoor[9],
//         localCoor[2], localCoor[6], localCoor[10]);
//     transformMat.Transpose();
//     gp_XYZ localOriginPnt(-localCoor[12], -localCoor[13], -localCoor[14]);
// 
//     gp_GTrsf trsf(transformMat, localOriginPnt);
//     BRepBuilderAPI_GTransform transformer(trsf);
    transformer.Perform(shape, isCopy);
    if (transformer.IsDone())
    {
        return transformer.Shape();
    }
    else
    {
        return TopoDS_Shape();
    }
}

void ShapeUtilities::vectorTransformFromLocalToGlobal(double* localCoor, const double& sourceVecX, const double& sourceVecY, const double& sourceVecZ, double& xVec, double& yVec, double& zVec)
{
    double px1, px2, py1, py2, pz1, pz2;
    px1 = localCoor[0] * sourceVecX + localCoor[4] * sourceVecY + localCoor[8] * sourceVecZ + localCoor[12];
    py1 = localCoor[1] * sourceVecX + localCoor[5] * sourceVecY + localCoor[9] * sourceVecZ + localCoor[13];
    pz1 = localCoor[2] * sourceVecX + localCoor[6] * sourceVecY + localCoor[10] * sourceVecZ + localCoor[14];

    px2 = localCoor[12];
    py2 = localCoor[13];
    pz2 = localCoor[14];

    xVec = px1-px2;
    yVec = py1-py2;
    zVec = pz1-pz2;


//     gp_Mat transformMat(
//         localCoor[0], localCoor[4], localCoor[8],
//         localCoor[1], localCoor[5], localCoor[9],
//         localCoor[2], localCoor[6], localCoor[10]);
// 
//     gp_XYZ localOriginPnt(localCoor[12], localCoor[13], localCoor[14]);
// 
//     gp_GTrsf trsf(transformMat, localOriginPnt);
//     BRepBuilderAPI_GTransform transformer(trsf);
//     gp_XYZ newXYS = xyz.Multiplied(transformMat);
// 
//     xVec = newXYS.X();
//     yVec = newXYS.Y();
//     zVec = newXYS.Z();
}

void ShapeUtilities::pntTransformFromLocalToGlobal(double* localCoor,
    const double& sourceX, const double& sourceY, const double& sourceZ, 
    double& x, double& y, double& z)
{
    x = localCoor[0] * sourceX + localCoor[4] * sourceY + localCoor[8] * sourceZ + localCoor[12];
    y = localCoor[1] * sourceX + localCoor[5] * sourceY + localCoor[9] * sourceZ + localCoor[13];
    z = localCoor[2] * sourceX + localCoor[6] * sourceY + localCoor[10] * sourceZ + localCoor[14];

//     if (sourceX == 0 && sourceY == 0 && sourceZ == 0)
//     {
//         x = localCoor[12];
//         y = localCoor[13];
//         z = localCoor[14];
//         return;
//     }
//     gp_XYZ xyz(sourceX, sourceY, sourceZ);
//     gp_Mat transformMat(
//         localCoor[0], localCoor[4], localCoor[8],
//         localCoor[1], localCoor[5], localCoor[9],
//         localCoor[2], localCoor[6], localCoor[10]);
// 
//     gp_XYZ localOriginPnt(localCoor[12], localCoor[13], localCoor[14]);
// 
//     gp_GTrsf trsf(transformMat, localOriginPnt);
//     BRepBuilderAPI_GTransform transformer(trsf);
//     gp_XYZ newXYS = xyz.Multiplied(transformMat);
// 
//     x = newXYS.X();
//     y = newXYS.Y();
//     z = newXYS.Z();
}

// void ShapeUtilities::vectorTransformFromLocalToGlobal(double* localCoor, const double& sourceX, const double& sourceY, const double& sourceZ, double& x, double& y, double& z)
// {
//     gp_XYZ sourceXYZ(sourceX, sourceY, sourceZ);
// //     gp_Mat transformMat(
// //         localCoor[0], localCoor[1], localCoor[2],
// //         localCoor[4], localCoor[5], localCoor[6],
// //         localCoor[8], localCoor[9], localCoor[10]);
//     gp_Mat transformMat(
//         localCoor[0], localCoor[4], localCoor[8],
//         localCoor[1], localCoor[5], localCoor[9],
//         localCoor[2], localCoor[6], localCoor[10]);
//     gp_XYZ localOriginPnt(localCoor[12], localCoor[13], localCoor[14]);
// 
//     gp_Pnt targetPnt = transformMat*sourceXYZ + localOriginPnt;
//     x = targetPnt.X() - localCoor[12];
//     y = targetPnt.Y() - localCoor[13];
//     z = targetPnt.Z() - localCoor[14];
// }
// 
// void ShapeUtilities::pntsTransformFromLocalToGlobal(double* localCoor, const std::vector<std::vector<double>>& points, std::vector<std::vector<double>>& globalPoints)
// {
//     double x, y, z;
//     for (int i = 0; i < points.size(); ++i)
//     {
//         pntTransformFromLocalToGlobal(localCoor, points[i][0], points[i][1], points[i][2], x, y, z);
//         std::vector<double> pnt;
//         pnt.push_back(x);
//         pnt.push_back(y);
//         pnt.push_back(z);
// 
//         globalPoints.push_back(pnt);
//     }
// }
// 
// void ShapeUtilities::lengthTransformFromLocalToGlobalAccordingToVector(double* localCoor, const double& sourceLength, gp_Vec vec, double& length)
// {
//     vec.Normalize();
//     vec.Multiply(sourceLength);
//     gp_XYZ sourceXYZ = vec.XYZ();
//     gp_Mat transformMat(
//         localCoor[0], localCoor[1], localCoor[2],
//         localCoor[4], localCoor[5], localCoor[6],
//         localCoor[8], localCoor[9], localCoor[10]);
//     gp_XYZ localOriginPnt(localCoor[12], localCoor[13], localCoor[14]);
// 
//     gp_Vec targetVec = transformMat*sourceXYZ + localOriginPnt;
//     Geom_VectorWithMagnitude targetVecWithMag(targetVec- localOriginPnt);
//     length = (sourceLength<0?-1.:1.)*targetVecWithMag.Magnitude();
// }
// 
// void ShapeUtilities::lengthTransformFromLocalToGlobalAccordingToNormal(double* localCoor, const double& sourceLength, gp_Vec normal, double& length)
// {
//     //find perpendicular vector
//     gp_Vec finalVec;
//     gp_Vec xAxis(1, 0, 0);
//     if (normal.IsParallel(xAxis,10e-4))
//     {
//         gp_Vec yAxis(0, 1, 0);
//         finalVec = normal.Crossed(yAxis);
//     }
//     else
//     {
//         finalVec = normal.Crossed(xAxis);
//     }
// 
//     //transformed
//     lengthTransformFromLocalToGlobalAccordingToVector(localCoor, sourceLength, finalVec, length);
// }

void ShapeUtilities::getAxis(const DATUMAXISTYPE& axis, double& xAxis, double& yAxis, double& zAxis, double isanti)
{
    xAxis = yAxis = zAxis = 0;
    switch (axis)
    {
    case X:
    {
        xAxis = isanti < 0 ? -1 : 1;
        break;
    }
    case Y:
    {
        yAxis = isanti < 0 ? -1 : 1;
        break;
    }
    case Z:
    {
        zAxis = isanti < 0 ? -1 : 1;
        break;
    }
    default:
        break;
    }
}

void ShapeUtilities::getRadius(const std::vector<std::vector<double>>& points, double& xCenter, double& yCenter, double& zCenter, double& radius)
{
    double x1, y1, z1, x2, y2, z2, x3, y3, z3;
    x1 = points[0][0];
    y1 = points[0][1];
    z1 = points[0][2];
    x2 = points[1][0];
    y2 = points[1][1];
    z2 = points[1][2];
    x3 = points[2][0];
    y3 = points[2][1];
    z3 = points[2][2];

    double a1, b1, c1, d1;
    double a2, b2, c2, d2;
    double a3, b3, c3, d3;

    a1 = (y1*z2 - y2*z1 - y1*z3 + y3*z1 + y2*z3 - y3*z2);
    b1 = -(x1*z2 - x2*z1 - x1*z3 + x3*z1 + x2*z3 - x3*z2);
    c1 = (x1*y2 - x2*y1 - x1*y3 + x3*y1 + x2*y3 - x3*y2);
    d1 = -(x1*y2*z3 - x1*y3*z2 - x2*y1*z3 + x2*y3*z1 + x3*y1*z2 - x3*y2*z1);

    a2 = 2 * (x2 - x1);
    b2 = 2 * (y2 - y1);
    c2 = 2 * (z2 - z1);
    d2 = x1 * x1 + y1 * y1 + z1 * z1 - x2 * x2 - y2 * y2 - z2 * z2;

    a3 = 2 * (x3 - x1);
    b3 = 2 * (y3 - y1);
    c3 = 2 * (z3 - z1);
    d3 = x1 * x1 + y1 * y1 + z1 * z1 - x3 * x3 - y3 * y3 - z3 * z3;

    xCenter = -(b1*c2*d3 - b1*c3*d2 - b2*c1*d3 + b2*c3*d1 + b3*c1*d2 - b3*c2*d1)
        / (a1*b2*c3 - a1*b3*c2 - a2*b1*c3 + a2*b3*c1 + a3*b1*c2 - a3*b2*c1);
    yCenter = (a1*c2*d3 - a1*c3*d2 - a2*c1*d3 + a2*c3*d1 + a3*c1*d2 - a3*c2*d1)
        / (a1*b2*c3 - a1*b3*c2 - a2*b1*c3 + a2*b3*c1 + a3*b1*c2 - a3*b2*c1);
    zCenter = -(a1*b2*d3 - a1*b3*d2 - a2*b1*d3 + a2*b3*d1 + a3*b1*d2 - a3*b2*d1)
        / (a1*b2*c3 - a1*b3*c2 - a2*b1*c3 + a2*b3*c1 + a3*b1*c2 - a3*b2*c1);

    radius = sqrt(pow(xCenter - x1, 2) + pow(yCenter - y1, 2) + pow(zCenter - z1, 2));

// 	double  k11, k12, k13, k14;
// 	k11 = (x1 - y3)*(z2 - z3) - (y2 - y3)*(z1 - z3);
// 	k12 = (x2 - x3)*(z1 - z3) - (x1 - x3)*(z2 - z3);
// 	k13 = (x1 - x3)*(y2 - y3) - (x2 - x3)*(y1 - y3);
// 	k14 = -(k11*x3 + k12 * y3 + z3);
// 
// 	double  k21, k22, k23, k24;
// 	k21 = x2 - x1;
// 	k22 = y2 - y1;
// 	k23 = z2 - z1;
// 	k24 = ((pow(x2, 2) - pow(x1, 2) + pow(y2, 2) - pow(y1, 2) + pow(z2, 2) - pow(z1, 2))) / 2.0;
// 
// 	double  k31, k32, k33, k34;
// 	k31 = x3 - x2;
// 	k32 = y3 - y2;
// 	k33 = z3 - z2;
// 	k34 = ((pow(x3, 2) - pow(x2, 2) + pow(y3, 2) - pow(y2, 2) + pow(z3, 2) - pow(z2, 2))) / 2.0;
// 	//求圆心半径
// // 	double x0, y0, z0, r;
// 	xCenter = (-k14 * k22*k33 - k24 * k32*k13 - k34 * k12*k23 + k13 * k22*k34 + k33 * k12*k24 + k23 * k32*k14) / (k11*k22*k33 + k12 * k23*k31 + k21 * k32*k13 - k13 * k22*k31 - k12 * k21*k33 - k23 * k31*k11);
// 	yCenter = (-k11 * k24*k33 - k21 * k34*k13 - k14 * k23*k31 + k13 * k24*k31 + k23 * k34*k11 + k14 * k21*k33) / (k11*k22*k33 + k12 * k23*k31 + k21 * k32*k13 - k13 * k22*k31 - k12 * k21*k33 - k23 * k31*k11);
// 	zCenter = (-k11 * k22*k34 - k21 * k32*k14 - k12 * k24*k31 + k14 * k22*k31 + k12 * k21*k34 + k24 * k32*k11) / (k11*k22*k33 + k12 * k23*k31 + k21 * k32*k13 - k13 * k22*k31 - k12 * k21*k33 - k23 * k31*k11);
// 	radius = sqrt(pow(xCenter - x1, 2) + pow(yCenter - y1, 2) + pow(zCenter - z1, 2));
}

// bool ShapeUtilities::isShapePlannar(const TopoDS_Shape& shape, double& xNormal, double& yNormal, double& zNormal, bool& hasNormal)
// {
// 	Handle(Geom_Plane) plane = findPlane(shape);
// 	if (plane)
// 	{
// 		gp_Ax1 ax = plane->Axis();
// 		xNormal = ax.Direction().X();
// 		yNormal = ax.Direction().Y();
// 		zNormal = ax.Direction().Z();
// 	}
// 	else
// 	{
// 		return false;
// 	}
// 
// //     hasNormal = false;
// //     TopExp_Explorer exp(shape, TopAbs_VERTEX);
// //     int numVertex = 0;
// //     for (; exp.More(); exp.Next())
// //     {
// //         numVertex++;
// //     }
// //     if (numVertex < 2)
// //     {
// //         return false;
// //     }
// //     else if (numVertex == 2)
// //     {
// //         hasNormal = true;
// //         return false;
// //     }
// // 
// //     gp_Pnt firstPnt, secondPnt;
// //     exp.Init(shape, TopAbs_VERTEX);
// //     firstPnt = BRep_Tool::Pnt(TopoDS::Vertex(exp.Value()));
// //     exp.Next();
// //     secondPnt = BRep_Tool::Pnt(TopoDS::Vertex(exp.Value()));
// //     exp.Next();
// //     gp_Dir normal(0, 0, 0);
// // 
// //     bool planar = false;
// //     hasNormal = true;
// //     for (; exp.More(); exp.Next())
// //     {
// //         gp_Pnt restPnt = BRep_Tool::Pnt(TopoDS::Vertex(exp.Value()));
// //         gp_Dir restTangent1(restPnt.XYZ() - firstPnt.XYZ());
// //         gp_Dir restTangent2(restPnt.XYZ() - firstPnt.XYZ());
// //         if (restTangent1.IsParallel(restTangent2, 0.0001))
// //         {
// //             continue;
// //         }
// // 
// //         gp_Dir normal2 = restTangent1.Crossed(restTangent2);
// //         if (!normal.XYZ().IsEqual(gp_Dir(0, 0, 0).XYZ(), 0.0001))
// //         {
// //             if (!normal2.IsParallel(normal, 0.0001))
// //             {
// //                 hasNormal = false;
// //                 return false;
// //             }
// //             else
// //             {
// //                 planar = true;
// //             }
// //         }
// //         else
// //         {
// //             normal = normal2;
// //             planar = true;
// //         }
// //     }
// // 
// // //     if (hasNormal)
// // //     {
// // //         gp_Dir tangent(secondPnt.XYZ() - firstPnt.XYZ());
// // //         gp_Dir dir1(1, 0, 0), dir2(0, 0, 1);
// // //         if (dir1.IsParallel(tangent, 0.0001))
// // //         {
// // //             normal = tangent.Crossed(dir2);
// // //         }
// // //         else
// // //         {
// // //             normal = tangent.Crossed(dir1);
// // //         }
// // //     }
// // 
// //     xNormal = normal.X();
// //     yNormal = normal.Y();
// //     zNormal = normal.Z();
// // 
// // 
// // 
// // 
// // 
// // 
// //     return planar;
// }

bool ShapeUtilities::isShapePlannar(const TopoDS_Shape& shape, gp_Pln& plane)
{
	try
	{
		TopExp_Explorer exp(shape, TopAbs_VERTEX);
		int i = 0;
		for (; exp.More(); exp.Next())
		{
			i++;
		}

		TColgp_Array1OfPnt vertices(1,i);
		i = 0;
		exp.Init(shape, TopAbs_VERTEX);
		for (; exp.More(); exp.Next())
		{
			i++;
			vertices.SetValue(i, BRep_Tool::Pnt(TopoDS::Vertex(exp.Value())));
		}

		GProp_PEquation PE(vertices, Precision::Confusion());

		if (!PE.IsPlanar())
		{
			return false;
		}
		else
		{
			plane = PE.Plane();
		}
		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool ShapeUtilities::isFacePlannar(const TopoDS_Shape& face, gp_Pln& plane, double precision)
{
	try
	{
        Handle(Geom_Surface) surf = BRep_Tool::Surface(TopoDS::Face(face));
        if (!surf)
            return false;
        GeomAbs_SurfaceType type;
        Handle(Geom_Surface) basicsurf = GetBasicSurface(surf, type);

        switch (type)
        {
        case GeomAbs_Plane:
            {
                Handle(Geom_Plane) cassurf = Handle(Geom_Plane)::DownCast(basicsurf);
                plane = cassurf->Pln();
                return true;
                break;
			}
		case GeomAbs_Cylinder:
		case GeomAbs_Cone:
		case GeomAbs_Sphere:
		case GeomAbs_Torus:
        {
            return false;
            break;
        }
		case GeomAbs_BezierSurface:
		case GeomAbs_BSplineSurface:
		case GeomAbs_SurfaceOfRevolution:
		case GeomAbs_SurfaceOfExtrusion:
		case GeomAbs_OffsetSurface:
		case GeomAbs_OtherSurface:
            return isShapePlannarNumericalSolution(face, plane, precision);
        }
		return false;
	}
	catch (...)
	{
		return false;
	}
}

int ShapeUtilities::getNumTopoType(const TopoDS_Shape& shape, const TopAbs_ShapeEnum& type)
{
	try
	{
		int num = 0;

		TopExp_Explorer exp(shape, type);
		for (; exp.More(); exp.Next())
		{
			num++;
		}

		return num;
	}
	catch (...)
	{
		return -1;
	}
}

// Handle(Geom_Plane) ShapeUtilities::findPlane(const TopoDS_Shape& S)
// {
// // 	Handle(GeomLib_IsPlanarSurface) tool = new GeomLib_IsPlanarSurface()
//     Handle(Geom_Plane) plane = nullptr;
//     BRepBuilderAPI_FindPlane planeFinder(S);
//     if (planeFinder.Found())
//         plane = planeFinder.Plane();
//     return plane;
// }

// bool ShapeUtilities::FindConnectedEdges(const TopoDS_Shape& F,//orig const TopoDS_Face& F
//     const TopoDS_Vertex& V,
//     TopoDS_Edge& E1,
//     TopoDS_Edge& E2)
// {
//     TopTools_IndexedDataMapOfShapeListOfShape  vertexMap;
//     TopExp::MapShapesAndAncestors(F, TopAbs_VERTEX,
//         TopAbs_EDGE, vertexMap);
// 
//     if (vertexMap.Contains(V)) {
//         TopTools_ListIteratorOfListOfShape iterator(vertexMap.FindFromKey(V));
//         if (iterator.More()) {
//             E1 = TopoDS::Edge(iterator.Value());
//             iterator.Next();
//         } // if ...
//         else return false;
//         if (iterator.More()) {
//             E2 = TopoDS::Edge(iterator.Value());
//             iterator.Next();
//         } // if ...
//         else return false;
// 
//         if (iterator.More()) return false;
//     } // if (isFind)
//     else return false;
//     return true;
// } // FindConnectedEdges

bool ShapeUtilities::FindConnectedEdges(const TopoDS_Shape& F/*orig const TopoDS_Face& F*/, const TopoDS_Vertex& V, TopTools_ListOfShape& edges)
{
	TopTools_IndexedDataMapOfShapeListOfShape  vertexMap;
	TopExp::MapShapesAndAncestors(F, TopAbs_VERTEX,
		TopAbs_EDGE, vertexMap);

	if (vertexMap.Contains(V)) {
		TopTools_ListIteratorOfListOfShape iterator(vertexMap.FindFromKey(V));
		for (; iterator.More(); iterator.Next())
		{
			TopoDS_Edge E = TopoDS::Edge(iterator.Value());
			if (!edges.Contains(E))
			{
				edges.Append(E);
			}
		}
	} // if (isFind)
	else 
	{
		return false;
	}
	return true;
}

bool ShapeUtilities::PlanarBaryCalculate(const TopoDS_Shape& planarShape, gp_XYZ& bary)
{
//     if (!planarShape.Closed())
//     {
//         return false;
//     }

    TopoDS_Wire sourceWire;
    if (!shapeFilter(planarShape, TopAbs_WIRE, sourceWire))
    {
        return false;
    }

    // Calculate approximate barycenter
    BRepTools_WireExplorer Exp(sourceWire);
    // Class BRep_Tool without fields and without Constructor :
    //  BRep_Tool BT;
    bary = gp_XYZ(0., 0., 0.);
    Standard_Integer nb;

    for (nb = 0; Exp.More(); Exp.Next()) {
        //    Bary += BT.Pnt(Exp.CurrentVertex()).XYZ();
        bary += BRep_Tool::Pnt(Exp.CurrentVertex()).XYZ();
        nb++;
    }
    bary /= nb;

    return true;
}

bool ShapeUtilities::GetCircleCenter(Handle(Geom_Curve) curve, gp_Pnt& pnt)
{
    try
    {
        if (curve->IsKind("Geom_Circle"))
        {
            Handle(Geom_Circle) circle = Handle(Geom_Circle)::DownCast(curve);
            pnt = circle->Circ().Location();
        }
        else if (curve->IsKind("Geom_Ellipse"))
        {
            Handle(Geom_Ellipse) ellipse = Handle(Geom_Ellipse)::DownCast(curve);
            pnt = ellipse->Elips().Location();
        }
        else if (curve->IsKind("Geom_TrimmedCurve"))
        {
            Handle(Geom_TrimmedCurve) trimmedCurve = Handle(Geom_TrimmedCurve)::DownCast(curve);
            return GetCircleCenter(trimmedCurve->BasisCurve(), pnt);
        }
        else
            return false;
        return true;
    }
    catch (...)
    {
        return false;
    }
}

void ShapeUtilities::GetAllNonCompoundShapes(const TopoDS_Shape& orgShape, TopTools_ListOfShape& collections)
{
    if (orgShape.ShapeType() != TopAbs_COMPOUND)
    {
        TopoDS_Iterator itor(orgShape);
        for (; itor.More(); itor.Next())
            GetAllNonCompoundShapes((itor.Value(), collections);
    }
    else
    {
        collections.Append(orgShape);
    }
}

void ShapeUtilities::GetAllNonCompoundShapes(const TopoDS_Shape& orgShape, TopTools_ListOfShape& solids, TopTools_ListOfShape& faces, TopTools_ListOfShape& edges)
{
	if (orgShape.ShapeType() != TopAbs_COMPOUND)
	{
		TopoDS_Iterator itor(orgShape);
        for (; itor.More(); itor.Next())
            GetAllNonCompoundShapes((itor.Value(), solids, faces, edges);
	}
	else
	{
        switch (orgShape.ShapeType())
        {
        case TopAbs_SOLID:
            solids.Append(orgShape);
            break;
        case TopAbs_SHELL:
        case TopAbs_FACE:
            faces.Append(orgShape);
            break;
        case TopAbs_EDGE:
            edges.Append(orgShape);
            break;
        }
	}
}

void ShapeUtilities::GetAllFaces(const TopoDS_Shape& orgShape, TopTools_ListOfShape& collections)
{
    if (orgShape.ShapeType() < TopAbs_FACE)
    {
        TopoDS_Iterator itor(orgShape);
        for (; itor.More(); itor.Next())
            GetAllFaces((itor.Value(), collections);
    }
    else if (orgShape.ShapeType() == TopAbs_FACE)
    {
        collections.Append(orgShape);
    }
    else
    {
        return;
    }
}

void ShapeUtilities::GetShapeTypeRange(const TopTools_ListOfShape& shapes, TopAbs_ShapeEnum& lower, TopAbs_ShapeEnum& upper)
{
    if (shapes.Size() == 0)
    {
        lower = upper = TopAbs_SHAPE;
        return;
    }
    TopTools_ListOfShape::Iterator iter(shapes);
    lower = upper = iter.Value().ShapeType();
    iter.Next();
    for (; iter.More(); iter.Next())
    {
        TopAbs_ShapeEnum curtype = iter.Value().ShapeType();
        if (curtype > lower)
            lower = curtype;
        if (curtype < upper)
            upper = curtype;
    }
}

Handle(Geom_Surface) ShapeUtilities::GetBasicSurface(Handle(Geom_Surface) surf, GeomAbs_SurfaceType& type)
{
    const Handle(Standard_Type)& thetype = surf->DynamicType();
    if (thetype == STANDARD_TYPE(Geom_RectangularTrimmedSurface))
    {
        return GetBasicSurface(Handle(Geom_RectangularTrimmedSurface)::DownCast(surf)->BasisSurface(), type);
    }
    else if (thetype == STANDARD_TYPE(Geom_Plane))
        type = GeomAbs_Plane;
    else if (thetype == STANDARD_TYPE(Geom_CylindricalSurface))
        type = GeomAbs_Cylinder;
    else if (thetype == STANDARD_TYPE(Geom_ConicalSurface))
        type = GeomAbs_Cone;
    else if (thetype == STANDARD_TYPE(Geom_SphericalSurface))
        type = GeomAbs_Sphere;
    else if (thetype == STANDARD_TYPE(Geom_ToroidalSurface))
        type = GeomAbs_Torus;
    else if (thetype == STANDARD_TYPE(Geom_SurfaceOfRevolution))
        type = GeomAbs_SurfaceOfRevolution;
    else if (thetype == STANDARD_TYPE(Geom_SurfaceOfLinearExtrusion))
        type = GeomAbs_SurfaceOfExtrusion;
    else if (thetype == STANDARD_TYPE(Geom_BezierSurface))
        type = GeomAbs_BezierSurface;
    else if (thetype == STANDARD_TYPE(Geom_BSplineSurface))
        type = GeomAbs_BSplineSurface;
    else if (thetype == STANDARD_TYPE(Geom_OffsetSurface))
        type = GeomAbs_OffsetSurface;
    else
        type = GeomAbs_OtherSurface;
    return surf;
}

Handle(Geom_Curve) ShapeUtilities::GetBasicCurve(Handle(Geom_Curve) curve)
{
    const Handle(Standard_Type)& thetype = curve->DynamicType();
    if (thetype == STANDARD_TYPE(Geom_TrimmedCurve) || thetype == STANDARD_TYPE(Geom_OffsetCurve))
    {
        return GetBasicCurve(Handle(Geom_TrimmedCurve)::DownCast(curve)->BasisCurve());
    }
    else
        return curve;
}

bool ShapeUtilities::GetPntUVonFace(TopoDS_Shape& face, gp_Pnt& pnt, double& UValue, double& VValue)
{
    TopoDS_Face curFace;
    if (face.ShapeType() <= TopAbs_FACE)
    {
        if (!shapeFilter(face, TopAbs_FACE, curFace))
            return false;
    }
    else
        return false;

    Handle(Geom_Surface) surface = BRep_Tool::Surface(TopoDS::Face(curFace));
    ShapeAnalysis_Surface anaSurf(surface);
    gp_Pnt2d pnt2d = anaSurf.ValueOfUV(pnt, Precision::Confusion() * 100);

    UValue = pnt2d.X();
    VValue = pnt2d.Y();

    return true;
}

bool ShapeUtilities::GetPntUVonFace(TopoDS_Shape face, TopoDS_Shape edge, gp_Pnt& pnt, double& UValue, double& VValue)
{
    TopoDS_Face curFace;
	if (face.ShapeType() <= TopAbs_FACE)
	{
		if (!shapeFilter(face, TopAbs_FACE, curFace))
			return false;
	}
	else
		return false;

	Handle(Geom_Surface) surface = BRep_Tool::Surface(TopoDS::Face(curFace));
	ShapeAnalysis_Surface anaSurf(surface);
	gp_Pnt2d pnt2d = anaSurf.ValueOfUV(pnt, Precision::Confusion() * 100);

	UValue = pnt2d.X();
	VValue = pnt2d.Y();

    if (surface->IsKind(STANDARD_TYPE(Geom_SurfaceOfRevolution)))
    {
        TopoDS_Edge dsEdge = TopoDS::Edge(edge);
        TopoDS_Face dsFace = TopoDS::Face(face);
        ShapeAnalysis_Edge sae;
        gp_Pnt2d first, last;
        sae.BoundUV(dsEdge, dsFace, first, last);

        bool uclosed = surface->IsUClosed();
        bool vclosed = surface->IsVClosed();

        double umin, umax, vmin, vmax;
        surface->Bounds(umin, umax, vmin, vmax);
        if (uclosed && abs(UValue - umin) < Precision::Confusion() * 100)
        {
            //ShapeAnalysis_Edge sae;
            if (sae.HasPCurve(dsEdge, dsFace))
            {
                gp_Dir2d dir(gp_Vec2d(first, last));
                if (abs(last.X() - umin) < Precision::Confusion() * 100 && abs(last.X() - first.X()) > Precision::Confusion() * 100)
                {
                    UValue = umax;
                }
            }
        }

        if (vclosed && abs(VValue - vmin) < Precision::Confusion() * 100)
        {
			gp_Dir2d dir(gp_Vec2d(first, last));
			if (abs(last.Y() - vmin) < Precision::Confusion() * 100 && abs(last.Y() - first.Y()) > Precision::Confusion() * 100)
			{
				VValue = vmax;
			}
        }
    }
    return true;
}

bool ShapeUtilities::GetPntUVonEdge(TopoDS_Shape edge, gp_Pnt& pnt, double& UValue)
{
    TopoDS_Face curEdge;
	if (curEdge.ShapeType() <= TopAbs_EDGE)
	{
		if (!shapeFilter(edge, TopAbs_EDGE, curEdge))
			return false;
	}
	else
		return false;
    
    double start, end;
    Handle(Geom_Curve) curve = BRep_Tool::Curve(TopoDS::Edge(curEdge), start, end);
    GeomAPI_ProjectPointOnCurve apj1(pnt, curve);
    apj1.Perform(pnt);
    if (apj1.NbPoints() < 1)
        return false;
    UValue = apj1.Parameter(1);
    return true;
}

bool ShapeUtilities::IsShapeGeomSame(const TopoDS_Shape shape1, const TopoDS_Shape shape2, const TopAbs_ShapeEnum type)
{
    try
    {
        if (shape1.ShapeType() > type || shape2.ShapeType() > type)
            return false;
        TopoDS_Shape filterShape1, filterShape2;
        shapeFilter(shape1, type, filterShape1);
        shapeFilter(shape2, type, filterShape2);

        if (type == TopAbs_VERTEX)
        {
            if (BRep_Tool::Pnt(TopoDS::Vertex(filterShape1)).IsEqual(BRep_Tool::Pnt(TopoDS::Vertex(filterShape2)), Precision::Confusion()))
                return true;
        }
        else if (type == TopAbs_EDGE)
        {
            double start1, end1, start2, end2;
            Handle(Geom_Curve) curve1 = BRep_Tool::Curve(TopoDS::Edge(filterShape1), start1, end1);
            Handle(Geom_Curve) curve2 = BRep_Tool::Curve(TopoDS::Edge(filterShape2), start2, end2);
            if (!curve1 || !curve2)
                return false;

            if (curve1->Value(start1).IsEqual(curve2->Value(start2), Precision::Confusion()) &&
                curve1->Value(end1).IsEqual(curve2->Value(end2), Precision::Confusion()) &&
                curve1->Value(0.5 * (start1 + end1)).IsEqual(curve2->Value(0.5 * (start2 + end2)), Precision::Confusion()))
                return true;
			else if (curve1->Value(start1).IsEqual(curve2->Value(end2), Precision::Confusion()) &&
				curve1->Value(end1).IsEqual(curve2->Value(start2), Precision::Confusion()) &&
				curve1->Value(0.5 * (start1 + end1)).IsEqual(curve2->Value(0.5 * (start2 + end2)), Precision::Confusion()))
				return true;
        }
        else if (type == TopAbs_FACE)
        {
            double ustart1, uend1, vstart1, vend1;
            double ustart2, uend2, vstart2, vend2;
            Handle(Geom_Surface) surface1 = BRep_Tool::Surface(TopoDS::Face(filterShape1));
            Handle(Geom_Surface) surface2 = BRep_Tool::Surface(TopoDS::Face(filterShape2));

            BRepAdaptor_Surface adp_sur1(TopoDS::Face(filterShape1));
            BRepAdaptor_Surface adp_sur2(TopoDS::Face(filterShape2));
            ustart1 = adp_sur1.FirstUParameter();
            uend1 = adp_sur1.LastUParameter();
            vstart1 = adp_sur1.FirstVParameter();
            vend1 = adp_sur1.LastVParameter();

			ustart2 = adp_sur2.FirstUParameter();
			uend2 = adp_sur2.LastUParameter();
			vstart2 = adp_sur2.FirstVParameter();
			vend2 = adp_sur2.LastVParameter();

            if (surface1->Value(ustart1, vstart1).IsEqual(surface2->Value(ustart2, vstart2), Precision::Confusion()) &&
                surface1->Value(uend1, vstart1).IsEqual(surface2->Value(uend2, vstart2), Precision::Confusion()) &&
                surface1->Value(ustart1, vend1).IsEqual(surface2->Value(ustart2, vend2), Precision::Confusion()) &&
                surface1->Value(uend1, vend1).IsEqual(surface2->Value(uend2, vend2), Precision::Confusion()) &&
                surface1->Value(0.5 * (ustart1 + uend1), 0.5 * (vstart1 + vend1)).IsEqual(surface2->Value(0.5 * (ustart2 + uend2), 0.5 * (vstart2 + vend2)), Precision::Confusion()))
                return true;
        }
        return false;
    }
    catch (...)
    {
        return false;
    }
}

void ShapeUtilities::FindAllCells(const TopoDS_Shape shape, TopTools_ListOfShape& collections)
{
    if (shape.ShapeType() == TopAbs_SOLID)
    {
        collections.Append(shape);
        return;
    }

    TopTools_ListOfShape orgcollections;
    FindAllCellsWithoutCheck(shape, orgcollections);

    for (auto iter : orgcollections)
    {
        bool notCell = false;
        TopoDS_Shape curShape = iter;
        for (auto iter2 : orgcollections)
        {
            TopoDS_Shape toolshape = iter2;
            if(curShape.IsSame(toolshape))
                continue;

            TopTools_IndexedMapOfShape shapes;
            TopExp::MapShapes(toolshape, curShape.ShapeType(), shapes);
            if (shapes.Contains(curShape))
            {
                notCell = true;
                break;
            }
        }
        if (!notCell)
            collections.Append(curShape);
    }
}

bool ShapeUtilities::ConvertBoundedCurve(TopoDS_Shape edge, Handle(Geom_BoundedCurve)& BndCurve)
{
    try
    {
        if (edge.ShapeType() != TopAbs_EDGE)
            return false;
        double first, last;
        Handle(Geom_Curve) curve = BRep_Tool::Curve(TopoDS_Edge(edge), first, last);
        BndCurve = Handle(Geom_BoundedCurve)::DownCast(curve);
        if (!BndCurve)
            BndCurve = new Geom_TrimmedCurve(curve, first, last);
        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool ShapeUtilities::SameDimension(const TopTools_ListOfShape collections, int& minDim, int& maxDim)
{
    minDim = maxDim = -1;
    try
    {
        std::set<int> dimensions;
        for (auto iter : collections)
        {
            switch (iter.ShapeType())
            {
            case TopAbs_COMPOUND:
            case TopAbs_COMPSOLID:
            case TopAbs_SOLID:
                dimensions.insert(3);
                break;
            case TopAbs_SHELL:
            case TopAbs_FACE:
				dimensions.insert(2);
				break;
            case TopAbs_WIRE:
            case TopAbs_EDGE:
				dimensions.insert(1);
				break;
			case TopAbs_VERTEX:
				dimensions.insert(0);
				break;
            }
        }

        if (dimensions.size() > 1)
        {
            minDim = *dimensions.begin();
            minDim = *(--dimensions.end());
        }
        else if (dimensions.size() == 1)
        {
            minDim = maxDim = *dimensions.begin();
            return true;
        }
        else
            return false;
        return false;
    }
    catch (...)
    {
        return false;
    }
}

bool ShapeUtilities::FaceNormal(const TopoDS_Face face, const double& u, const double& v, gp_Dir& dir)
{
    BRepGProp_Face anaface(face);
    double umin, umax, vmin, vmax;
    anaface.Bounds(umin, umax, vmin, vmax);
    gp_Pnt pnt;
    gp_Vec normal;
    anaface.Normal(u, v, pnt, normal);
    if (normal.Magnitude() == 0)
    {
        Handle(Geom_Surface) surface = BRep_Tool::Surface(TopoDS::Face(face));
        GeomAbs_SurfaceType type;
        Handle(Geom_Surface) basicSurf = ShapeUtilities::GetBasicSurface(surface, type);
        if (basicSurf->IsKind(STANDARD_TYPE(Geom_SurfaceOfRevolution)))
        {
            Handle(Geom_SurfaceOfRevolution) finalSurf = Handle(Geom_SurfaceOfRevolution)::DownCast(basicSurf);
            normal = finalSurf->Axis().Direction();
        }
        else if (basicSurf->IsKind(STANDARD_TYPE(Geom_ConicalSurface)))
        {
            Handle(Geom_ConicalSurface) finalSurf = Handle(Geom_ConicalSurface)::DownCast(basicSurf);
            normal = finalSurf->Axis().Direction();
        }
        else if (basicSurf->IsKind(STANDARD_TYPE(Geom_SphericalSurface)))
        {
            Handle(Geom_SphericalSurface) finalSurf = Handle(Geom_SphericalSurface)::DownCast(basicSurf);
            normal = finalSurf->Axis().Direction();
        }
        else
            return false;
    }
    dir = gp_Dir(normal);

    return true;
}

bool ShapeUtilities::FaceMiddleNormal(const TopoDS_Face face, gp_Dir& dir)
{
    BRepGProp_Face anaface(face);
    double umin, umax, vmin, vmax;
	anaface.Bounds(umin, umax, vmin, vmax);
	gp_Pnt pnt;
	gp_Vec normal;
    double u = 0.5 * (umin + umax);
    double v = 0.5 * (vmin + vmax);
	anaface.Normal(u, v, pnt, normal);
    
	if (normal.Magnitude() == 0)
	{
		Handle(Geom_Surface) surface = BRep_Tool::Surface(TopoDS::Face(face));
		GeomAbs_SurfaceType type;
		Handle(Geom_Surface) basicSurf = ShapeUtilities::GetBasicSurface(surface, type);
		if (basicSurf->IsKind(STANDARD_TYPE(Geom_SurfaceOfRevolution)))
		{
			Handle(Geom_SurfaceOfRevolution) finalSurf = Handle(Geom_SurfaceOfRevolution)::DownCast(basicSurf);
			normal = finalSurf->Axis().Direction();
		}
		else if (basicSurf->IsKind(STANDARD_TYPE(Geom_ConicalSurface)))
		{
			Handle(Geom_ConicalSurface) finalSurf = Handle(Geom_ConicalSurface)::DownCast(basicSurf);
			normal = finalSurf->Axis().Direction();
		}
		else if (basicSurf->IsKind(STANDARD_TYPE(Geom_SphericalSurface)))
		{
			Handle(Geom_SphericalSurface) finalSurf = Handle(Geom_SphericalSurface)::DownCast(basicSurf);
			normal = finalSurf->Axis().Direction();
		}
		else
			return false;
	}
	dir = gp_Dir(normal);

	return true;
}

bool ShapeUtilities::EdgeIsLinear(const TopoDS_Edge edge)
{
    double strt, ed;
    Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, strt, ed);
    if (!curve)
        return false;
    Handle(Geom_Curve) basiccurve = GetBasicCurve(curve);

    if(basiccurve->IsKind(STANDARD_TYPE(Geom_Line)))
        return true;
    else
    {
        BRepAdaptor_Curve adapterCurve(edge);
        double ustart = strt;
        double uend = ed;
        if (ustart > uend)
            return false;

        int sampleNum = 10;
        double diff = (uend - ustart) / (double)(sampleNum - 1);
        double curU = ustart;
        gp_Vec initVec;
        for (int j = 0; j < sampleNum; j++)
        {
            gp_Pnt curPnt;
            gp_Vec tangential;
            adapterCurve.D1(curU, curPnt, tangential);
            if (j == 0)
            {
                initVec = tangential;
            }
            else
            {
                if (!initVec.IsParallel(tangential, Precision::Angular() * 100))
                    return false;
            }
            curU += diff;
        }
        return true;
    }
}

typedef NCollection_List<gp_Dir> DirList;
bool ShapeUtilities::ReverseFaceNormal4CellsBuild(const TopTools_ListOfShape cellsArgs, const TopTools_ListOfShape cellsTools,
    TopTools_ListOfShape& newcellsArgs, TopTools_ListOfShape& newcellsTools)
{
    BRep_Builder B;
    TopoDS_Compound cmp;
    B.MakeCompound(cmp);

    for (auto iter : cellsArgs)
    {
        B.Add(cmp, iter);
        GetAllFaces(iter, newcellsArgs);
    }
	for (auto iter : cellsToolss)
	{
		B.Add(cmp, iter);
		GetAllFaces(iter, newcellsTools);
	}

    gp_Pln plane;
    if (!IsShapeGeomSame(cmp, plane))
        return false;

    DirList refDirList;
    gp_Dir instanceDir;

	for (auto iter : newcellsArgs)
	{
        FaceMiddleNormal(TopoDS::Face(iter), instanceDir);
        UnifyAllFaceNormal(instanceDir, newcellsArgs);
        refDirList.Append(instanceDir);
	}

    for (auto iter : refDirList)
    {
        UnifyAllFaceNormal(iter, newcellsTools);
    }

    return true;
}

CoinsideType ShapeUtilities::IsEdgeCoincide(const TopoDS_Shape blankedge, const TopoDS_Shape tooledge, const double tolerance)
{
    try
    {
        if (IsShapeGeomSame(blankedge, tooledge, TopAbs_EDGE))
            return Same;

        BOPAlgo_CellsBuilder* mbuilder = new BOPAlgo_CellsBuilder;

        double tol = tolerance > Precision::Confusion() ? tolerance : Precision::Confusion();
        mbuilder->SetRunParallel(true);
        mbuilder->SetFuzzyValue(tol);

        mbuilder->Clear();
        mbuilder->AddArgument(blankedge);
        mbuilder->AddArgument(tooledge);
        mbuilder->Perform();
        if (mbuilder->HasErrors())
            return Neither;

        TopTools_ListOfShape totake, toavoid;
        totake.Append(blankedge);
        toavoid.Append(tooledge);
        mbuilder->AddToResult(totake, toavoid);
        TopoDS_Shape targetshape = mbuilder->Shape();
        if (targetshape.ShapeType() > TopAbs_EDGE)
            return NotConside;

        double blanklen, toollen, containlen;

        OCCBasicTools::GetShapeVolume(blankedge, TopAbs_EDGE, blanklen);
        OCCBasicTools::GetShapeVolume(tooledge, TopAbs_EDGE, toollen);
        OCCBasicTools::GetShapeVolume(targetshape, TopAbs_EDGE, containlen);

        if (containlen < tol)
            return NotConside;
        else if (blanklen - containlen < tol && blanklen < toollen)
            return Contained;
        else if (toollen - containlen < tol && blanklen > toollen)
            return Containing;
        else if (containlen < toollen && containlen < blanklen)
            return PartCoinside;
        else
            return Neither;
    }
    catch (...)
    {
        return Neither;
    }
}

void ShapeUtilities::BoxEFSort(const TopoDS_Shape box, TopTools_IndexedMapOfShape& faces, TopTools_IndexedMapOfShape& edges)
{
    faces.Clear();
    edges.Clear();

    bool acisOrder = false;
    if (acisOrder)
    {
        TopTools_IndexedMapOfShape orgfaces, orgedges;
        TopExp::MapShapes(box, TopAbs_FACE, orgfaces);
        TopExp::MapShapes(box, TopAbs_EDGE, orgedges);

        faces.Add(orgfaces.FindKey(6));
        faces.Add(orgfaces.FindKey(5));
        faces.Add(orgfaces.FindKey(3));
        faces.Add(orgfaces.FindKey(1));
        faces.Add(orgfaces.FindKey(4));
        faces.Add(orgfaces.FindKey(2));

        edges.Add(orgedges.FindKey(8));
        edges.Add(orgedges.FindKey(12));
        edges.Add(orgedges.FindKey(3));
        edges.Add(orgedges.FindKey(10));
        edges.Add(orgedges.FindKey(6));
        edges.Add(orgedges.FindKey(9));
        edges.Add(orgedges.FindKey(1));
        edges.Add(orgedges.FindKey(11));
        edges.Add(orgedges.FindKey(2));
        edges.Add(orgedges.FindKey(5));
        edges.Add(orgedges.FindKey(4));
        edges.Add(orgedges.FindKey(7));
    }
    else
    {
        TopTools_IndexedMapOfShape targetfaces;
        TopExp::MapShapes(box, TopAbs_FACE, targetfaces);
        for (TopTools_IndexedMapOfShape::Iterator iter(targetfaces); iter.More(); iter.Next())
            faces.Add(iter.Value());

		TopTools_IndexedMapOfShape targetedges;
		TopExp::MapShapes(box, TopAbs_EDGE, targetedges);
		for (TopTools_IndexedMapOfShape::Iterator iter(targetedges); iter.More(); iter.Next())
			edges.Add(iter.Value());
    }
}

void ShapeUtilities::GetMultishellSolids(const TopTools_ListOfShape solids, TopTools_ListOfShape& singleShellSolid, TopTools_ListOfShape& multiShellSolid)
{
    for (auto iter : solids)
    {
        TopTools_IndexedMapOfShape shells;
        TopExp::MapShapes(iter, TopAbs_SHELL, shells);
        if (shells.Size() > 1)
            multiShellSolid.Append(iter);
        else
            singleShellSolid.Append(iter);
    }
}

void ShapeUtilities::UnifyAllFaceNormal(const gp_Dir refDir, TopTools_ListOfShape& cellsArgs)
{
    gp_Dir instanceDir;
    for (auto iter : cellsArgs)
    {
        TopoDS_Shape changeface = iter;
        FaceMiddleNormal(TopoDS::Face(changeface), instanceDir);
        if (refDir.IsEqual(instanceDir, Precision::Confusion()))
            continue;
        else if(refDir.IsOpposite(instanceDir, Precision::Confusion()))
            changeface.Reverse();
        else
            continue;
    }
}

void ShapeUtilities::FindAllCellsWithoutCheck(const TopoDS_Shape shape, TopTools_ListOfShape& collections)
{
    if (shape.ShapeType() > TopAbs_FACE)
        return;
    TopoDS_Iterator iter(shape);
    for (; iter.More(); iter.Next())
    {
        TopoDS_Shape subshape = iter.Value();
        TopAbs_ShapeEnum sptype = subshape.ShapeType();
        if (sptype == TopAbs_SOLID)
        {
            if (!collections.Contains(subshape))
                collections.Append(subshape);
        }
        else if (sptype == TopAbs_FACE)
        {
            if (shape.ShapeType() == TopAbs_SHELL)
            {
                TopTools_IndexedMapOfShape faces;
TopExp::MapShapes(shape, TopAbs_FACE, faces);
if (faces.Size() > 1)
continue;
else
{
    if (!collections.Contains(subshape))
        collections.Append(subshape);
}
            }
        }
        else
        FindAllCellsWithoutCheck(subshape, collections);
    }
}

bool ShapeUtilities::IsShapePlannarNumericalSolution(const TopoDS_Shape shape, gp_Pln& plane, double precision)
{
    try
    {
        if (precision == 0)
            precision = Precision::Confusion();

        TopExp_Explorer ex(shape, TopAbs_EDGE);
        int i = 0;
        for (; ex.More(); ex.Next())
            i++;
        if (i < 3)
        {
            std::vector<std::vector<gp_Pnt>> pnts;

            TopExp_Explorer exedge(shape, TopAbs_EDGE);
            for (; exedge.More(); exedge.Next())
            {
                TopoDS_Shape edge = exedge.Current();
                double start, end;
                Handle(Geom_Curve) curve = BRep_Tool::Curve(TopoDS::Edge(edge), start, end);
                if (!curve)
                    continue;

                gp_Pnt startpt, endpt, midpt, pt14, pt34;
                curve->D0(start, startpt);
                curve->D0(end, endpt);
                curve->D0(0.5 * (start + end), midpt);
                curve->D0(start + (end - start) / 4.0, pt14);
                curve->D0(start - (end - start) / 4.0, pt34);

                std::vector<gp_Pnt> edgepts;
                edgepts.push_back(startpt);
                edgepts.push_back(endpt);
                edgepts.push_back(midpt);
                edgepts.push_back(pt14);
                edgepts.push_back(pt34);

                pnts.push_back(edgepts);
            }

            TColgp_Array1OfPnt vertices(1, i * 5);
            i = 0;
            for (int j = 0; j < pnts.size(); j++)
            {
                for (int k = 0; k < pnts[j].size(); k++)
                {
                    i++;
                    vertices.SetValue(i, pnts[j][k]);
                }
            }

            GProp_PEquation PE(vertices, precision);

            if (!PE.IsPlanar())
                return false;
            else
                plane = PE.Plane();
        }
        else
        {
            TColgp_Array1OfPnt vertices(1, i);
            ex.Init(shape, TopAbs_EDGE);
            for (; ex.More(); ex.Next())
            {
                gp_Pnt pnt = BRep_Tool::Pnt(TopExp::FirstVertex(TopoDS::Edge((ex.Current()))));
                vertices.SetValue(i, pnt);
            }

            GProp_PEquation PE(vertices, precision);
            if (!PE.IsPlanar())
                return false;
            else
                plane = PE.Plane();
        }
        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool ShapeUtilities::BuildSurfaceEdge(TopoDS_Shape owner, TopoDS_Shape preshape, TopoDS_Shape curshape, std::tuple<double, double> preValue,
    std::tuple<double, double> curValue, TopoDS_Edge& targetedge)
{
    gp_Pnt2d p1, p2;
    TopAbs_ShapeEnum preType = preshape.ShapeType();
    TopAbs_ShapeEnum curType = curshape.ShapeType();
    //输入的两个对象必须都是边，用边切面
    if (preType == TopAbs_EDGE && curType == TopAbs_EDGE)
    {
        TopoDS_Face sharedface;
        TopTools_IndexedMapOfShape faces, edges;
        //分离出体上的每一个面，看看那个面再被切割
        TopExp::MapShapes(owner, TopAbs_FACE, faces);
        for (TopTools_IndexedMapOfShape::Iterator iter(faces); iter.More(); iter.Next())
        {
            edges.Clear();
            TopExp::MapShapes(iter.Value(), TopAbs_EDGE, edges);
            if (edges.Contains(preshape) && edges.Contains(curshape))
            {
                sharedface = TopoDS::Face(iter.Value());
                break;
            }
        }

        if (sharedface.IsNull())
            return false;

        Handle(Geom_Surface) srf = BRep_Tool::Surface(TopoDS::Face(sharedface));

        bool bPreClosedPCurve = false, bCurClosedPCurve = false;
        gp_Pnt2d p11, p21, p12, p22;
        GetUVPoint(std::get<0>(preValue), TopoDS::Edge(preshape), TopoDS::Face(sharedface), true, bPreClosedPCurve, p11);
        if (bPreClosedPCurve)
        {
            GetUVPoint(std::get<0>(preValue), TopoDS::Edge(preshape), TopoDS::Face(sharedface), false, bPreClosedPCurve, p12);
        }
        GetUVPoint(std::get<0>(curValue), TopoDS::Edge(curshape), TopoDS::Face(sharedface), true, bCurClosedPCurve, p21);
        if (bCurClosedPCurve)
        {
            GetUVPoint(std::get<0>(curValue), TopoDS::Edge(curshape), TopoDS::Face(sharedface), false, bPreClosedPCurve, p22);
        }

        if (!bPreClosedPCurve && !bCurClosedPCurve)
        {
            p1 = p11;
            p2 = p21;
        }
        else if (!bPreClosedPCurve && bCurClosedPCurve)
        {
            p1 = p11;
            double mag1 = gp_Vec2d(p11, p21).Magnitude();
            double mag2 = gp_Vec2d(p11, p22).Magnitude();

            if (mag1 > mag2)
            {
                p2 = p22;
            }
            else
            {
                p2 = p21;
            }
        }
        else if (bPreClosedPCurve && !bCurClosedPCurve)
        {
            p2 = p21;
            double mag1 = gp_Vec2d(p11, p21).Magnitude();
            double mag2 = gp_Vec2d(p12, p21).Magnitude();

            if (mag1 > mag2)
            {
                p1 = p12;
            }
            else
            {
                p1 = p11;
            }
        }
        else
            return false;

        Handle(Geom2d_Line) newPCurve = new Geom2d_Line(p1, gp_Dir2d(p2.XY() - p1.XY()));
        targetedge = BRepLib_MakeEdge(newPCurve, srf, 0, gp_Vec2d(p1, p2).Magnitude());
        BRepLib::BuildCurve3d(TopoDS::Edge(targetedge));
    }
    else if (preType == TopAbs_EDGE && curType == TopAbs_FACE)
    {
        GetUVPoint(std::get<0>(curValue), std::get<1>(curValue), TopoDS::Face(curshape), p2);
        Handle(Geom_Surface) surf = BRep_Tool::Surface(TopoDS::Face(curshape));

        bool bPreClosePCurve = false;
        gp_Pnt2d p11, p12;
        GetUVPoint(std::get<0>(preValue), TopoDS::Edge(preshape), TopoDS::Face(curshape), true, bPreClosePCurve, p11);
        if (bPreClosePCurve)
        {
            GetUVPoint(std::get<0>(preValue), TopoDS::Edge(preshape), TopoDS::Face(curshape), false, bPreClosePCurve, p12);
            double mag1 = gp_Vec2d(p11, p2).Magnitude();
            double mag2 = gp_Vec2d(p12, p2).Magnitude();
            if (mag1 > mag2)
            {
                p1 = p12;
            }
            else
                p1 = p11;
        }
        else
        {
            p1 = p11;
        }
        Handle(Geom2d_Line) newPCurve = new Geom2d_Line(p1, gp_Dir2d(p2.XY() - p1.XY()));
        targetedge = BRepLib_MakeEdge(newPCurve, surf, 0, gp_Vec2d(p1, p2).Magnitude());
        BRepLib::BuildCurve3d(TopoDS::Edge(targetedge));
    }
    else if (preType == TopAbs_FACE && curType == TopAbs_EDGE)
    {
        GetUVPoint(std::get<0>(preValue), std::get<1>(preValue), TopoDS::Face(preshape), p1);
        Handle(Geom_Surface) surf = BRep_Tool::Surface(TopoDS::Face(preshape));

        bool bCurClosePCurve = false;
        gp_Pnt2d p21, p22;
        GetUVPoint(std::get<0>(curValue), TopoDS::Edge(curshape), TopoDS::Face(preshape), true, bCurClosePCurve, p21);
        if (bCurClosePCurve)
        {
            GetUVPoint(std::get<0>(curValue), TopoDS::Edge(curshape), TopoDS::Face(preshape), false, bCurClosePCurve, p22);
            double mag1 = gp_Vec2d(p21, p2).Magnitude();
            double mag2 = gp_Vec2d(p22, p2).Magnitude();
            if (mag1 > mag2)
            {
                p1 = p22;
            }
            else
                p1 = p21;
        }
        else
        {
            p2 = p21;
        }
        Handle(Geom2d_Line) newPCurve = new Geom2d_Line(p1, gp_Dir2d(p2.XY() - p1.XY()));
        targetedge = BRepLib_MakeEdge(newPCurve, surf, 0, gp_Vec2d(p1, p2).Magnitude());
        BRepLib::BuildCurve3d(TopoDS::Edge(targetedge));
    }
    else if (preType == TopAbs_FACE && curType == TopAbs_FACE)
    {
        GetUVPoint(std::get<0>(preValue), std::get<1>(preValue), TopoDS::Face(preshape), p1);
        Handle(Geom_Surface) surf1 = BRep_Tool::Surface(TopoDS::Face(preshape));
        GetUVPoint(std::get<0>(curValue), std::get<1>(curValue), TopoDS::Face(curshape), p2);
        Handle(Geom_Surface) surf2 = BRep_Tool::Surface(TopoDS::Face(preshape));

        if (!surf1 || !surf2)
            return false;
        if (preshape.IsSame(curshape))
        {
            Handle(Geom2d_Line) newPCurve = new Geom2d_Line(p1, gp_Dir2d(p2.XY() - p1.XY()));
            targetedge = BRepLib_MakeEdge(newPCurve, surf1, 0, gp_Vec2d(p1, p2).Magnitude());
            BRepLib::BuildCurve3d(TopoDS::Edge(targetedge));
        }
        else
            return false;
    }
    return true;
}

bool ShapeUtilities::GetUVPoint(double normalizedT, TopoDS_Edge edge, TopoDS_Face face,
    bool& firstpcurve, bool& secondpcurve, gp_Pnt2d& p)
{
    TopLoc_Location loc;
    Handle(Geom_Surface) surf = BRep_Tool::Surface(face, loc);

    double fstT, lstT, f, l, realT;
    Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, fstT, lstT);
    realT = fstT + (lstT - fstT) * normalizedT;

    Handle(Geom2d_Curve) aPCurve = BRep_Tool::CurveOnSurface(edge, face, f, l);
    if (!aPCurve)
        return;

    Handle(Geom2d_Curve) finalPCurve;
    const BRep_TEdge* TE = static_cast<const BRep_TEdge*>(edge.TShape().get());
    BRep_ListIteratorOfListOfCurveRepresentation itcr(TE->Curves());

    loc = loc.Predivided(edge.Location());
    while (itcr.More())
    {
        Handle(BRep_CurveRepresentation) cr = itcr.Value();
        if (cr->IsCurveOnSurface(surf, loc))
        {
            if (cr->IsCurveOnClosedSurface())
            {
                Handle(BRep_CurveOnClosedSurface) closedPCurve = Handle(BRep_CurveOnClosedSurface)::DownCast(cr);
                secondpcurve = true;
                if (firstpcurve)
                    finalPCurve = closedPCurve->PCurve();
                else
                    finalPCurve = closedPCurve->PCurve2();
            }
            else
            {
                finalPCurve = aPCurve;
            }
            break;
        }
        itcr.Next();
    }
    finalPCurve->D0(realT, p);
}

void ShapeUtilities::GetUVPoint(const double& normalizedU, const double& normalizedV, TopoDS_Face face, gp_Pnt2d& p)
{
    double ustart, uend, vstart, vend;
    BRepTools::UVBounds(face, ustart, uend, vstart, vend);
    p.SetX(ustart + (uend - ustart) * normalizedU);
    p.SetY(vstart + (vend - vstart) * normalizedV);
}

bool ShapeUtilities::GetPntUonEdge(TopoDS_Shape edge, gp_Pnt pnt, double& uvalue)
{
    TopoDS_Shape curedge;
    if (edge.ShapeType() <= TopAbs_EDGE)
    {
        if (!shapeFilter(edge, TopAbs_EDGE, curedge))
            return false;
    }
    else
        return false;

    double start, end;
    Handle(Geom_Curve) curve = BRep_Tool::Curve(TopoDS::Edge(curedge), start, end);
    GeomAPI_ProjectPointOnCurve apj(pnt, curve);
    apj.Perform(pnt);
    if (apj.NbPoints() < 1)
        return false;
    double dis = apj.LowerDistance();
    if (dis > 0.00001)
        return false;
    uvalue = apj.LowerDistanceParameter();
    return true;
}

bool ShapeUtilities::shapeFilter(TopoDS_Shape& origshape, TopAbs_ShapeEnum targettype,
    TopoDS_Shape& filtershape)
{
    if (origshape.NbChildren() == 1 && origshape.ShapeType() < targettype)
    {
        TopoDS_Iterator iter(origshape);
        return shapeFilter(iter.Value(), targettype, filtershape);
    }
    else if (origshape.ShapeType() == targettype)
    {
        filtershape = origshape;
        return true;
    }
    return false;
}