// Last presented frame as packed RGB, for save-state thumbnails. Kept in C++
// because Flycast's renderer headers clash with Apple's Rect type in
// Objective-C files.
#include "types.h"
#include "hw/pvr/Renderer_if.h"

#include <vector>

bool SPCaptureLastFrameRGB(std::vector<u8>& rgb, int& width, int& height)
{
	return renderer != nullptr && renderer->GetLastFrame(rgb, width, height) && width > 0 && height > 0
			&& rgb.size() >= size_t(width) * height * 3;
}
