/*! \file EnumAndStruct.h
*   \brief 用于放置各类枚举和结构体定义，仅OCC动态库部分
*   \author HST
*   \date 2020.10.8
*/

#ifndef ENUMANDSTRUCT_H
#define ENUMANDSTRUCT_H

#include "xdModelingHeader.h"
#include <TopoDS_Shape.hxx>
#include <TDF_Label.hxx>

/*! \enum EnShapeModifiedType
* 模型变换过程中，拓扑有三种方式，不变、编辑和生成
*/
enum EnShapeModifiedType
{
    EnNotChanged,
    EnModified,
    EnGenerated,
};

/*! \brief 用于记录Label查找的结构体
*/
struct LabelInfo
{
	TDF_Label   label; ///查找到的label
	int         calledTimes = -1; ///查找到的label调用的次数
    EnShapeModifiedType         shapeHistory = EnNotChanged;
};

/*! \enum EnAdapatType
* 模型变换的类型，包括复制、拉伸、拔模拉伸、旋转、扫略、放样、布尔减、布尔交、布尔并、倒圆角、二维倒角、不规则布尔运算等.
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
	EnSew1,//缝合成shell
	EnSew2,//缝合成solid
	EnFillet,
    EnChamfer,
    EnFillet2D,
};

/*! \enum EnFixType
* 模型修复的类型.
*/
enum XDMODELING_API EnFixType
{
	EnFixWhole,
	EnFixOrder,
	EnFixEdgeConnect,
	EnFixFaceConnect,
};


#endif