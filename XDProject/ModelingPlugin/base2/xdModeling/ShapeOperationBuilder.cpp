#include "ShapeOperationBuilder.h"

ShapeOperationBuilder::ShapeOperationBuilder()
    :m_builder(nullptr)
{

}

ShapeOperationBuilder::~ShapeOperationBuilder()
{
	if (m_builder)
	{
		delete m_builder;
	}
}