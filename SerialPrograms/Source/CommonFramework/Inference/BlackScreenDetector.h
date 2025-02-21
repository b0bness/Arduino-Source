/*  Black Screen Detector
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 *
 *      Returns true after a black screen has been detected and
 * the black screen has ended.
 *
 */

#ifndef PokemonAutomation_CommonFramework_BlackScreenDetector_H
#define PokemonAutomation_CommonFramework_BlackScreenDetector_H

#include "CommonFramework/Tools/VideoFeed.h"
#include "CommonFramework/Tools/Logger.h"
#include "CommonFramework/Inference/VisualInferenceCallback.h"

namespace PokemonAutomation{


class BlackScreenDetector : public VisualInferenceCallback{
public:
    BlackScreenDetector(const ImageFloatBox& box = {0.1, 0.1, 0.8, 0.8});

    virtual bool process_frame(
        const QImage& frame,
        std::chrono::system_clock::time_point timestamp
    ) override;

private:
    ImageFloatBox m_box;
};


class BlackScreenOverDetector : public VisualInferenceCallback{
public:
    BlackScreenOverDetector(const ImageFloatBox& box = {0.1, 0.1, 0.8, 0.8});

    bool black_is_over(const QImage& frame);
    virtual bool process_frame(
        const QImage& frame,
        std::chrono::system_clock::time_point timestamp
   ) override;

private:
    ImageFloatBox m_box;
    bool m_has_been_black;
};



}
#endif
