#include "Geom_BoundedMathCurve.hxx"

#include <ElCLib.hxx>
#include <Geom_BezierCurve.hxx>
#include <Geom_BSplineCurve.hxx>
#include <Geom_Circle.hxx>
#include <Geom_Curve.hxx>
#include <Geom_Ellipse.hxx>
#include <Geom_Geometry.hxx>
#include <Geom_Hyperbola.hxx>
#include <Geom_Line.hxx>
#include <Geom_OffsetCurve.hxx>
#include <Geom_Parabola.hxx>
#include <Geom_UndefinedDerivative.hxx>
#include <Geom_UndefinedValue.hxx>
#include <gp.hxx>
#include <gp_Pnt.hxx>
#include <gp_Trsf.hxx>
#include <gp_Vec.hxx>
#include <Precision.hxx>
#include <Standard_ConstructionError.hxx>
#include <Standard_NoSuchObject.hxx>
#include <Standard_RangeError.hxx>
#include <Standard_Type.hxx>

// std::string replace_str(std::string str, const std::string& to_replaced, const std::string& newchars)
// {
//     std::string targetStr = str;
//     for (std::string::size_type pos(0); pos != std::string::npos; pos += newchars.length())
//     {
//         pos = targetStr.find(to_replaced, pos);
//         if (pos != std::string::npos)
//             targetStr.replace(pos, to_replaced.length(), newchars);
//         else
//             break;
//     }
//     return   targetStr;
// }

IMPLEMENT_STANDARD_RTTIEXT(Geom_BoundedMathCurve, Geom_BoundedCurve)

typedef Geom_BoundedMathCurve         TrimmedCurve;
typedef gp_Ax1  Ax1;
typedef gp_Ax2  Ax2;
typedef gp_Pnt  Pnt;
typedef gp_Trsf Trsf;
typedef gp_Vec  Vec;

//=======================================================================
//function : Copy
//purpose  : 
//=======================================================================

Handle(Geom_Geometry) Geom_BoundedMathCurve::Copy() const {
    Handle(Geom_BoundedMathCurve) MC;
    MC = new Geom_BoundedMathCurve(m_xFuncExpress, m_yFuncExpress, m_zFuncExpress, m_varName, m_startU, m_endU);
    MC->setReverseSymbol(m_reverseSymbol);
    return MC;

}

//=======================================================================
//function : Geom_BoundedMathCurve
//purpose  : 
//=======================================================================

Standard_EXPORT Geom_BoundedMathCurve::Geom_BoundedMathCurve(const std::string xFuncExpress, const std::string yFuncExpress, const std::string zFuncExpress, const std::string varName, const Standard_Real startU, const Standard_Real endU)
    :m_xFuncExpress(xFuncExpress), m_yFuncExpress(yFuncExpress), m_zFuncExpress(zFuncExpress),
    m_varName(varName), m_startU(startU), m_endU(endU)
{
    m_reverseSymbol = Standard_False;
    m_smooth = GeomAbs_C0;

    //build function
    Standard_Real U;

    if (!m_symbolTable.add_variable(m_varName, U))
    {
        return;
    }

    m_symbolTable.add_constants();

    m_xExpression.register_symbol_table(m_symbolTable);
    m_yExpression.register_symbol_table(m_symbolTable);
    m_zExpression.register_symbol_table(m_symbolTable);

    if (m_zFuncExpress.empty())
    {
        m_zFuncExpress = "0.0";
    }

    if (!m_parser.compile(m_xFuncExpress, m_xExpression)
        || !m_parser.compile(m_yFuncExpress, m_yExpression)
        || !m_parser.compile(m_zFuncExpress, m_zExpression))
    {
        return;
    }

	if (m_startU > m_endU)
	{
		double swap = m_startU;
		m_startU = m_endU;
		m_endU = m_startU;
	}

//     //reverse function
//     m_revVarName = m_varName + "rev";
//     std::string revFuncExpress = "(" + std::to_string(m_endU) + "+" + std::to_string(m_startU) + "-" + m_revVarName + ")";
//     m_xRevFuncExpress = replace_str(m_xFuncExpress, m_varName, revFuncExpress);
//     m_yRevFuncExpress = replace_str(m_yFuncExpress, m_varName, revFuncExpress);
//     m_zRevFuncExpress = replace_str(m_zFuncExpress, m_varName, revFuncExpress);
//     //build function
//     Standard_Real Urev;
// 
//     if (!m_revSymbolTable.add_variable(m_revVarName, Urev))
//     {
//         return;
//     }
// 
//     m_revSymbolTable.add_constants();
// 
//     m_xRevExpression.register_symbol_table(m_revSymbolTable);
//     m_yRevExpression.register_symbol_table(m_revSymbolTable);
//     m_zRevExpression.register_symbol_table(m_revSymbolTable);
// 
//     if (!m_parser.compile(m_xRevFuncExpress, m_xRevExpression)
//         || !m_parser.compile(m_yRevFuncExpress, m_yRevExpression)
//         || !m_parser.compile(m_zRevFuncExpress, m_zRevExpression))
//     {
//         return;
//     }
}

//=======================================================================
//function : Reverse
//purpose  : 
//=======================================================================

void Geom_BoundedMathCurve::Reverse()
{
    m_reverseSymbol = m_reverseSymbol ? Standard_False : Standard_True;
}


//=======================================================================
//function : ReversedParameter
//purpose  : 
//=======================================================================

Standard_Real Geom_BoundedMathCurve::ReversedParameter
(const Standard_Real U) const
{
    Standard_Real reversedParameter = m_reverseSymbol? m_endU + m_startU - U :U;
    return reversedParameter;
}

//=======================================================================
//function : FirstParameter
//purpose  : 
//=======================================================================

Standard_Real Geom_BoundedMathCurve::FirstParameter() const
{
    return m_startU;
}

//=======================================================================
//function : LastParameter
//purpose  : 
//=======================================================================

Standard_Real Geom_BoundedMathCurve::LastParameter() const
{
    return m_endU;
}


Standard_EXPORT gp_Pnt Geom_BoundedMathCurve::StartPoint() const
{
    Standard_Real U = FirstParameter();
    gp_Pnt P;
    D0(U, P);
    return P;
}

Standard_EXPORT gp_Pnt Geom_BoundedMathCurve::EndPoint() const
{
    Standard_Real U = LastParameter();
    gp_Pnt P;
    D0(U, P);
    return P;
}

//=======================================================================
//function : IsClosed
//purpose  : 
//=======================================================================

Standard_Boolean Geom_BoundedMathCurve::IsClosed() const
{
    return Standard_False;
}

//=======================================================================
//function : IsPeriodic
//purpose  : 
//=======================================================================

Standard_Boolean Geom_BoundedMathCurve::IsPeriodic() const
{
    //return basisCurve->IsPeriodic();
    return Standard_False;
}


//=======================================================================
//function : Continuity
//purpose  : 
//=======================================================================

GeomAbs_Shape Geom_BoundedMathCurve::Continuity() const {

    return m_smooth;
}

//=======================================================================
//function : IsCN
//purpose  : 
//=======================================================================

Standard_Boolean Geom_BoundedMathCurve::IsCN(const Standard_Integer N) const {

    Standard_RangeError_Raise_if(N < 0, " ");
    return N >= 1 ? Standard_False : Standard_True;
}

//=======================================================================
//function : D0
//purpose  : 
//=======================================================================

void Geom_BoundedMathCurve::D0(const Standard_Real U, Pnt& P) const
{
	Standard_Real& var = m_symbolTable.get_variable(m_varName)->ref();
	var = ReversedParameter(U);

	Standard_Real x = m_xExpression.value();
	Standard_Real y = m_yExpression.value();
	Standard_Real z = m_zExpression.value();

	P.SetX(x);
	P.SetY(y);
	P.SetZ(z);
}


//=======================================================================
//function : D1
//purpose  : 
//=======================================================================

void Geom_BoundedMathCurve::D1(const Standard_Real U, Pnt& P, Vec& V1) const
{
    D0(U, P);

	double mark = m_reverseSymbol ? -1.0 : 1.0;

	Standard_Real& var = m_symbolTable.get_variable(m_varName)->ref();
	var = ReversedParameter(U);
	Standard_Real dxdU = exprtk::derivative(m_xExpression, var, mark*0.00000001);
	Standard_Real dydU = exprtk::derivative(m_yExpression, var, mark*0.00000001);
	Standard_Real dzdU = exprtk::derivative(m_zExpression, var, mark*0.00000001);

	V1.SetX(dxdU);
	V1.SetY(dydU);
	V1.SetZ(dzdU);
}


//=======================================================================
//function : D2
//purpose  : 
//=======================================================================

void Geom_BoundedMathCurve::D2(const Standard_Real U,
    Pnt& P, Vec& V1, Vec& V2) const
{
    D1(U, P, V1);

	double mark = m_reverseSymbol ? -1.0 : 1.0;

	Standard_Real& var = m_symbolTable.get_variable(m_varName)->ref();
	var = ReversedParameter(U);
	Standard_Real dxdU = exprtk::second_derivative(m_xExpression, var, mark*0.00001);
	Standard_Real dydU = exprtk::second_derivative(m_yExpression, var, mark*0.00001);
	Standard_Real dzdU = exprtk::second_derivative(m_zExpression, var, mark*0.00001);

	V2.SetX(dxdU);
	V2.SetY(dydU);
	V2.SetZ(dzdU);
}


//=======================================================================
//function : D3
//purpose  : 
//=======================================================================

void Geom_BoundedMathCurve::D3(const Standard_Real U,
    Pnt& P, Vec& V1, Vec& V2, Vec& V3) const
{
    D2(U, P, V1, V2);

	double mark = m_reverseSymbol ? -1.0 : 1.0;

	Standard_Real& var = m_symbolTable.get_variable(m_varName)->ref();
	var = ReversedParameter(U);
	Standard_Real dxdU = exprtk::third_derivative(m_xExpression, var, mark*0.0001);
	Standard_Real dydU = exprtk::third_derivative(m_yExpression, var, mark*0.0001);
	Standard_Real dzdU = exprtk::third_derivative(m_zExpression, var, mark*0.0001);

	V3.SetX(dxdU);
	V3.SetY(dydU);
	V3.SetZ(dzdU);
}


//=======================================================================
//function : DN
//purpose  : 
//=======================================================================

Vec Geom_BoundedMathCurve::DN(const Standard_Real U,
    const Standard_Integer N) const
{
    Standard_RangeError_Raise_if(N > 3, " ");

    gp_Pnt P;
    gp_Vec V1, V2, V3;

    if (N == 0)
    {
        D0(U, P);
        return Vec(P.XYZ());
    }
    else if (N == 1)
    {
        D1(U, P, V1);
        return V1;
    }
    else if (N == 2)
    {
        D2(U, P, V1, V2);
        return V2;
    }
    else
    {
        D3(U, P, V1, V2, V3);
        return V3;
    }
}


//=======================================================================
//function : Transform
//purpose  : 
//=======================================================================

void Geom_BoundedMathCurve::Transform(const Trsf& T)
{
    gp_Mat matirx = T.VectorialPart();

    m_xFuncExpress = strFuncMultiplyValue(m_xFuncExpress, matirx.Value(0, 0)) + strFuncMultiplyValue(m_yFuncExpress, matirx.Value(0, 1)) + strFuncMultiplyValue(m_zFuncExpress, matirx.Value(0, 2));
    m_yFuncExpress = strFuncMultiplyValue(m_xFuncExpress, matirx.Value(1, 0)) + strFuncMultiplyValue(m_yFuncExpress, matirx.Value(1, 1)) + strFuncMultiplyValue(m_zFuncExpress, matirx.Value(1, 2));
    m_zFuncExpress = strFuncMultiplyValue(m_xFuncExpress, matirx.Value(2, 0)) + strFuncMultiplyValue(m_yFuncExpress, matirx.Value(2, 1)) + strFuncMultiplyValue(m_zFuncExpress, matirx.Value(2, 2));

    if (!m_parser.compile(m_xFuncExpress, m_xExpression)
        || !m_parser.compile(m_yFuncExpress, m_yExpression)
        || !m_parser.compile(m_zFuncExpress, m_zExpression))
    {
        return;
    }

//     m_xRevFuncExpress = strFuncMultiplyValue(m_xRevFuncExpress, matirx.Value(0, 0)) + strFuncMultiplyValue(m_yRevFuncExpress, matirx.Value(0, 1)) + strFuncMultiplyValue(m_zRevFuncExpress, matirx.Value(0, 2));
//     m_yRevFuncExpress = strFuncMultiplyValue(m_xRevFuncExpress, matirx.Value(1, 0)) + strFuncMultiplyValue(m_yRevFuncExpress, matirx.Value(1, 1)) + strFuncMultiplyValue(m_zRevFuncExpress, matirx.Value(1, 2));
//     m_zRevFuncExpress = strFuncMultiplyValue(m_xRevFuncExpress, matirx.Value(2, 0)) + strFuncMultiplyValue(m_yRevFuncExpress, matirx.Value(2, 1)) + strFuncMultiplyValue(m_zRevFuncExpress, matirx.Value(2, 2));
// 
//     if (!m_parser.compile(m_xRevFuncExpress, m_xRevExpression)
//         || !m_parser.compile(m_yRevFuncExpress, m_yRevExpression)
//         || !m_parser.compile(m_zRevFuncExpress, m_zRevExpression))
//     {
//         return;
//     }
}


//=======================================================================
//function : strFuncMultiplyValue
//purpose  : 
//=======================================================================
std::string Geom_BoundedMathCurve::strFuncMultiplyValue(const std::string origFunc, const Standard_Real value)
{
    std::string newFunc;
    newFunc = std::to_string(value) + "*(" + origFunc + ")";
    return newFunc;
}