// Created on: 1993-03-10
// Created by: JCV
// Copyright (c) 1993-1999 Matra Datavision
// Copyright (c) 1999-2014 OPEN CASCADE SAS
//
// This file is part of Open CASCADE Technology software library.
//
// This library is free software; you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License version 2.1 as published
// by the Free Software Foundation, with special exception defined in the file
// OCCT_LGPL_EXCEPTION.txt. Consult the file LICENSE_LGPL_21.txt included in OCCT
// distribution for complete text of the license and disclaimer of any warranty.
//
// Alternatively, this file may be used under the terms of Open CASCADE
// commercial license or contractual agreement.

#include "Geom_BoundedMathCurve.hxx"

#include <string>

#include <Geom_Curve.hxx>
#include <Geom_BoundedMathSurface.hxx>
#include <Geom_UndefinedDerivative.hxx>
#include <Geom_UndefinedValue.hxx>
#include <gp_GTrsf2d.hxx>
#include <gp_Pnt.hxx>
#include <gp_Trsf.hxx>
#include <gp_Vec.hxx>
#include <Standard_NoSuchObject.hxx>
#include <Standard_RangeError.hxx>
#include <Standard_Type.hxx>

std::string replace_str(std::string str, const std::string& to_replaced, const std::string& newchars)
{
	std::string targetStr = str;
	for (std::string::size_type pos(0); pos != std::string::npos; pos += newchars.length())
	{
		pos = targetStr.find(to_replaced, pos);
		if (pos != std::string::npos)
			targetStr.replace(pos, to_replaced.length(), newchars);
		else
			break;
	}
	return   targetStr;
}


IMPLEMENT_STANDARD_RTTIEXT(Geom_BoundedMathSurface,Geom_BoundedSurface)

Standard_EXPORT Geom_BoundedMathSurface::Geom_BoundedMathSurface(const std::string xFuncExpress, const std::string yFuncExpress, 
	const std::string zFuncExpress, const std::string UName, 
	const std::string VName, const Standard_Real startU, 
	const Standard_Real endU, const Standard_Real startV, const Standard_Real endV)
	:m_xFuncExpress(xFuncExpress), m_yFuncExpress(yFuncExpress), m_zFuncExpress(zFuncExpress),
	m_varUName(UName), m_varVName(VName), m_startU(startU), m_endU(endU), m_startV(startV), m_endV(endV)
{
	m_reverseUSymbol = Standard_False;
	m_reverseVSymbol = Standard_False;

	m_smooth = GeomAbs_C1;

	//build function
	Standard_Real U,V;	
	if (!m_symbolTable.add_variable(m_varUName, U)|| !m_symbolTable.add_variable(m_varVName, V))
	{
		return;
	}

	m_symbolTable.add_constants();

	m_xExpression.register_symbol_table(m_symbolTable);
	m_yExpression.register_symbol_table(m_symbolTable);
	m_zExpression.register_symbol_table(m_symbolTable);

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


	if (m_startV > m_endV)
	{
		double swap = m_startV;
		m_startV = m_endV;
		m_endV = m_startV;
	}

// 	//only U reverse
// 	Standard_Real U2, V2;
// 	m_varUName_UrevV = m_varUName + "rev";
// 	m_varVName_UrevV = m_varVName;
// 	std::string revFuncExpress = "(" + std::to_string(m_endU) + "+" + std::to_string(m_startU) + "-" + m_varUName_UrevV + ")";
// 	m_yFuncExpress_UrevV = replace_str(m_yFuncExpress, m_varUName, revFuncExpress);
// 	m_yFuncExpress_UrevV = replace_str(m_yFuncExpress, m_varUName, revFuncExpress);
// 	m_yFuncExpress_UrevV = replace_str(m_yFuncExpress, m_varUName, revFuncExpress);
// 	if (!m_symbolTable_UrevV.add_variable(m_varUName_UrevV, U2) || !m_symbolTable_UrevV.add_variable(m_varVName_UrevV, V2))
// 	{
// 		return;
// 	}
// 
// 	m_symbolTable_UrevV.add_constants();
// 
// 	m_xExpression_UrevV.register_symbol_table(m_symbolTable_UrevV);
// 	m_yExpression_UrevV.register_symbol_table(m_symbolTable_UrevV);
// 	m_zExpression_UrevV.register_symbol_table(m_symbolTable_UrevV);
// 
// 	if (!m_parser.compile(m_xFuncExpress_UrevV, m_xExpression_UrevV)
// 		|| !m_parser.compile(m_yFuncExpress_UrevV, m_yExpression_UrevV)
// 		|| !m_parser.compile(m_zFuncExpress_UrevV, m_zExpression_UrevV))
// 	{
// 		return;
// 	}
// 
// 	//only V reverse
// 	Standard_Real U3, V3;
// 	m_varUName_UVrev = m_varUName;
// 	m_varVName_UVrev = m_varVName + "rev";
// 	revFuncExpress = "(" + std::to_string(m_endV) + "+" + std::to_string(m_startV) + "-" + m_varVName_UVrev + ")";
// 	m_yFuncExpress_UVrev = replace_str(m_yFuncExpress, m_varVName, revFuncExpress);
// 	m_yFuncExpress_UVrev = replace_str(m_yFuncExpress, m_varVName, revFuncExpress);
// 	m_yFuncExpress_UVrev = replace_str(m_yFuncExpress, m_varVName, revFuncExpress);
// 	if (!m_symbolTable_UVrev.add_variable(m_varUName_UVrev, U3) || !m_symbolTable_UVrev.add_variable(m_varVName_UVrev, V3))
// 	{
// 		return;
// 	}
// 
// 	m_symbolTable_UVrev.add_constants();
// 
// 	m_xExpression_UVrev.register_symbol_table(m_symbolTable_UVrev);
// 	m_yExpression_UVrev.register_symbol_table(m_symbolTable_UVrev);
// 	m_zExpression_UVrev.register_symbol_table(m_symbolTable_UVrev);
// 
// 	if (!m_parser.compile(m_xFuncExpress_UVrev, m_xExpression_UVrev)
// 		|| !m_parser.compile(m_yFuncExpress_UVrev, m_yExpression_UVrev)
// 		|| !m_parser.compile(m_zFuncExpress_UVrev, m_zExpression_UVrev))
// 	{
// 		return;
// 	}
// 
// 	//double reverse
// 	Standard_Real U4, V4;
// 	m_varUName_doubleRev = m_varUName + "rev";
// 	m_varVName_doubleRev = m_varVName + "rev";
// 	revFuncExpress = "(" + std::to_string(m_endV) + "+" + std::to_string(m_startV) + "-" + m_varVName_doubleRev + ")";
// 	m_yFuncExpress_doubleRev = replace_str(m_yFuncExpress, m_varVName, revFuncExpress);
// 	m_yFuncExpress_doubleRev = replace_str(m_yFuncExpress, m_varVName, revFuncExpress);
// 	m_yFuncExpress_doubleRev = replace_str(m_yFuncExpress, m_varVName, revFuncExpress);
// 	if (!m_symbolTable_doubleRev.add_variable(m_varUName_doubleRev, U4) || !m_symbolTable_doubleRev.add_variable(m_varVName_doubleRev, V4))
// 	{
// 		return;
// 	}
// 
// 	m_symbolTable_doubleRev.add_constants();
// 
// 	m_xExpression_doubleRev.register_symbol_table(m_symbolTable_doubleRev);
// 	m_yExpression_doubleRev.register_symbol_table(m_symbolTable_doubleRev);
// 	m_zExpression_doubleRev.register_symbol_table(m_symbolTable_doubleRev);
// 
// 	if (!m_parser.compile(m_xFuncExpress_doubleRev, m_xExpression_doubleRev)
// 		|| !m_parser.compile(m_yFuncExpress_doubleRev, m_yExpression_doubleRev)
// 		|| !m_parser.compile(m_zFuncExpress_doubleRev, m_zExpression_doubleRev))
// 	{
// 		return;
// 	}
}


Standard_EXPORT void Geom_BoundedMathSurface::UReverse()
{
	m_reverseUSymbol = m_reverseUSymbol ? Standard_False : Standard_True;
}

Standard_EXPORT Standard_Real Geom_BoundedMathSurface::UReversedParameter(const Standard_Real U) const
{
	Standard_Real reversedParameter = m_reverseUSymbol ? m_endU + m_startU - U : U;
	return reversedParameter;
}


Standard_EXPORT void Geom_BoundedMathSurface::VReverse()
{
	m_reverseVSymbol = m_reverseVSymbol ? Standard_False : Standard_True;
}

Standard_EXPORT Standard_Real Geom_BoundedMathSurface::VReversedParameter(const Standard_Real V) const
{
	Standard_Real reversedParameter = m_reverseVSymbol ? m_endV + m_startV - V : V;
	return reversedParameter;
}

Standard_EXPORT void Geom_BoundedMathSurface::Bounds(Standard_Real& U1, Standard_Real& U2, Standard_Real& V1, Standard_Real& V2) const
{
	U1 = m_startU;
	U2 = m_endU;
	V1 = m_startV;
	V2 = m_endV;
}


Standard_EXPORT Standard_Boolean Geom_BoundedMathSurface::IsUClosed() const
{
	return Standard_False;
}


Standard_EXPORT Standard_Boolean Geom_BoundedMathSurface::IsVClosed() const
{
	return Standard_False;
}


Standard_EXPORT Standard_Boolean Geom_BoundedMathSurface::IsUPeriodic() const
{
	return Standard_False;
}


Standard_EXPORT Standard_Boolean Geom_BoundedMathSurface::IsVPeriodic() const
{
	return Standard_False;
}

Handle(Geom_Curve) Geom_BoundedMathSurface::UIso(const Standard_Real U) const
{
	Handle(Geom_BoundedMathCurve) curve;

	std::string isoU = std::to_string(U);
	std::string xFuncExpress = replace_str(m_xFuncExpress, m_varUName, isoU);
	std::string yFuncExpress = replace_str(m_yFuncExpress, m_varUName, isoU);
	std::string zFuncExpress = replace_str(m_zFuncExpress, m_varUName, isoU);

	curve = new Geom_BoundedMathCurve(xFuncExpress, yFuncExpress, zFuncExpress, m_varVName, m_startV, m_endV);

	return curve;
}


Handle(Geom_Curve) Geom_BoundedMathSurface::VIso(const Standard_Real V) const
{
	Handle(Geom_BoundedMathCurve) curve;

	std::string isoV = std::to_string(V);
	std::string xFuncExpress = replace_str(m_xFuncExpress, m_varVName, isoV);
	std::string yFuncExpress = replace_str(m_yFuncExpress, m_varVName, isoV);
	std::string zFuncExpress = replace_str(m_zFuncExpress, m_varVName, isoV);

	curve = new Geom_BoundedMathCurve(xFuncExpress, yFuncExpress, zFuncExpress, m_varUName, m_startU, m_endU);

	return curve;
}


Standard_EXPORT GeomAbs_Shape Geom_BoundedMathSurface::Continuity() const
{
	return m_smooth;
}


Standard_EXPORT Standard_Boolean Geom_BoundedMathSurface::IsCNu(const Standard_Integer N) const
{
	Standard_RangeError_Raise_if(N < 0, " ");
	return N >= 2 ? Standard_False : Standard_True;
}


Standard_EXPORT Standard_Boolean Geom_BoundedMathSurface::IsCNv(const Standard_Integer N) const
{
	Standard_RangeError_Raise_if(N < 0, " ");
	return N >= 2 ? Standard_False : Standard_True;
}


Standard_EXPORT void Geom_BoundedMathSurface::D0(const Standard_Real U, const Standard_Real V, gp_Pnt& P) const
{
	Standard_Real& varU = m_symbolTable.get_variable(m_varUName)->ref();
	Standard_Real& varV = m_symbolTable.get_variable(m_varVName)->ref();
	varU = UReversedParameter(U);
	varV = VReversedParameter(V);

	Standard_Real x = m_xExpression.value();
	Standard_Real y = m_yExpression.value();
	Standard_Real z = m_zExpression.value();

	P.SetX(x);
	P.SetY(y);
	P.SetZ(z);
}


Standard_EXPORT void Geom_BoundedMathSurface::D1(const Standard_Real U, const Standard_Real V, gp_Pnt& P, gp_Vec& D1U, gp_Vec& D1V) const
{
	D0(U, V, P);

	Standard_Real& varU = m_symbolTable.get_variable(m_varUName)->ref();
	Standard_Real& varV = m_symbolTable.get_variable(m_varVName)->ref();
	varU = UReversedParameter(U);
	varV = VReversedParameter(V);

	double mark = m_reverseUSymbol ? -1.0 : 1.0;
	Standard_Real dxdU = exprtk::derivative(m_xExpression, varU, mark*0.00000001);
	Standard_Real dydU = exprtk::derivative(m_yExpression, varU, mark*0.00000001);
	Standard_Real dzdU = exprtk::derivative(m_zExpression, varU, mark*0.00000001);
	D1U.SetX(dxdU);
	D1U.SetY(dydU);
	D1U.SetZ(dzdU);

	mark = m_reverseVSymbol ? -1.0 : 1.0;
	Standard_Real dxdV = exprtk::derivative(m_xExpression, varV, mark*0.00000001);
	Standard_Real dydV = exprtk::derivative(m_yExpression, varV, mark*0.00000001);
	Standard_Real dzdV = exprtk::derivative(m_zExpression, varV, mark*0.00000001);
	D1V.SetX(dxdV);
	D1V.SetY(dydV);
	D1V.SetZ(dzdV);
}


Standard_EXPORT void Geom_BoundedMathSurface::D2(const Standard_Real U, const Standard_Real V, gp_Pnt& P, gp_Vec& D1U, gp_Vec& D1V, gp_Vec& D2U, gp_Vec& D2V, gp_Vec& D2UV) const
{
	D1(U, V, P, D1U, D1V);

	Standard_Real& varU = m_symbolTable.get_variable(m_varUName)->ref();
	Standard_Real& varV = m_symbolTable.get_variable(m_varVName)->ref();
	varU = UReversedParameter(U);
	varV = VReversedParameter(V);

	double mark = m_reverseUSymbol ? -1.0 : 1.0;
	Standard_Real dxdU = exprtk::second_derivative(m_xExpression, varU, mark*0.00001);
	Standard_Real dydU = exprtk::second_derivative(m_yExpression, varU, mark*0.00001);
	Standard_Real dzdU = exprtk::second_derivative(m_zExpression, varU, mark*0.00001);
	D2U.SetX(dxdU);
	D2U.SetY(dydU);
	D2U.SetZ(dzdU);

	mark = m_reverseVSymbol ? -1.0 : 1.0;
	Standard_Real dxdV = exprtk::second_derivative(m_xExpression, varV, mark*0.00001);
	Standard_Real dydV = exprtk::second_derivative(m_yExpression, varV, mark*0.00001);
	Standard_Real dzdV = exprtk::second_derivative(m_zExpression, varV, mark*0.00001);
	D2V.SetX(dxdV);
	D2V.SetY(dydV);
	D2V.SetZ(dzdV);
}


Standard_EXPORT void Geom_BoundedMathSurface::D3(const Standard_Real U, const Standard_Real V, gp_Pnt& P, gp_Vec& D1U, gp_Vec& D1V, gp_Vec& D2U, gp_Vec& D2V, gp_Vec& D2UV, gp_Vec& D3U, gp_Vec& D3V, gp_Vec& D3UUV, gp_Vec& D3UVV) const
{
	D2(U, V, P, D1U, D1V, D2U,D2V, D2UV);
}


Standard_EXPORT gp_Vec Geom_BoundedMathSurface::DN(const Standard_Real U, const Standard_Real V, const Standard_Integer Nu, const Standard_Integer Nv) const
{
	Standard_RangeError_Raise_if(Nu > 0, " ");
	Standard_RangeError_Raise_if(Nv > 0, " ");

// 	gp_Pnt P;
// 	gp_Vec V1, V2, V3;
// 
// 	if (N == 0)
// 	{
// 		D0(U, P);
// 		return Vec(P.XYZ());
// 	}
// 	else if (N == 1)
// 	{
// 		D1(U, P, V1);
// 		return V1;
// 	}
// 	else if (N == 2)
// 	{
// 		D2(U, P, V1, V2);
// 		return V2;
// 	}
// 	else
// 	{
// 		D3(U, P, V1, V2, V3);
// 		return V3;
// 	}

}

//=======================================================================
//function : Value
//purpose  : 
//=======================================================================

gp_Pnt  Geom_BoundedMathSurface::Value(const Standard_Real U, 
			    const Standard_Real V)const 
{
  gp_Pnt P;
  D0(U,V,P);
  return P;
}

Standard_EXPORT void Geom_BoundedMathSurface::Transform(const gp_Trsf& T)
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
}

Handle(Geom_Geometry) Geom_BoundedMathSurface::Copy() const
{
	Handle(Geom_BoundedMathSurface) MC;
	MC = new Geom_BoundedMathSurface(m_xFuncExpress, m_yFuncExpress, m_zFuncExpress, m_varUName, m_varVName, m_startU, m_endU, m_startV, m_endV);
	MC->setReverseUSymbol(m_reverseUSymbol);
	MC->setReverseVSymbol(m_reverseVSymbol);

	return MC;
}

//=======================================================================
//function : strFuncMultiplyValue
//purpose  : 
//=======================================================================
std::string Geom_BoundedMathSurface::strFuncMultiplyValue(const std::string origFunc, const Standard_Real value)
{
	std::string newFunc;
	newFunc = std::to_string(value) + "*(" + origFunc + ")";
	return newFunc;
}