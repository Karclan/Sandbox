#pragma once

namespace Graphics
{
	//A single creation render stats component
	//Containing info such as viewport size
	//Provides a way to query info about the render
	//Without having to have direct dependencies on the rendering system itself
	struct RenderStats
	{
		unsigned int ViewPortHeight { 0u };
		unsigned int ViewPortWidth { 0u };
		unsigned int ViewPortStartX { 0u };
		unsigned int ViewPortStartY { 0u };
	};
}