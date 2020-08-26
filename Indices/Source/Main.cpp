#include "RenderSurface.h"
#include "RenderContext.h"

int main(int /*argc*/, char** /*argv*/)
{
	RenderSurface renderSurface;
	RenderContext renderContext;

	while (!renderSurface.myShouldClose)
	{
		renderContext.Render(renderSurface.GetScreenWidth(), renderSurface.GetScreenHeight());
		renderSurface.Tick();
	}

	renderSurface.Destroy();

	return 0;
}
