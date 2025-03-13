#include <RayCaster.h>

#include <iostream>

int main(int argc, char** argv)
{
	RayCaster rayCaster(800, 600);
	rayCaster.setWinTitle("RayCaster");

	rayCaster.setShaderSource(R"(Resources\model.shader)", "modelShader");
	rayCaster.setShaderSource(R"(Resources\raycaster.shader)", "raycasterShader");
	rayCaster.appendMutableModel("raycaster", "raycasterShader");
#if 1
	rayCaster.setShaderSource(R"(Resources\xAxis.shader)", "xAxis");
	rayCaster.setShaderSource(R"(Resources\yAxis.shader)", "yAxis");
	rayCaster.setShaderSource(R"(Resources\zAxis.shader)", "zAxis");
	// x
	std::vector<GLModule::VertexNS::Vertex> verticesX
	{
		{ {500, 0, 0}, { 0, 0, 0}, {0, 0} },
		{ {-500, 0, 0}, { 0, 0, 0}, {0, 0} },
		// arrow
		{ {400, 100, 0}, { 0, 0, 0}, {0, 0} },
		{ {500, 0, 0}, { 0, 0, 0}, {0, 0} },
		{ {400, -100, 0}, { 0, 0, 0}, {0, 0} },
		{ {500, 0, 0}, { 0, 0, 0}, {0, 0} },
	};
	rayCaster.appendModel(verticesX, {}, "xAxis", GL_LINES);
	// y
	std::vector<GLModule::VertexNS::Vertex> verticesY
	{
		{ { 0, 500, 0 }, { 0, 0, 0}, { 0, 0 } },
		{ {0, -500, 0}, { 0, 0, 0}, {0, 0} },
		// arrow
		{ {100, 400, 0}, { 0, 0, 0}, {0, 0} },
		{ {0, 500, 0}, { 0, 0, 0}, {0, 0} },
		{ {-100, 400, 0}, { 0, 0, 0}, {0, 0} },
		{ {0, 500, 0}, { 0, 0, 0}, {0, 0} },
	};
	rayCaster.appendModel(verticesY, {}, "yAxis", GL_LINES);
	// z
	std::vector<GLModule::VertexNS::Vertex> verticesZ
	{
		{ {0, 0, 500}, { 0, 0, 0}, {0, 0} },
		{ {0, 0, -500}, { 0, 0, 0}, {0, 0} },
		// arrow
		{ {100, 0, 400}, { 0, 0, 0}, {0, 0} },
		{ {0, 0, 500}, { 0, 0, 0}, {0, 0} },
		{ {-100, 0, 400}, { 0, 0, 0}, {0, 0} },
		{ {0, 0, 500}, { 0, 0, 0}, {0, 0} },
	};
	rayCaster.appendModel(verticesZ, {}, "zAxis", GL_LINES);
#endif

#if 0
	std::vector<GLModule::VertexNS::Vertex> vertices
	{
		{ {-50, -50, 0}, { 0, 0, 1}, {0, 0} },
		{ {50, -50, 0}, { 0, 0, 1}, {0, 0} },
		{ {50, 50, 0}, { 0, 0, 1}, {0, 0} },
	};
	rayCaster.appendModel(vertices, {}, "modelShader");
#else
	rayCaster.appendModel(R"(Resources\10_20_100.stl)", "modelShader");
#endif
	if (rayCaster.initialize())
	{
		rayCaster.Run();
	}

	return 0;
}
