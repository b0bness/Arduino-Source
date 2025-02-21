/*  Turbo Button
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#include "NintendoSwitch/Commands/NintendoSwitch_PushButtons.h"
#include "NintendoSwitch_TurboButton.h"

namespace PokemonAutomation{
namespace NintendoSwitch{



TurboButton_Descriptor::TurboButton_Descriptor()
    : RunnableSwitchProgramDescriptor(
        "NintendoSwitch:TurboButton",
        "Nintendo Switch", "Turbo Button",
        "ComputerControl/blob/master/Wiki/Programs/NintendoSwitch/TurboButton.md",
        "Mash a controller button. (similar to turbo controller)",
        FeedbackType::NONE,
        PABotBaseLevel::PABOTBASE_12KB
    )
{}



TurboButton::TurboButton(const TurboButton_Descriptor& descriptor)
    : SingleSwitchProgramInstance(descriptor)
    , BUTTON(
        "<b>Button to Mash:</b>",
        {
            "Y",
            "B",
            "A",
            "X",
            "L",
            "R",
            "ZL",
            "ZR",
            "Minus (-)",
            "Plus (+)",
            "L-Click (left joystick click)",
            "R-Click (right joystick click)",
            "Home",
            "Capture",
        },
        2
    )
    , PERIOD(
        "<b>Period (time between presses):</b>",
        "8", 8
    )
{
    PA_ADD_OPTION(BUTTON);
    PA_ADD_OPTION(PERIOD);
}
void TurboButton::program(SingleSwitchProgramEnvironment& env){
    while (true){
        pbf_press_button(env.console, (Button)1 << BUTTON, 5, PERIOD - 5);
    }
}



}
}

