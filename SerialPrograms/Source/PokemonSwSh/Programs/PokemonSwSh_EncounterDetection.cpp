/*  Encounter Tracker
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#include "Common/SwitchFramework/Switch_PushButtons.h"
#include "CommonFramework/Globals.h"
#include "CommonFramework/Tools/ErrorDumper.h"
#include "CommonFramework/Tools/InterruptableCommands.h"
#include "CommonFramework/Inference/ImageTools.h"
#include "CommonFramework/Inference/VisualInferenceSession.h"
#include "CommonFramework/Inference/BlackScreenDetector.h"
#include "PokemonSwSh_EncounterDetection.h"

namespace PokemonAutomation{
namespace NintendoSwitch{
namespace PokemonSwSh{


StandardEncounterDetection::StandardEncounterDetection(
    ProgramEnvironment& env,
    ConsoleHandle& console,
    Language language,
    const EncounterFilter& filter,
    ShinyType shininess,
    std::chrono::milliseconds read_name_delay
)
    : m_env(env)
    , m_console(console)
    , m_language(language)
    , m_filter(filter)
    , m_shininess(shininess)
    , m_read_name_delay(read_name_delay)
{}
bool StandardEncounterDetection::is_shiny() const{
    switch (m_shininess){
    case ShinyType::UNKNOWN:
        return false;
    case ShinyType::NOT_SHINY:
        return false;
    case ShinyType::STAR_SHINY:
        return true;
    case ShinyType::SQUARE_SHINY:
        return true;
    case ShinyType::UNKNOWN_SHINY:
        return true;
    }
    return false;
}
const std::set<std::string>* StandardEncounterDetection::candidates(){
    if (m_language == Language::None){
        m_name_read = true;
        return nullptr;
    }

    if (m_name_read){
        return &m_candidates;
    }

    InferenceBoxScope box(m_console, ImageFloatBox(0.76, 0.04, 0.15, 0.044));
    m_env.wait(m_read_name_delay);

    QImage screen = m_console.video().snapshot();
    QImage frame = screen;
    frame = extract_box(frame, box);

    OCR::MatchResult result = PokemonNameReader::instance().read_substring(m_language, frame);
    result.log(m_console);

    if (result.matched){
        m_candidates = std::move(result.slugs);
    }else{
        dump_image(
            m_console, screen,
            QString::fromStdString("StandardEncounterDetection-NameOCR-" + language_data(m_language).code)
        );
    }
    m_name_read = true;
    return &m_candidates;
}

bool filter_match(ShinyType detection, ShinyFilter filter){
    if (detection == ShinyType::UNKNOWN){
        return false;
    }

    switch (filter){
    case ShinyFilter::ANYTHING:
        return true;
    case ShinyFilter::NOT_SHINY:
        return detection == ShinyType::NOT_SHINY;
    case ShinyFilter::ANY_SHINY:
        return detection != ShinyType::NOT_SHINY;
    case ShinyFilter::STAR_ONLY:
        return detection == ShinyType::STAR_SHINY || detection == ShinyType::UNKNOWN_SHINY;
    case ShinyFilter::SQUARE_ONLY:
        return detection == ShinyType::SQUARE_SHINY || detection == ShinyType::UNKNOWN_SHINY;
    case ShinyFilter::NOTHING:
        return false;
    }

    return false;
}

std::pair<EncounterAction, std::string> StandardEncounterDetection::get_action(){
    if (m_shininess == ShinyType::UNKNOWN){
        return {EncounterAction::RunAway, ""};
    }

    std::pair<EncounterAction, std::string> action;
    action.first = filter_match(m_shininess, m_filter.shiny_filter())
        ? EncounterAction::StopProgram
        : EncounterAction::RunAway;

    const std::vector<EncounterFilterOverrides>& overrides = m_filter.overrides();
    if (!overrides.empty()){
        const std::set<std::string>* candidates = this->candidates();
        if (candidates != nullptr){
            for (const EncounterFilterOverrides& override : overrides){
                //  Not a token match.
                if (candidates->find(override.pokemon_slug) == candidates->end()){
                    continue;
                }

                //  Not a shiny filter match.
                if (!filter_match(m_shininess, override.shininess)){
                    continue;
                }

                action.first = override.action;
                action.second = override.pokeball_slug;
            }
        }
    }

    return action;
}




}
}
}
