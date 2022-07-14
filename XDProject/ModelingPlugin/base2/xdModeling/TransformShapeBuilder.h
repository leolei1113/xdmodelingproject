/*! \file TransformShapeBuilder.h
*   \brief TransformShapeBuilder���ͷ�ļ�
*   \author HST
*   \date 2020.10.8
*/

#ifndef TRANSFORMSHAPEBUILDER_H
#define TRANSFORMSHAPEBUILDER_H

#include "ShapeOperationBuilder.h"

#include <TopoDS_Wire.hxx>

/*! \class TransformShapeBuilder
*   \brief ���ڶԼ�������б任���࣬����ƽ�ơ���ת�����š�����
*/
class TransformShapeBuilder :
    public ShapeOperationBuilder
{
public:
    /*! \fn TransformShapeBuilder()
    *  \brief TransformShapeBuilder�Ĺ��캯��.
    */
    TransformShapeBuilder();
    /*! \fn ~TransformShapeBuilder()
    *  \brief TransformShapeBuilder����������.
    */
    ~TransformShapeBuilder();

public:
    /*! \fn bool shapeTranslate(TopoDS_Shape origShape, const double& vectorX, const double& vectorY, const double& vectorZ, const bool& isCopy)
    *  \brief ��ģ�ͽ���ƽ�Ʋ���.
    *  \param origShape ��Ҫ���в�����ģ�Ͷ���.
    *  \param vectorX ƽ��������x����.
    *  \param vectorY ƽ��������y����.
    *  \param vectorZ ƽ��������z����.
    *  \param isCopy �Ƿ���ԭģ��.
    *  \return �����Ƿ�ɹ�.
    */
    bool shapeTranslate(TopoDS_Shape origShape, const double& vectorX, const double& vectorY, const double& vectorZ, const bool& isCopy);

    /*! \fn bool shapeRotate(TopoDS_Shape origShape, const double& axisPosX, const double& axisPosY, const double& axisPosZ, const double& axisVecX, const double& axisVecY, const double& axisVecZ, const double& angle, const bool& isCopy)
    *  \brief ��ģ�ͽ�����ת����.
    *  \param origShape ��Ҫ���в�����ģ�Ͷ���.
    *  \param axisPosX ��ת������x����.
    *  \param axisPosY ��ת������y����.
    *  \param axisPosZ ��ת������z����.
    *  \param axisVecX ��ת�᷽��������x����.
    *  \param axisVecY ��ת�᷽��������y����.
    *  \param axisVecZ ��ת�᷽��������z����.
    *  \param angle ��ת�Ƕ�.
    *  \param isCopy �Ƿ���ԭģ��.
    *  \return �����Ƿ�ɹ�.
    */
    bool shapeRotate(TopoDS_Shape origShape, const double& axisPosX, const double& axisPosY, const double& axisPosZ, 
        const double& axisVecX, const double& axisVecY, const double& axisVecZ, const double& angle, const bool& isCopy);

    /*! \fn bool shapeScale(TopoDS_Shape origShape, const double& basePosX, const double& basePosY, const double& basePosZ, const double& scaleX, const double& scaleY, const double& scaleZ, const bool& isCopy)
    *  \brief ��ģ�ͽ������Ų���.
    *  \param origShape ��Ҫ���в�����ģ�Ͷ���.
    *  \param basePosX ���Ųο����x����.
    *  \param basePosY ���Ųο����y����.
    *  \param basePosZ ���Ųο����z����.
    *  \param scaleX x�������ű���.
    *  \param scaleY y�������ű���.
    *  \param scaleZ z�������ű���.
    *  \param isCopy �Ƿ���ԭģ��.
    *  \return �����Ƿ�ɹ�.
    */
    bool shapeScale(TopoDS_Shape origShape, const double& basePosX, const double& basePosY, const double& basePosZ,
        const double& scaleX, const double& scaleY, const double& scaleZ, const bool& isCopy);

	/*! \fn bool shapeScale(TopoDS_Shape origShape, const double& basePosX, const double& basePosY, const double& basePosZ, const double& scale, const bool& isCopy)
	*  \brief ��ģ�ͽ������Ų���.
	*  \param origShape ��Ҫ���в�����ģ�Ͷ���.
	*  \param basePosX ���Ųο����x����.
	*  \param basePosY ���Ųο����y����.
	*  \param basePosZ ���Ųο����z����.
	*  \param scale ���������ű���.
	*  \return �����Ƿ�ɹ�.
	*/
	bool shapeScale(TopoDS_Shape origShape, const double& basePosX, const double& basePosY, const double& basePosZ,
		const double& scale, const bool& isCopy);

    /*! \fn bool shapeScale(TopoDS_Shape origShape, double* wcsMatrix, const double& basePosX, const double& basePosY, const double& basePosZ, const double& scaleX, const double& scaleY, const double& scaleZ, const bool& isCopy)
    *  \brief �ӿ�������.
    */
    bool shapeScale(TopoDS_Shape origShape, double* wcsMatrix, const double& basePosX, const double& basePosY, const double& basePosZ,
        const double& scaleX, const double& scaleY, const double& scaleZ, const bool& isCopy);

    /*! \fn bool shapeMirror(TopoDS_Shape origShape, const double& basePosX, const double& basePosY, const double& basePosZ, const double& normalVecX, const double& normalVecY, const double& normalVecZ, const bool& isCopy)
    *  \brief ��ģ�ͽ��о������.
    *  \param origShape ��Ҫ���в�����ģ�Ͷ���.
    *  \param basePosX ����ƽ��������һ���x����.
    *  \param basePosY ����ƽ��������һ���y����.
    *  \param basePosZ ����ƽ��������һ���z����.
    *  \param normalVecX ����ƽ�淨��������x����.
    *  \param normalVecY ����ƽ�淨��������y����.
    *  \param normalVecZ ����ƽ�淨��������z����.
    *  \param isCopy �Ƿ���ԭģ��.
    *  \return �����Ƿ�ɹ�.
    */
    bool shapeMirror(TopoDS_Shape origShape, const double& basePosX, const double& basePosY, const double& basePosZ,
        const double& normalVecX, const double& normalVecY, const double& normalVecZ, const bool& isCopy);

    /*! \fn bool shapeCopy(TopoDS_Shape origShape)
    *  \brief ��ģ�ͽ��и��Ʋ���.
    *  \param origShape ��Ҫ���в�����ģ�Ͷ���.
    *  \return �����Ƿ�ɹ�.
    */
    bool shapeCopy(TopoDS_Shape origShape);
};

#endif
