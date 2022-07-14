#include "PrimShapeBuilder.h"


PrimShapeBuilder::PrimShapeBuilder()
{
}


PrimShapeBuilder::~PrimShapeBuilder()
{
    if (m_extrude)
    {
        delete m_extrude;
    }

    if (m_revol)
    {
        delete m_revol;
    }
}

bool PrimShapeBuilder::extrudeOperation(TopoDS_Shape shape, const double& xVec, const double& yVec, const double& zVec)
{
    try
    {
        if (m_extrude)
        {
            delete m_extrude;
        }
        m_extrude = new BRepPrimAPI_MakePrism(shape, gp_Vec(xVec, yVec, zVec));
        m_extrude->Build();
        if (!m_extrude->IsDone()) {
            return false;
        }

        setBuilder(m_extrude);

        setShape(m_extrude->Shape());

        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool PrimShapeBuilder::revolOperation(TopoDS_Shape shape, const double& xStart, const double& yStart, const double& zStart, const double& xVec, const double& yVec, const double& zVec, const double& angle /*= 360.0*/)
{
    try
    {
        if (m_revol)
        {
            delete m_revol;
        }
        m_revol = new BRepPrimAPI_MakeRevol(shape, gp_Ax1(gp_Pnt(xStart, yStart, zStart), gp_Vec(xVec, yVec, zVec)), angle);
        m_revol->Build();
        if (!m_revol->IsDone()) {
            return false;
        }

        setBuilder(m_revol);

        setShape(m_revol->Shape());

        return true;
    }
    catch (...)
    {
        return false;
    }
}
