#include "OCCBasicTools.h"
#include "ShapeUtilities.h"
#include <map>

#include <TopTools_ListOfShape.hxx>
#include <TopTools_ListIteratorOfListOfShape.hxx>
#include <ShapeAnalysis_Edge.hxx>
#include <ShapeAnalysis_WireOrder.hxx>
#include <BRepAlgoAPI_Defeaturing.hxx>

#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_Copy.hxx>
#include <BRepBuilderAPI_Transform.hxx>

#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Face.hxx>
#include <BRep_Tool.hxx>
#include <Geom_Line.hxx>
#include <TopExp_Explorer.hxx>
#include <BRepFill_Filling.hxx>
#include <BOPAlgo_MakerVolume.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <Geom_Circle.hxx> 
//#include <Geom_Conic.hxx>

#include <gp.hxx>
#include <gp_Ax1.hxx>
#include <gp_Ax2.hxx>
#include <gp_Dir.hxx>
#include <gp_Pnt.hxx>
#include <gp_Trsf.hxx>
#include <gp_Vec.hxx>
#include <gp_Lin.hxx>

#include <BRepExtrema_DistShapeShape.hxx>
#include <GeomAPI_ExtremaCurveCurve.hxx>
#include <GeomAPI_IntCS.hxx>
#include <BRepBndLib.hxx>
#include <GProp_GProps.hxx>
#include <BRepGProp.hxx>
#include <TopoDS_Compound.hxx>
#include <BRep_Builder.hxx>
#include <GeomLProp_CLProps.hxx>
#include <GeomLProp_SLProps.hxx>
#include <gp_Pln.hxx>
#include <BRepOffsetAPI_DraftAngle.hxx> 

#include <ShapeUpgrade_RemoveInternalWires.hxx>

OCCBasicTools::OCCBasicTools()
{
}


OCCBasicTools::~OCCBasicTools()
{
}

TopoDS_Face OCCBasicTools::GetFaceFromPntList(std::vector<gp_Pnt> PntsList)
{
	TopTools_ListOfShape tmpEdgeList;

	for (int i = 0; i < PntsList.size(); ++i)
	{
		gp_Pnt NextPnt;
		if (i == PntsList.size() - 1)
		{
			NextPnt = PntsList[0];
		}
		else
		{
			NextPnt = PntsList[i + 1];
		}
		if (PntsList[i].IsEqual(NextPnt, 0.00001))
		{
			continue;
		}
		tmpEdgeList.Append(BRepBuilderAPI_MakeEdge(PntsList[i], NextPnt));
	}
	BRepBuilderAPI_MakeWire anWire;
	anWire.Add(tmpEdgeList);

	TopoDS_Wire Wire = anWire.Wire();
	return BRepBuilderAPI_MakeFace(Wire);


}

Standard_Boolean OCCBasicTools::GetOrderWireFromEdges(std::vector<TopoDS_Edge> anEdges, TopoDS_Wire& OrderWire) {
	ShapeAnalysis_Edge anEdgeAnalyser;
	ShapeAnalysis_WireOrder aWireOrder;
	for (std::vector<TopoDS_Edge>::const_iterator i = anEdges.begin();
		i != anEdges.end(); ++i)
	{
		TopoDS_Vertex aVf = anEdgeAnalyser.FirstVertex(*i);
		TopoDS_Vertex aVl = anEdgeAnalyser.LastVertex(*i);
		gp_Pnt aPf = BRep_Tool::Pnt(aVf);
		gp_Pnt aPl = BRep_Tool::Pnt(aVl);
		aWireOrder.Add(aPf.XYZ(), aPl.XYZ());
	}
	// 
	TopTools_ListOfShape aOrderedEdges;
	for (Standard_Integer e = 1; e <= aWireOrder.NbEdges(); ++e)
	{
		const TopoDS_Edge& anEdge = anEdges.at(e - 1);
		aOrderedEdges.Append(anEdge);
	}
	BRepBuilderAPI_MakeWire aWireMaker;
	aWireMaker.Add(aOrderedEdges);

	if (!aWireMaker.IsDone())
	{
		return Standard_False;
	}
	OrderWire = aWireMaker.Wire();
	return Standard_True;
}

bool OCCBasicTools::DistanceMeasurement(TopoDS_Shape shape1, TopoDS_Shape shape2, double& minDis, gp_Pnt& pnt1, gp_Pnt& pnt2)
{
    BRepExtrema_DistShapeShape measureTool;
    measureTool.LoadS1(shape1);
    measureTool.LoadS2(shape2);
    measureTool.Perform();

    if (!measureTool.IsDone())
    {
        return false;
    }

    minDis = measureTool.Value();

	pnt1 = measureTool.PointOnShape1(1);
	pnt2 = measureTool.PointOnShape2(1);

    return true;
}

bool OCCBasicTools::AngleMeasurement(TopoDS_Shape shape1, TopoDS_Shape shape2, double& angle)
{
    if (shape1.ShapeType() == TopAbs_EDGE && shape2.ShapeType() == TopAbs_EDGE)
    {
        Handle(Geom_Curve) curve1, curve2;
        double first1, last1, first2, last2;
        curve1 = BRep_Tool::Curve(TopoDS::Edge(shape1), first1, last1);
        curve2 = BRep_Tool::Curve(TopoDS::Edge(shape2), first2, last2);

		if (curve1->IsKind(STANDARD_TYPE(Geom_Line)) && curve2->IsKind(STANDARD_TYPE(Geom_Line)))
		{
			Handle(Geom_Line) line1 = Handle(Geom_Line)::DownCast(curve1);
			Handle(Geom_Line) line2 = Handle(Geom_Line)::DownCast(curve2);
			angle = line1->Lin().Angle(line2->Lin());
		}
		else
		{
			double distance;
			BRepExtrema_DistShapeShape measureTool;
			measureTool.LoadS1(shape1);
			measureTool.LoadS2(shape2);
			measureTool.Perform();

			if (!measureTool.IsDone())
			{
				return false;
			}

			distance = measureTool.Value();
			if (distance > Precision::Confusion())
			{
				return false;
			}

			double U1, U2;
			try
			{
				measureTool.ParOnEdgeS1(1, U1);
				measureTool.ParOnEdgeS2(1, U2);
			}
			catch (...)
			{
				return false;
			}

			gp_Pnt p1, p2;
			gp_Vec vec1, vec2;
			curve1->D1(U1, p1, vec1);
			curve2->D1(U2, p2, vec2);

			angle = vec1.Angle(vec2);
		}
// 
//         GeomAPI_ExtremaCurveCurve extCC;
//         extCC.Init(curve1, curve2);
//         
//         //collection
//         int numPnt = extCC.NbExtrema();
//         if (numPnt < 1)
//         {
//             return false;
//         }
//         double distance = extCC.Distance(1);
//         if (distance > Precision::Confusion())
//         {
//             return false;
//         }
//         extCC.Parameters(1, U1, U2);
// 
//         gp_Pnt p1, p2;
//         gp_Vec vec1, vec2;
//         curve1->D1(U1, p1, vec1);
//         curve2->D1(U2, p2, vec2);
// 
//         angle = vec1.Angle(vec2);

        return true;
    }
    else if (shape1.ShapeType() == TopAbs_FACE && shape2.ShapeType() == TopAbs_FACE)
    {
		gp_Pln plane1, plane2;
		if (!ShapeUtilities::isShapePlannar(shape1, plane1) || !ShapeUtilities::isShapePlannar(shape2, plane2))
		{
			return false;
		}

//         Handle(Geom_Plane) plane1, plane2;
//         plane1 = ShapeUtilities::findPlane(shape1);
//         plane2 = ShapeUtilities::findPlane(shape2);
//         if (plane1.IsNull() || plane2.IsNull())
//         {
//             return false;
//         }

        angle = plane1.Axis().Angle(plane2.Axis());

        return true;
    }
    else if ((shape1.ShapeType() == TopAbs_EDGE && shape2.ShapeType() == TopAbs_FACE)|| (shape1.ShapeType() == TopAbs_FACE && shape2.ShapeType() == TopAbs_EDGE))
    {
        Handle(Geom_Surface) surf;
        Handle(Geom_Curve) curve;
		TopoDS_Shape face, edge;
        double first, last;
        if (shape1.ShapeType() == TopAbs_EDGE)
        {
            surf = BRep_Tool::Surface(TopoDS::Face(shape2));
			face = shape2;
            curve = BRep_Tool::Curve(TopoDS::Edge(shape1), first, last);
			edge = shape1;
        }
        else
        {
            surf = BRep_Tool::Surface(TopoDS::Face(shape1));
            curve = BRep_Tool::Curve(TopoDS::Edge(shape2), first, last);
			face = shape1;
			edge = shape2;
        }

        //get surface
        if (surf.IsNull() || curve.IsNull())
        {
            return false;
        }

		double distance;
		BRepExtrema_DistShapeShape measureTool;
		try
		{
			measureTool.LoadS1(face);
			measureTool.LoadS2(edge);
		}
		catch (...)
		{
			return false;
		}
		measureTool.Perform();

		if (!measureTool.IsDone())
		{
			return false;
		}

		distance = measureTool.Value();
		if (distance > Precision::Confusion())
		{
			return false;
		}

		double U1,V1, U2;
		measureTool.ParOnFaceS1(1, U1, V1);
		measureTool.ParOnEdgeS2(1, U2);

		gp_Pnt p1, p2;
		gp_Vec vec1, vecU, vecV;
		curve->D1(U2, p1, vec1);
		surf->D1(U1, V1, p2, vecU, vecV);
		gp_Vec norm = vecU.Crossed(vecV);
		angle = M_PI_2 - norm.Angle(vec1);

        return true;
    }
    else
    {
        return false;
    }
}


bool OCCBasicTools::GetBndBoxParams(TopoDS_Shape shape, double& theXmin, double& theYmin, double& theZmin, double& theXmax, double& theYmax, double& theZmax)
{
    if (shape.IsNull())
    {
        return false;
    }


    Bnd_Box box;
	BRepBndLib::Add(shape, box, Standard_True);
    box.Get(theXmin, theYmin, theZmin, theXmax, theYmax, theZmax);

    return true;
}

bool OCCBasicTools::GetBndBoxParams(TopTools_ListOfShape shapes, double& theXmin, double& theYmin, double& theZmin, double& theXmax, double& theYmax, double& theZmax)
{
	if (shapes.Size()==0)
	{
		return false;
	}


	Bnd_Box box;
	TopTools_ListOfShape::Iterator iter(shapes);
	for (; iter.More(); iter.Next())
	{
		BRepBndLib::Add(iter.Value() , box, Standard_True);
	}
	box.Get(theXmin, theYmin, theZmin, theXmax, theYmax, theZmax);

	return true;
}

bool OCCBasicTools::GetEdgeOrFaceGradients(TopoDS_Shape shape, const std::vector<double>& inParams, const int& level, std::vector<double>& pos, std::vector<double>& gradients)
{
    if (level == 0)
    {
        return false;
    }

    if (shape.ShapeType() == TopAbs_EDGE)
    {
        if (inParams.size() != 1)
        {
            return false;
        }
        double U = inParams.at(0);

        try
        {
            double first, last;
            Handle(Geom_Curve) curve = BRep_Tool::Curve(TopoDS::Edge(shape), first, last);

            gp_Vec vec;
            vec = curve->DN(U, level);
            gradients.push_back(vec.X());
            gradients.push_back(vec.Y());
            gradients.push_back(vec.Z());

            gp_Pnt pnt;
            curve->D0(U, pnt);
            pos.push_back(pnt.X());
            pos.push_back(pnt.Y());
            pos.push_back(pnt.Z());

            return true;
        }
        catch (...)
        {
            return false;
        }
    }
    else if (shape.ShapeType() == TopAbs_FACE)
    {
        if (inParams.size() != 2)
        {
            return false;
        }
        double U = inParams.at(0);
        double V = inParams.at(1);
        try
        {
            Handle(Geom_Surface) surf = BRep_Tool::Surface(TopoDS::Face(shape));

            gp_Vec vec;
            vec = surf->DN(U, V, level, level);
            gradients.push_back(vec.X());
            gradients.push_back(vec.Y());
            gradients.push_back(vec.Z());

            gp_Pnt pnt;
            surf->D0(U, V, pnt);
            pos.push_back(pnt.X());
            pos.push_back(pnt.Y());
            pos.push_back(pnt.Z());
            return true;
        }
        catch (...)
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

bool OCCBasicTools::GetShapeVolume(TopoDS_Shape shape, double& volume)
{
    try
    {
        GProp_GProps gprops;
        BRepGProp::VolumeProperties(shape, gprops);
        volume = gprops.Mass();

		return true;
    }
    catch (...)
    {
        return false;
    }
}

bool OCCBasicTools::createCompound(TopTools_ListOfShape shapes, TopoDS_Shape& target)
{
	try
	{
		TopoDS_Compound cmp;
		BRep_Builder shellBuilder;
		shellBuilder.MakeCompound(cmp);
		TopTools_ListOfShape::Iterator iter(shapes);
		for (; iter.More(); iter.Next())
		{
			shellBuilder.Add(cmp, iter.Value());
		}

		if (cmp.IsNull())
		{
			return false;
		}

		target = cmp;

		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool OCCBasicTools::calcCurveCurvature(TopoDS_Shape shape, const double& T, double& curvature, gp_Pnt& centerOfCurvature)
{
	try
	{
		if (shape.ShapeType() != TopAbs_EDGE)
		{
			return false;
		}

		double start, end;
		Handle(Geom_Curve) curve = BRep_Tool::Curve(TopoDS::Edge(shape), start, end);
		GeomLProp_CLProps prop(curve, T, 2, Precision::Confusion());
		curvature = prop.Curvature();
		prop.CentreOfCurvature(centerOfCurvature);
		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool OCCBasicTools::calcSurfCurvature(TopoDS_Shape shape, const double& U, const double& V, double& maxCurvature)
{
	try
	{
		if (shape.ShapeType() != TopAbs_FACE)
		{
			return false;
		}

		Handle(Geom_Surface) surf = BRep_Tool::Surface(TopoDS::Face(shape));
		GeomLProp_SLProps prop(surf, U, V, 2, Precision::Confusion());
		maxCurvature = prop.MaxCurvature();
		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool OCCBasicTools::DraftFacesOnModel(TopoDS_Shape& muttershape, TopTools_ListOfShape facestodraft, double draftangle, gp_Pln Neutral, gp_Dir pullingdiraction)
{
	BRepOffsetAPI_DraftAngle theDraft(muttershape);
	for (auto iter : facestodraft)
	{
		TopoDS_Face todo_face = TopoDS::Face(iter);
		theDraft.Add(todo_face, pullingdiraction, draftangle, Neutral);
	}
	theDraft.Build();
	muttershape = theDraft.Shape();
}

bool OCCBasicTools::AutoBuildSolidByFaces(TopTools_ListOfShape facelist, TopoDS_Shape& buildsolid)
{
	//补全原来自由边空洞
	std::vector<TopoDS_Shape> edges, neededges;
	for (auto iter : facelist)
	{
		TopExp_Explorer edgeex;
		for (edgeex.Init(iter, TopAbs_EDGE); edgeex.More(); edgeex.Next())
		{
			edges.push_back(edgeex.Current());
		}
	}
	for (int i = 0; i < edges.size(); i++)
	{
		int count = 0;
		for (int j = 0; j < edges.size(); j++)
		{
			if (ShapeUtilities::IsShapeGeomSame(edges[i], edges[j], TopAbs_EDGE))
				count++;
		}
		if (count == 1)
			neededges.push_back(edges[i]);
	}
	BRepFill_Filling bff;
	for (int i = 0; i < neededges.size(); i++)
	{
		bff.Add(TopoDS::Edge(neededges[i]), GeomAbs_C0);
	}
	bff.Build();
	if (!bff.IsDone())
		return false;
	TopoDS_Face needface = bff.Face();
	facelist.Append(needface);

	//补体
	BOPAlgo_MakerVolume bmv;
	bmv.SetArguments(facelist);
	bmv.SetGlue(BOPAlgo_GlueFull);
	bmv.Perform();

	if(bmv.HasErrors())
		return false;

	buildsolid = bmv.Shape();
	return true;
}

//分离step文件各特征方法 start
bool OCCBasicTools::STPSeperateBodies(TopoDS_Shape input, TopTools_ListOfShape featurelists)
{
	//存一下所有面
	TopTools_ListOfShape allfaces;
	TopExp_Explorer faceex;
	for (faceex.Init(input, TopAbs_FACE); faceex.More(); faceex.Next())
	{
		allfaces.Append(faceex.Current())
	}
	//存放各个独立体的面
	std::vector<std::vector<TopoDS_Face>> independentbodyfaces;
	GetIndependentFeatureFacesAndStore(input, allfaces, independentbodyfaces);
}

bool OCCBasicTools::GetIndependentFeatureFacesAndStore(TopoDS_Shape shape, TopTools_ListOfShape& facepackage, std::vector<std::vector<TopoDS_Face>>& independentbodyfaces)
{
	TopTools_ListOfShape featurefaces;
	bool goahead = true;
	LoopOperate(shape, featurefaces, goahead, facepackage);
}

bool OCCBasicTools::LoopOperate(TopoDS_Shape shape, TopTools_ListOfShape& featurefaces, bool& goahead, TopTools_ListOfShape& facepackage)
{
	TopTools_ListOfShape reffeaturefaces;
	for (auto iter : featurefaces)
	{
		BRepAdaptor_Surface brs(TopoDS::Face(iter));
		double uf = brs.FirstUParameter();
		double vf = brs.FirstVParameter();
		double ue = brs.LastUParameter();
		double ve = brs.LastVParameter();
		gp_Dir dir;
		gp_Pnt mid = brs.Value((ue + uf) / 2, (ve + vf) / 2);
		if (!ShapeUtilities::FaceNormal(TopoDS::Face(iter), (ue + uf) / 2, (ve + vf) / 2, dir))
			continue;
		for (auto iter2 : facepackage)
		{
			if (iter2.IsSame(iter))
				continue;
			TopoDS_Shape common = BRepAlgoAPI_Common(iter, iter2);
			if (!common.IsNull())
			{
				BRepAdaptor_Surface xbrs(TopoDS::Face(iter2));
				double xuf = brs.FirstUParameter();
				double xvf = brs.FirstVParameter();
				double xue = brs.LastUParameter();
				double xve = brs.LastVParameter();
				gp_Dir xdir;
				if (!ShapeUtilities::FaceNormal(TopoDS::Face(iter2), (xue + xuf) / 2, (xve + xvf) / 2, xdir))
					continue;
				gp_Dir newdir = xdir.Crossed(dir);

				gp_Pnt xmid = brs.Value((xue + xuf) / 2, (xve + xvf) / 2);
			}
		}
	}
}
//分离step文件各特征方法 end

bool OCCBasicTools::GetArcTopologyCenter(TopoDS_Edge edge, gp_Pnt center)
{
	double first, end;
	Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, first, end);
	Handle(Geom_Circle) boundedCurve = Handle(Geom_Circle)::DownCast(curve);
	if (!boundedCurve)
		return false;
	center = boundedCurve->Location();
	return true;
}

bool OCCBasicTools::RemoveHolesWithinAreaAndDetectFaces(TopoDS_Shape muttershape, TopoDS_Shape& output, TopTools_ListOfShape& facelist, double inputarea)
{
	ShapeUpgrade_RemoveInternalWires sri(muttershape);
	sri.RemoveFaceMode() = true;
	sri.MinArea() = inputarea;
	sri.Perform();
	ShapeExtend_Status ses;
	if (sri.Status(ses) && ses <= ShapeExtend_DONE)
	{
		output = sri.GetResult();
		TopTools_SequenceOfShape faces = sri.RemovedFaces();
		for (auto iter : faces)
		{
			facelist.Append(iter);
		}
		return true;
	}
	return false;
}

bool OCCBasicTools::ComputeNURBSKnotsMults(std::vector<double> src, TColStd_Array1OfReal knots, TColStd_Array1OfInteger mults)
{
	std::map<double, int> dst;
	for (int i = 0; i < src.size(); i++)
	{
		double checkstr = src[i];
		int flag = 0;
		for (int j = 0; j < i; j++)
		{
			if (checkstr == src[j])
			{
				flag = 1;
				break;
			}
		}
		if (!flag)
		{
			int count = 1;
			for (int j = i + 1; j < src.size(); j++)
			{
				if (checkstr == src[j])
					count++;
			}
			dst.insert(std::pair<double, int>(checkstr, count));
		}
	}
	int size = dst.size();
	knots = TColStd_Array1OfReal(1, size);
	mults = TColStd_Array1OfInteger(1, size);

	std::map<double, int>::iterator iter = dst.begin();
	for (int i = 0; iter != dst.end(), i < size; iter++, i++)
	{
		knots.SetValue(i + 1, iter->first);
		mults.SetValue(i + 1, iter->second);
	}
	return true;
}

bool OCCBasicTools::DetectSliverFaces(TopoDS_Shape ent, double tolerance, std::vector<TopoDS_Shape>& sliverfaces)
{
	try
	{
		TopExp_Explorer ex(ent, TopAbs_FACE);
		for (; ex.More(); ex.Next())
		{
			TopoDS_Face myface = TopoDS::Face((ex.Current()));
			std::vector<TopoDS_Edge> edgelist;
			TopExp_Explorer exx;
			for (exx.Init(myface, TopAbs_EDGE); exx.More(); exx.Next())
			{
				TopoDS_Edge xxedge = TopoDS::Edge((exx.Current()));
				GProp_GProps gg;
				BRepGProp::LinearProperties(xxedge, gg);
				double area = gg.Mass();
				if (area <= tolerance)
				{
					edgelist.push_back(xxedge);
				}
			}
			if (edgelist.size() > 3)
				return true;
			bool ifadd = false;
			if (edgelist.size() == 1)
				ifadd = true;
			else if (edgelist.size() == 2)
			{
				TopoDS_Edge edge1 = edgelist[0];
				TopoDS_Edge edge2 = edgelist[1];
				BRepAdaptor_Curve cv1(edge1);
				gp_Pnt pnt11 = cv1.Value(cv1.FirstParameter());
				gp_Pnt pnt12 = cv1.Value(cv1.LastParameter());
				BRepAdaptor_Curve cv2(edge2);
				gp_Pnt pnt21 = cv2.Value(cv2.FirstParameter());
				gp_Pnt pnt22 = cv2.Value(cv2.LastParameter());
				double distance1 = pnt11.Distance(pnt21);
				double distance2 = pnt12.Distance(pnt22);
				if (distance1 <= tolerance && distance2 <= tolerance)
					ifadd = true;
			}
			else if (edgelist.size() == 3)
			{
				TopoDS_Edge edge1 = edgelist[0];
				TopoDS_Edge edge2 = edgelist[1];
				TopoDS_Edge edge3 = edgelist[2];
				BRepAdaptor_Curve cv1(edge1);
				gp_Pnt pnt11 = cv1.Value(cv1.FirstParameter());
				gp_Pnt pnt12 = cv1.Value(cv1.LastParameter());
				BRepAdaptor_Curve cv2(edge2);
				gp_Pnt pnt21 = cv2.Value(cv2.FirstParameter());
				gp_Pnt pnt22 = cv2.Value(cv2.LastParameter());
				BRepAdaptor_Curve cv3(edge3);
				gp_Pnt pnt31 = cv3.Value(cv3.FirstParameter());
				gp_Pnt pnt32 = cv3.Value(cv3.LastParameter());
				double distance1 = pnt11.Distance(pnt21);
				double distance2 = pnt12.Distance(pnt22);
				double distance3 = pnt11.Distance(pnt31);
				double distance4 = pnt12.Distance(pnt32);
				double distance5 = pnt21.Distance(pnt31);
				double distance6 = pnt22.Distance(pnt32);
				if (distance1 <= tolerance &&
					distance2 <= tolerance &&
					distance3 <= tolerance &&
					distance4 <= tolerance &&
					distance5 <= tolerance &&
					distance6 <= tolerance&&)
					ifadd = true;
			}
			if (ifadd)
				sliverfaces.push_back(myface);
		}
		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool OCCBasicTools::RemoveFace(TopoDS_Shape mutter_3d, TopTools_ListOfShape facestoremove, TopoDS_Shape& result)
{
	if (facestoremove.Size() == 0)
	{
		result = mutter_3d;
		return true;
	}

	BRepAlgoAPI_Defeaturing adf;
	adf.SetShape(mutter_3d);
	adf.AddFacesToRemove(facestoremove);
	adf.SetRunParallel(true);
	adf.SetToFillHistory(false);
	adf.Build();

	if (!adf.IsDone())
		return false;

	result = adf.Shape();
	return true;
}