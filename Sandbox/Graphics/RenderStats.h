#pragma once

namespace Graphics
{
	//A single creation render stats component
	//Containing info such as viewport size
	//Provides a way to query info about the render
	//Without having to have direct dependencies on the rendering system itself
	struct RenderStats
	{
		int ViewPortHeight { 0 };
		int ViewPortWidth { 0 };
		int ViewPortStartX { 0 };
		int ViewPortStartY { 0 };
		float ViewPortCursorX { 0.f };
		float ViewPortCursorY { 0.f };
	};
}