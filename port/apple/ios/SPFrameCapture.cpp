// Renderer access for the iOS shell: the last presented frame as packed RGB
// (save-state thumbnails) and a texture-cache rebuild (texture upscaling).
// Kept in C++ because Flycast's renderer headers clash with Apple's Rect type
// in Objective-C files.
#include "types.h"
#include "hw/pvr/Renderer_if.h"

#include <vector>

bool SPCaptureLastFrameRGB(std::vector<u8>& rgb, int& width, int& height)
{
	return renderer != nullptr && renderer->GetLastFrame(rgb, width, height) && width > 0 && height > 0
			&& rgb.size() >= size_t(width) * height * 3;
}

// Flycast rebuilds its texture cache only on load or exit. Setting the
// renderer's own flag makes a texture upscaling change show at once; the
// member pointer is the legal way to reach a protected member from outside.
struct SPRendererAccess : Renderer
{
	static void resetTextures()
	{
		bool Renderer::*flag = &SPRendererAccess::resetTextureCache;
		if (renderer != nullptr)
			renderer->*flag = true;
	}
};

void SPResetTextureCache()
{
	SPRendererAccess::resetTextures();
}
