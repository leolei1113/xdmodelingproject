# 1、准备工作，编译方式、目标文件名、依赖库路径的定义。
CXX = g++
CXXFLAGS  := -Wall -O3 -std=c++0x 

# opencv 头文件和lib路径 
OCC_INC_ROOT = /home/neuer/package/occ_install/include/opencascade
OCC_LIB_ROOT = /home/neuer/package/occ_install/lib

OBJS = AcisModelBuilder.o LabelShapeAdapter.o OccModelBuilder.o OccApplication.o OccDocShape.o OccModel.o OccShapeStorage.o
LIB = libxdOccGeom.so # 目标文件名 

OPENCV_INC= -I $(OCC_INC_ROOT)

INCLUDE_PATH = $(OPENCV_INC) -I./

LIB_PATH = -L $(OCC_LIB_ROOT)

# 依赖的lib名称
OCC_LIB = -lBin \
 -lXml \
 -lStd \
 -lXmlXCAF \
 -lBinXCAF \
 -lVCAF \
 -lXCAF \
 -lLCAF \
 -lBrep \
 -lIGES \
 -lShHealing \
 -lStep \
 -lXSBase \
 -lBO \
 -lBool \
 -lCAF \
 -lCDF \
 -lernel \
 -lFeat \
 -lFillet \
 -lG2d \
 -lG3d \
 -lGeomAlgo \
 -lGeomBase \
 -lHLR \
 -lMath \
 -lOffset \
 -lPrim \
 -lService \
 -lVrml \
 -lStl \
 -lBRep \
 -lSTEP \
 -lSTEP209 \
 -lSTEPAttr \
 -lSTEPBase \
 -lTopAlgo \
 -lV3d \
 -lMesh \
 -lMeshVS \
 -lSTL \
 -lOpenGl

all : $(LIB)

# 2. 生成.o文件 
$(OBJS) :%.o : %.cpp
	$(CXX) $(CXXFLAGS) -fpic -c $< -o $@ $(INCLUDE_PATH) $(LIB_PATH) $(OCC_LIB) 

# 3. 生成动态库文件
$(LIB) : $(OBJS)
	rm -f $@
	g++ -shared -o $@ $(OBJS)
	rm -f $(OBJS)

tags :
	ctags -R *

# 4. 删除中间过程生成的文件 
clean:
	rm -f $(OBJS) $(TARGET) $(LIB)
