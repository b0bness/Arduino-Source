/*  Image Diff
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#ifndef PokemonAutomation_CommonFramework_ImageDiff_H
#define PokemonAutomation_CommonFramework_ImageDiff_H

#include <QImage>
#include "CommonFramework/ImageTools/FloatPixel.h"

namespace PokemonAutomation{
namespace ImageMatch{


//  Compute average of all 4 components (ARGB).
QRgb pixel_average(const QImage& image);

//  Compute average of all 4 components.
//    - The two images must be the same dimensions.
//    - The alpha channels of "reference" is used to indicate which parts to ignore.
//      0 means background. 255 means object. No other values are valid.
QRgb pixel_average(const QImage& image, const QImage& alpha_mask);


//  Multiply every pixel by "multiplier". Alpha channels are ignored.
void scale_brightness(QImage& image, const FloatPixel& multiplier);


//  Compute root-mean-square deviation of the two images.
//    - The two images must be the same dimensions.
//    - The alpha channels of "reference" must be 0 or 255. No other values are valid.
//
//      If (reference.alpha == 255)  Count the pixel.
//      If (reference.alpha ==   0)  Ignore the pixel and exclude from pixel count.
//
double pixel_RMSD(const QImage& reference, const QImage& image);


//  Compute root-mean-square deviation of the two images.
//    - The two images must be the same dimensions.
//    - The alpha channels of "reference" must be 0 or 255. No other values are valid.
//
//      If (reference.alpha == 255)  Count the pixel.
//      If (reference.alpha ==   0)  Replace reference pixel with "background".
//
double pixel_RMSD(const QImage& reference, const QImage& image, QRgb background);


//  Compute root-mean-square deviation of the two images.
//    - The two images must be the same dimensions.
//    - The alpha channels of both images must be 0 or 255. No other values are valid.
//
//      If (reference.alpha == 255 && image.alpha == 255)  Count the pixel.
//      If (reference.alpha == 255 && image.alpha ==   0)  Count the pixel.
//      If (reference.alpha == 0   && image.alpha == 255)  Count the pixel as maximum possible distance. (255 for all RGB)
//      If (reference.alpha == 0   && image.alpha ==   0)  Ignore the pixel and exclude from pixel count.
//
double pixel_RMSD_masked(const QImage& reference, const QImage& image);

}
}
#endif