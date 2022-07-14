#include "TransformShapeBuilder.h"
#include "SolidShapeBuilder.h"

#include <BRepAdaptor_Surface.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRepBuilderAPI_GTransform.hxx>
#include <gp_GTrsf.hxx>
#include <BRepBuilderAPI_Copy.hxx>

#define PI 3.1415926

TransformShapeBuilder::TransformShapeBuilder()
{
}


TransformShapeBuilder::~TransformShapeBuilder()
{
}

bool TransformShapeBuilder::shapeTranslate(TopoDS_Shape origShape, const double& vectorX, const double& vectorY, const double& vectorZ, const bool& isCopy)
{
    try
    {
        gp_Trsf theTransformation;
        theTransformation.SetTranslation(gp_Vec(vectorX,vectorY,vectorZ));

		if (m_builder)
		{
			delete m_builder;
		}
        m_builder = new BRepBuilderAPI_Transform(origShape, theTransformation, isCopy);

        m_builder->Build();
        if (!m_builder->IsDone())
        {
            return false;
        }

        setShape(m_builder->Shape());

        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool TransformShapeBuilder::shapeRotate(TopoDS_Shape origShape, const double& axisPosX, const double& axisPosY, const double& axisPosZ, const double& axisVecX, const double& axisVecY, const double& axisVecZ, const double& angle, const bool& isCopy)
{
    try
    {
        gp_Ax1 theA1(gp_Pnt(axisPosX, axisPosY, axisPosZ), gp_Dir(axisVecX, axisVecY, axisVecZ));
        gp_Trsf theTransformation;
        theTransformation.SetRotation(theA1, angle*PI/180.);

		if (m_builder)
		{
			delete m_builder;
		}
		m_builder = new BRepBuilderAPI_Transform(origShape, theTransformation, isCopy);

        m_builder->Build();
        if (!m_builder->IsDone())
        {
            return false;
        }

        setShape(m_builder->Shape());

        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool TransformShapeBuilder::shapeScale(TopoDS_Shape origShape, const double& basePosX, const double& basePosY, const double& basePosZ, const double& scaleX, const double& scaleY, const double& scaleZ, const bool& isCopy)
{
    try
    {
        //平移到原点
        double translateX = -basePosX;
        double translateY = -basePosY;
        double translateZ = -basePosZ;

        if (shapeTranslate(origShape, translateX, translateY, translateZ, isCopy))
        {
            origShape = m_shape;
        }
        else
        {
            return false;
        }

//         gp_Trsf T;
//         T.SetScale(gp_Pnt(0, 0, 0), 0.5);
//         T.SetValues(scaleX, 0, 0, 0, 
//             0, scaleY, 0, 0, 
//             0, 0, scaleZ, 0);
//         m_builder = new BRepBuilderAPI_Transform(T);
//         BRepBuilderAPI_Transform* builder = dynamic_cast<BRepBuilderAPI_Transform*>(m_builder);
//         builder->Perform(origShape, false);

        //TODO, shape scaled by gp_GTrsf can't be filleted
        gp_Trsf T;
        gp_GTrsf GT(T);
        gp_Mat rot(scaleX, 0, 0, 0, scaleY, 0, 0, 0, scaleZ);
        GT.SetVectorialPart(rot);

		if (m_builder)
		{
			delete m_builder;
		}
		m_builder = new BRepBuilderAPI_GTransform(origShape, GT, false);
        m_builder->Build();


        if (!m_builder->IsDone())
        {
            return false;
        }
        origShape = m_builder->Shape();

        //平移回去
        translateX = basePosX;
        translateY = basePosY;
        translateZ = basePosZ;

        return shapeTranslate(origShape, translateX, translateY, translateZ, false);
    }
    catch (...)
    {
        return false;
    }
}

bool TransformShapeBuilder::shapeScale(TopoDS_Shape origShape, double* wcsMatrix, const double& basePosX, const double& basePosY, const double& basePosZ, const double& scaleX, const double& scaleY, const double& scaleZ, const bool& isCopy)
{
    try
    {
        //平移到原点
        double translateX = -basePosX;
        double translateY = -basePosY;
        double translateZ = -basePosZ;

        if (shapeTranslate(origShape, translateX, translateY, translateZ, isCopy))
        {
            origShape = m_shape;
        }
        else
        {
            return false;
        }

//         gp_Trsf T;
//         T.SetScale(gp_Pnt(0, 0, 0), 0.5);
//         T.SetValues(scaleX, 0, 0, 0, 
//             0, scaleY, 0, 0, 
//             0, 0, scaleZ, 0);
//         m_builder = new BRepBuilderAPI_Transform(T);
//         BRepBuilderAPI_Transform* builder = dynamic_cast<BRepBuilderAPI_Transform*>(m_builder);
//         builder->Perform(origShape, false);

        //TODO, shape scaled by gp_GTrsf can't be filleted in 2D form
        gp_Trsf T;
        gp_GTrsf GT(T);
        gp_Mat rot(scaleX, 0, 0, 0, scaleY, 0, 0, 0, scaleZ);
        GT.SetVectorialPart(rot);

		if (m_builder)
		{
			delete m_builder;
		}
		m_builder = new BRepBuilderAPI_GTransform(origShape, GT, false);
        m_builder->Build();

        m_builder->Build();
        if (!m_builder->IsDone())
        {
            return false;
        }
        origShape = m_builder->Shape();

        //平移回去
        translateX = basePosX;
        translateY = basePosY;
        translateZ = basePosZ;

        return shapeTranslate(origShape, translateX, translateY, translateZ, false);
    }
    catch (...)
    {
        return false;
    }
}

bool TransformShapeBuilder::shapeScale(TopoDS_Shape origShape, const double& basePosX, const double& basePosY, const double& basePosZ, const double& scale, const bool& isCopy)
{
	try
	{
		gp_Trsf theTransformation;
		theTransformation.SetScale(gp_Pnt(basePosX, basePosY, basePosZ), scale);

		if (m_builder)
		{
			delete m_builder;
		}
		m_builder = new BRepBuilderAPI_Transform(origShape, theTransformation, isCopy);

		m_builder->Build();
		if (!m_builder->IsDone())
		{
			return false;
		}

		setShape(m_builder->Shape());

		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool TransformShapeBuilder::shapeMirror(TopoDS_Shape origShape, const double& basePosX, const double& basePosY, const double& basePosZ, const double& normalVecX, const double& normalVecY, const double& normalVecZ, const bool& isCopy)
{
    try
    {
        gp_Dir xAxis(1,0,0),zAxis(normalVecX, normalVecY, normalVecZ);
//         if (xAxis.IsParallel(zAxis, 1e-4))
//         {
//             xAxis.SetXYZ(gp_XYZ(0, 1, 0));
//         }
//         yAxis = zAxis.Crossed(xAxis);

        gp_Trsf theTransformation;
        theTransformation.SetMirror(gp_Ax2(gp_Pnt(basePosX, basePosY, basePosZ), zAxis));

		if (m_builder)
		{
			delete m_builder;
		}
		m_builder = new BRepBuilderAPI_Transform(origShape, theTransformation, isCopy);

        m_builder->Build();
        if (!m_builder->IsDone())
        {
            return false;
        }

        setShape(m_builder->Shape());

        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool TransformShapeBuilder::shapeCopy(TopoDS_Shape origShape)
{
    try
    {
		if (m_builder)
		{
			delete m_builder;
		}
        m_builder = new BRepBuilderAPI_Copy(origShape);

        m_builder->Build();
        if (!m_builder->IsDone())
        {
            return false;
        }

        setShape(m_builder->Shape());

        return true;
    }
    catch (...)
    {
        return false;
    }
}
