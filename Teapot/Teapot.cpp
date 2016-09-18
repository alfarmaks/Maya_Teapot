#include <maya/MPxCommand.h>
#include <maya/MGlobal.h>
#include <maya/MFnPlugin.h>
#include <maya/MArgDatabase.h>
#include <maya/MArgList.h>

class Teapot : public MPxCommand
{
public:
	virtual MStatus doIt(const MArgList &args);
	static void *creator() { return new Teapot; }
private:
	double radius = 5;
	double x = 0, y = 0, z = 0;

	void createBody();
	void createCap();
	void connectCapAndBody();
	void moveTeapot();
};


//create teapot's body
void Teapot::createBody()
{
	//create body of teapot
	MGlobal::executeCommand(MString("polySphere -radius ") + radius + " -name part1_1");
	MGlobal::executeCommand(MString("move -relative ") + 0.0 + " " + (radius * 0.8) + " " + 0.0);
	MGlobal::executeCommand(MString("polyCube -width ") + (radius * 2.2)
		+ " -depth " + (radius * 2.2)
		+ " -height " + (radius * 1.6) + " -name part1_2");
	MGlobal::executeCommand(MString("move -relative ") + 0.0 + " " + (radius * 0.8) + " " + 0.0);

	//connect part of teapot's body
	MGlobal::executeCommand(MString("select -cl"));
	MGlobal::executeCommand(MString("select part1_1"));
	MGlobal::executeCommand(MString("select part1_2"));
	MGlobal::executeCommand(MString("polyCBoolOp -op 3 -ch 1 -preserveColor 0 -classification 2 -name part1_2 part1_2 part1_1"));
	MGlobal::executeCommand(MString("rename part1_3 part2_1"));

	//create sphere for inner part of teapot
	MGlobal::executeCommand(MString("polySphere -radius ") + (radius * 0.9) + " -name part2_2");
	MGlobal::executeCommand(MString("move -relative ") + 0.0 + " " + (radius * 0.9) + " " + 0.0);

	//connect sphere and part of teapot's body
	MGlobal::executeCommand(MString("select -cl"));
	MGlobal::executeCommand(MString("select part2_1"));
	MGlobal::executeCommand(MString("select part2_2"));
	MGlobal::executeCommand(MString("polyCBoolOp -op 2 -ch 1 -preserveColor 0 -classification 2 -name part2_1 part2_1 part2_2"));
	MGlobal::executeCommand(MString("rename part2_3 part1"));

	//create handle for body
	MGlobal::executeCommand(MString("polyTorus -radius ") + (radius * 0.4) + " -sectionRadius " + (radius * 0.12) + " -name part4_1");
	MGlobal::executeCommand(MString("rotate -r -os -fo -90 0 0"));
	MGlobal::executeCommand(MString("move -relative ") + (radius * 0.96) + " " + (radius * 0.8) + " " + 0.0);
	MGlobal::executeCommand(MString("polySphere -radius ") + radius + " -name part4_2");
	MGlobal::executeCommand(MString("move -relative ") + 0.0 + " " + (radius * 0.8) + " " + 0.0);

	//cut part that we need
	MGlobal::executeCommand(MString("select -cl"));
	MGlobal::executeCommand(MString("select part4_1"));
	MGlobal::executeCommand(MString("select part4_2"));
	MGlobal::executeCommand(MString("polyCBoolOp -op 2 -ch 1 -preserveColor 0 -classification 2 -name part4_1 part4_1 part4_2"));
	MGlobal::executeCommand(MString("rename part4_3 part3"));

	//connect handle for body and teapot's body
	MGlobal::executeCommand(MString("select -cl"));
	MGlobal::executeCommand(MString("select part1"));
	MGlobal::executeCommand(MString("select part3"));
	MGlobal::executeCommand(MString("polyCBoolOp -op 1 -ch 1 -preserveColor 0 -classification 2 -name part1 part1 part3"));

	//nose
	MGlobal::executeCommand(MString("polyCylinder -radius ") + (radius * 0.07) + " -height " + (radius * 0.8) + " -name part5_1");
	MGlobal::executeCommand(MString("polyCylinder -radius ") + (radius * 0.1) + " -height " + (radius * 0.8) + " -name part5_2");

	//cut hole in nose
	MGlobal::executeCommand(MString("select -cl"));
	MGlobal::executeCommand(MString("select part5_2"));
	MGlobal::executeCommand(MString("select part5_1"));
	MGlobal::executeCommand(MString("polyCBoolOp -op 2 -ch 1 -preserveColor 0 -classification 2 -name part5_2 part5_2 part5_1"));
	
	//create sphere to cut part of nose
	MGlobal::executeCommand(MString("rotate -r -os -fo 0 0 45"));
	MGlobal::executeCommand(MString("move -relative ") + (radius * (-1.2)) + " " + radius + " " + 0.0);
	MGlobal::executeCommand(MString("polySphere -radius ") + radius + " -name part5_4");
	MGlobal::executeCommand(MString("move -relative ") + 0.0 + " " + (radius * 0.8) + " " + 0.0);

	//cut part that we need for nose
	MGlobal::executeCommand(MString("select -cl"));
	MGlobal::executeCommand(MString("select part5_3"));
	MGlobal::executeCommand(MString("select part5_4"));
	MGlobal::executeCommand(MString("polyCBoolOp -op 2 -ch 1 -preserveColor 0 -classification 2 -name part5_3 part5_3 part5_4"));

	//create cube to cut part of nose
	MGlobal::executeCommand(MString("polyCube -width ") + (radius * 3.2)
		+ " -depth " + (radius * 3.2)
		+ " -height " + radius + " -name part5_6");
	MGlobal::executeCommand(MString("move -relative ") + 0.0 + " " + (radius * 1.6) + " " + 0.0);

	//cut part that we need for nose
	MGlobal::executeCommand(MString("select -cl"));
	MGlobal::executeCommand(MString("select part5_5"));
	MGlobal::executeCommand(MString("select part5_6"));
	MGlobal::executeCommand(MString("polyCBoolOp -op 2 -ch 1 -preserveColor 0 -classification 2 -name part5_5 part5_5 part5_6"));

	//create cylinder to cut hole in teapot's body
	MGlobal::executeCommand(MString("polyCylinder -radius ") + (radius * 0.07) + " -height " + (radius * 1.6) + " -name part5_8");
	MGlobal::executeCommand(MString("rotate -r -os -fo 0 0 45"));
	MGlobal::executeCommand(MString("move -relative ") + (radius * (-1.18)) + " " + radius + " " + 0.0);

	//cut hole in teapot's body
	MGlobal::executeCommand(MString("select -cl"));
	MGlobal::executeCommand(MString("select part5"));
	MGlobal::executeCommand(MString("select part5_8"));
	MGlobal::executeCommand(MString("polyCBoolOp -op 2 -ch 1 -preserveColor 0 -classification 2 -name part5 part5 part5_8"));

	//connect nose and body with handle
	MGlobal::executeCommand(MString("select -cl"));
	MGlobal::executeCommand(MString("select part6"));
	MGlobal::executeCommand(MString("select part5_7"));
	MGlobal::executeCommand(MString("polyCBoolOp -op 1 -ch 1 -preserveColor 0 -classification 2 -name part6 part6 part5_7"));
}

//create teapot's cap
void Teapot::createCap()
{
	//cap
	MGlobal::executeCommand(MString("polySphere -radius ") + radius + " -name part3_1");
	MGlobal::executeCommand(MString("move -relative ") + 0.0 + " " + (radius * 0.8 - 0.1) + " " + 0.0);
	MGlobal::executeCommand(MString("polyCube -width ") + (radius * 2.2)
		+ " -depth " + (radius * 2.2)
		+ " -height " + (radius * 2) + " -name part3_2");
	MGlobal::executeCommand(MString("move -relative ") + 0.0 + " " + (radius * 0.6) + " " + 0.0);

	//bool operation to create part of cap
	MGlobal::executeCommand(MString("select -cl"));
	MGlobal::executeCommand(MString("select part3_1"));
	MGlobal::executeCommand(MString("select part3_2"));
	MGlobal::executeCommand(MString("polyCBoolOp -op 2 -ch 1 -preserveColor 0 -classification 2 -name part3_1 part3_1 part3_2"));
	MGlobal::executeCommand(MString("polySphere -radius ") + (radius * 0.96) + " -name part3_4");
	MGlobal::executeCommand(MString("move -relative ") + 0.0 + " " + (radius * 0.8) + " " + 0.0);

	//connect part of cap with sphere to create normal cap
	MGlobal::executeCommand(MString("select -cl"));
	MGlobal::executeCommand(MString("select part3_3"));
	MGlobal::executeCommand(MString("select part3_4"));
	MGlobal::executeCommand(MString("polyCBoolOp -op 2 -ch 1 -preserveColor 0 -classification 2 -name part3_3 part3_3 part3_4"));

	//create handle
	MGlobal::executeCommand(MString("polySphere -radius ") + (radius * 0.1) + " -name part3_6");
	MGlobal::executeCommand(MString("move -relative ") + 0.0 + " " + (radius * 1.84) + " " + 0.0);

	//connect handle and cap
	MGlobal::executeCommand(MString("select -cl"));
	MGlobal::executeCommand(MString("select part3_5"));
	MGlobal::executeCommand(MString("select part3_6"));
	MGlobal::executeCommand(MString("polyCBoolOp -op 1 -ch 1 -preserveColor 0 -classification 2 -name part3_5 part3_5 part3_6"));
	MGlobal::executeCommand(MString("rename part3_7 part2"));
}

//connect teapot's body and cap
void Teapot::connectCapAndBody()
{
	MGlobal::executeCommand(MString("select -cl"));
	MGlobal::executeCommand(MString("select part2"));
	MGlobal::executeCommand(MString("select part7"));
	MGlobal::executeCommand(MString("polyCBoolOp -op 1 -ch 1 -preserveColor 0 -classification 2 -name part2 part2 part7"));
	MGlobal::executeCommand(MString("rename part8 Teapot"));
}

void Teapot::moveTeapot()
{
	MGlobal::executeCommand(MString("move -relative ") + x + " " + y + " " + z);
}

MStatus Teapot::doIt(const MArgList &args)
{
	unsigned index;
	index = args.flagIndex("-r", "-radius");
	if (MArgList::kInvalidArgIndex != index)
		args.get(index + 1, radius);
	index = args.flagIndex("-x", "-X" );
	if (MArgList::kInvalidArgIndex != index)
		args.get(index + 1, x);	index = args.flagIndex("-y", "-Y");
	if (MArgList::kInvalidArgIndex != index)
		args.get(index + 1, y);	index = args.flagIndex("-z", "-Z");
	if (MArgList::kInvalidArgIndex != index)
		args.get(index + 1, z);
	createBody();
	createCap();
	connectCapAndBody();
	if (!x || !y || !z)
	{
		moveTeapot();
	}

	return MS::kSuccess;
}


MStatus initializePlugin(MObject obj)
{
	MFnPlugin pluginFn(obj, "My plugin", "4.0");
	MStatus stat;
	stat = pluginFn.registerCommand("teapot", Teapot::creator);
	if (!stat)
		stat.perror("registerComrnand failed");
	return stat;
}

MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin pluginFn(obj);
	MStatus stat;
	stat = pluginFn.deregisterCommand("delTeapot");
	if (!stat)
		stat.perror("deregisterCommand failed");
	return stat;
}


//#include <maya/MTime.h>
//#include <maya/MFnMesh.h>
//#include <maya/MPoint.h>
//#include <maya/MFloatPoint.h>
//#include <maya/MFloatPointArray.h>
//#include <maya/MIntArray.h>
//#include <maya/MDoubleArray.h>
//#include <maya/MFnUnitAttribute.h>
//#include <maya/MFnTypedAttribute.h>
//#include <maya/MFnPlugin.h>
//
//#include <maya/MPxNode.h>
//#include <maya/MObject.h>
//#include <maya/MPlug.h>
//#include <maya/MDataBlock.h>
//#include <maya/MFnMeshData.h>
//
//#include <maya/MIOStream.h>
//
//MStatus returnStatus;
//
//#define McheckErr(stat, msg)		\
//	if (MS::kSuccess != stat ) {	\
//		cerr << msg;				\
//		return MS::kFailure;		\
//		}
//
//class animCube : public MPxNode
//{
//public:
//	animCube() {};
//	virtual 		~animCube() {};
//	virtual MStatus compute(const MPlug& plug, MDataBlock& data);
//	static  void*	creator();
//	static  MStatus initialize();
//
//	static MObject	time;
//	static MObject	outputMesh;
//	static MTypeId	id;
//
//protected:
//	MObject createMesh(const MTime& time, MObject& outData, MStatus& stat);
//
//	// Helpers
//	MObject createQuads(const MFloatPointArray& points, MObject& outData, MStatus& stat);
//	MObject createReverseQuads(const MFloatPointArray& points, MObject& outData, MStatus& stat);
//	MObject createTris(const MFloatPointArray& points, MObject& outData, MStatus& stat);
//	MObject createReverseTris(const MFloatPointArray& points, MObject& outData, MStatus& stat);
//};
//
//MObject animCube::time;
//MObject animCube::outputMesh;
//MTypeId animCube::id(0x80000);
//
//void* animCube::creator()
//{
//	return new animCube;
//}
//
//MStatus animCube::initialize()
//{
//	MFnUnitAttribute unitAttr;
//	MFnTypedAttribute typedAttr;
//
//	MStatus returnStatus;
//
//	animCube::time = unitAttr.create("time", "tm",
//		MFnUnitAttribute::kTime,
//		0.0, &returnStatus);
//	McheckErr(returnStatus, "ERROR creating animCube time attribute\n");
//
//
//	animCube::outputMesh = typedAttr.create("outputMesh", "out",
//		MFnData::kMesh,
//		&returnStatus);
//	McheckErr(returnStatus, "ERROR creating animCube output attribute\n");
//	typedAttr.setStorable(false);
//
//	returnStatus = addAttribute(animCube::time);
//	McheckErr(returnStatus, "ERROR adding time attribute\n");
//
//	returnStatus = addAttribute(animCube::outputMesh);
//	McheckErr(returnStatus, "ERROR adding outputMesh attribute\n");
//
//	returnStatus = attributeAffects(animCube::time,
//		animCube::outputMesh);
//	McheckErr(returnStatus, "ERROR in attributeAffects\n");
//
//	return MS::kSuccess;
//}
//
//MObject animCube::createMesh(const MTime& time,
//	MObject& outData,
//	MStatus& stat)
//
//{
//	// Scale the cube on the frame number, wrap every 10 frames.
//	//
//	const int frame = (int)time.as(MTime::kFilm);
//	const float cubeSize = 0.5f * (float)(frame % 10 + 1);
//
//	MFloatPointArray points;
//	points.append(-cubeSize, -cubeSize, -cubeSize);
//	points.append(cubeSize, -cubeSize, -cubeSize);
//	points.append(cubeSize, -cubeSize, cubeSize);
//	points.append(-cubeSize, -cubeSize, cubeSize);
//	points.append(-cubeSize, cubeSize, -cubeSize);
//	points.append(-cubeSize, cubeSize, cubeSize);
//	points.append(cubeSize, cubeSize, cubeSize);
//	points.append(cubeSize, cubeSize, -cubeSize);
//
//	MObject newMesh;
//
//	static const bool sTestVertexIdAndFaceId =
//		(getenv("MAYA_TEST_VERTEXID_AND_FACEID") != NULL);
//	if (sTestVertexIdAndFaceId)
//	{
//		// If the env var is set, the topology of the cube will be changed over
//		// frame number (looping in every 4 frames). When the shape is assigned
//		// with a hwPhongShader, the shader receives vertex ids and face ids,
//		// which are generated from polygonConnects passed to MFnMesh::create
//		// method in this plugin.
//		//
//		switch (frame % 4)
//		{
//		case 1:
//			newMesh = createQuads(points, outData, stat);
//			break;
//		case 2:
//			newMesh = createReverseQuads(points, outData, stat);
//			break;
//		case 3:
//			newMesh = createTris(points, outData, stat);
//			break;
//		case 0:
//			newMesh = createReverseTris(points, outData, stat);
//			break;
//		default:
//			newMesh = createQuads(points, outData, stat);
//			break;
//		}
//	}
//	else
//	{
//		newMesh = createQuads(points, outData, stat);
//	}
//
//	return newMesh;
//}
//
//MObject animCube::createQuads(
//	const MFloatPointArray& points,
//	MObject& outData,
//	MStatus& stat)
//{
//	// Set up an array containing the number of vertices
//	// for each of the 6 cube faces (4 vertices per face)
//	//
//	const int numFaces = 6;
//	int face_counts[numFaces] = { 4, 4, 4, 4, 4, 4 };
//	MIntArray faceCounts(face_counts, numFaces);
//
//	// Set up and array to assign vertices from points to each face 
//	//
//	const int numFaceConnects = 24;
//	int	face_connects[numFaceConnects] = { 0, 1, 2, 3,
//		4, 5, 6, 7,
//		3, 2, 6, 5,
//		0, 3, 5, 4,
//		0, 4, 7, 1,
//		1, 7, 6, 2 };
//	MIntArray faceConnects(face_connects, numFaceConnects);
//
//	MFnMesh	meshFS;
//	return meshFS.create(points.length(), faceCounts.length(),
//		points, faceCounts, faceConnects, outData, &stat);
//}
//
//MObject animCube::createReverseQuads(
//	const MFloatPointArray& points,
//	MObject& outData,
//	MStatus& stat)
//{
//	// Set up an array containing the number of vertices
//	// for each of the 6 cube faces (4 vertices per face)
//	//
//	const int numFaces = 6;
//	int face_counts[numFaces] = { 4, 4, 4, 4, 4, 4 };
//	MIntArray faceCounts(face_counts, numFaces);
//
//	// Set up and array to assign vertices from points to each face
//	//
//	const int numFaceConnects = 24;
//	int	face_connects[numFaceConnects] = { 0, 3, 2, 1,
//		4, 7, 6, 5,
//		3, 5, 6, 2,
//		0, 4, 5, 3,
//		0, 1, 7, 4,
//		1, 2, 6, 7 };
//	MIntArray faceConnects(face_connects, numFaceConnects);
//
//	MFnMesh	meshFS;
//	return meshFS.create(points.length(), faceCounts.length(),
//		points, faceCounts, faceConnects, outData, &stat);
//}
//
//MObject animCube::createTris(
//	const MFloatPointArray& points,
//	MObject& outData,
//	MStatus& stat)
//{
//	// Set up an array containing the number of vertices
//	// for each of the 12 triangles (3 verticies per triangle)
//	//
//	const int numFaces = 12;
//	int face_counts[numFaces] = { 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 };
//	MIntArray faceCounts(face_counts, numFaces);
//
//	// Set up and array to assign vertices from points to each face
//	//
//	const int numFaceConnects = 36;
//	int	face_connects[numFaceConnects] = { 0, 1, 2,
//		2, 3, 0,
//		4, 5, 6,
//		6, 7, 4,
//		3, 2, 6,
//		6, 5, 3,
//		0, 3, 5,
//		5, 4, 0,
//		0, 4, 7,
//		7, 1, 0,
//		1, 7, 6,
//		6, 2, 1 };
//	MIntArray faceConnects(face_connects, numFaceConnects);
//
//	MFnMesh	meshFS;
//	return meshFS.create(points.length(), faceCounts.length(),
//		points, faceCounts, faceConnects, outData, &stat);
//}
//
//MObject animCube::createReverseTris(
//	const MFloatPointArray& points,
//	MObject& outData,
//	MStatus& stat)
//{
//	// Set up an array containing the number of vertices
//	// for each of the 12 triangles (3 verticies per triangle)
//	//
//	const int numFaces = 12;
//	int face_counts[numFaces] = { 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 };
//	MIntArray faceCounts(face_counts, numFaces);
//
//	// Set up and array to assign vertices from points to each face
//	//
//	const int numFaceConnects = 36;
//	int	face_connects[numFaceConnects] = { 0, 2, 1,
//		2, 0, 3,
//		4, 6, 5,
//		6, 4, 7,
//		3, 6, 2,
//		6, 3, 5,
//		0, 5, 3,
//		5, 0, 4,
//		0, 7, 4,
//		7, 0, 1,
//		1, 6, 7,
//		6, 1, 2 };
//	MIntArray faceConnects(face_connects, numFaceConnects);
//
//	MFnMesh	meshFS;
//	return meshFS.create(points.length(), faceCounts.length(),
//		points, faceCounts, faceConnects, outData, &stat);
//}
//
//MStatus animCube::compute(const MPlug& plug, MDataBlock& data)
//
//{
//	MStatus returnStatus;
//
//	if (plug == outputMesh) {
//		/* Get time */
//		MDataHandle timeData = data.inputValue(time, &returnStatus);
//		McheckErr(returnStatus, "Error getting time data handle\n");
//		MTime time = timeData.asTime();
//
//		/* Get output object */
//
//		MDataHandle outputHandle = data.outputValue(outputMesh, &returnStatus);
//		McheckErr(returnStatus, "ERROR getting polygon data handle\n");
//
//		MFnMeshData dataCreator;
//		MObject newOutputData = dataCreator.create(&returnStatus);
//		McheckErr(returnStatus, "ERROR creating outputData");
//
//		createMesh(time, newOutputData, returnStatus);
//		McheckErr(returnStatus, "ERROR creating new Cube");
//
//		outputHandle.set(newOutputData);
//		data.setClean(plug);
//	}
//	else
//		return MS::kUnknownParameter;
//
//	return MS::kSuccess;
//}

