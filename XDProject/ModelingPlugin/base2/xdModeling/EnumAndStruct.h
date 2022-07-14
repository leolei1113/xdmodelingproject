/*! \file EnumAndStruct.h
*   \brief ���ڷ��ø���ö�ٺͽṹ�嶨�壬��OCC��̬�ⲿ��
*   \author HST
*   \date 2020.10.8
*/

#ifndef ENUMANDSTRUCT_H
#define ENUMANDSTRUCT_H

#include "xdModelingHeader.h"
#include <TopoDS_Shape.hxx>
#include <TDF_Label.hxx>

/*! \enum EnShapeModifiedType
* ģ�ͱ任�����У����������ַ�ʽ�����䡢�༭������
*/
enum EnShapeModifiedType
{
    EnNotChanged,
    EnModified,
    EnGenerated,
};

/*! \brief ���ڼ�¼Label���ҵĽṹ��
*/
struct LabelInfo
{
	TDF_Label   label; ///���ҵ���label
	int         calledTimes = -1; ///���ҵ���label���õĴ���
    EnShapeModifiedType         shapeHistory = EnNotChanged;
};

/*! \enum EnAdapatType
* ģ�ͱ任�����ͣ��������ơ����졢��ģ���졢��ת��ɨ�ԡ���������������������������������Բ�ǡ���ά���ǡ������򲼶������.
*/
enum XDMODELING_API EnAdapatType
{
	EnNormal,
    EnCopy,
    EnExtrude,
    EnExtrudeDraft,
    EnRevol,
    EnSweep,
    EnLoft,
    EnThicken,
    EnOffset,
	EnCut,
	EnCommon,
	EnSection,
	EnFuse,
    EnCellsBuilderCut,
    EnCellsBuilderCommon,
    EnCellsBuilderFuse,
	EnDetachEdges,
	EnSew1,//��ϳ�shell
	EnSew2,//��ϳ�solid
	EnFillet,
    EnChamfer,
    EnFillet2D,
};

/*! \enum EnFixType
* ģ���޸�������.
*/
enum XDMODELING_API EnFixType
{
	EnFixWhole,
	EnFixOrder,
	EnFixEdgeConnect,
	EnFixFaceConnect,
};


#endif