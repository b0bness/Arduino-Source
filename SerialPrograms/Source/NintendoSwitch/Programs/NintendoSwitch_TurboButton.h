/*  Turbo Button
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#ifndef PokemonAutomation_NintendoSwitch_TurboButton_H
#define PokemonAutomation_NintendoSwitch_TurboButton_H

#include "CommonFramework/Options/EnumDropdownOption.h"
#include "NintendoSwitch/Options/TimeExpressionOption.h"
#include "NintendoSwitch/Framework/SingleSwitchProgram.h"

namespace PokemonAutomation{
namespace NintendoSwitch{


class TurboButton_Descriptor : public RunnableSwitchProgramDescriptor{
public:
    TurboButton_Descriptor();
};


class TurboButton : public SingleSwitchProgramInstance{
public:
    TurboButton(const TurboButton_Descriptor& descriptor);

    virtual void program(SingleSwitchProgramEnvironment& env) override;

private:
    EnumDropdownOption BUTTON;
    TimeExpressionOption<uint16_t> PERIOD;
};



}
}
#endif

