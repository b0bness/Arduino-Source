/*  Max Lair Run Battle
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#ifndef PokemonAutomation_PokemonSwSh_MaxLair_Run_Battle_H
#define PokemonAutomation_PokemonSwSh_MaxLair_Run_Battle_H

#include "CommonFramework/Language.h"
#include "CommonFramework/Tools/ProgramEnvironment.h"
#include "PokemonSwSh/MaxLair/Options/PokemonSwSh_MaxLair_Options.h"
#include "PokemonSwSh/MaxLair/Framework/PokemonSwSh_MaxLair_StateMachine.h"
#include "PokemonSwSh/MaxLair/Framework/PokemonSwSh_MaxLair_StateTracker.h"

namespace PokemonAutomation{
namespace NintendoSwitch{
namespace PokemonSwSh{
namespace MaxLairInternal{


StateMachineAction run_move_select(
    ProgramEnvironment& env,
    ConsoleHandle& console,
    GlobalStateTracker& state_tracker,
    const ConsoleSpecificOptions& settings,
    bool currently_dmaxed, bool cheer_only
);

StateMachineAction throw_balls(
    AdventureRuntime& runtime,
    ProgramEnvironment& env,
    ConsoleHandle& console, Language language,
    GlobalStateTracker& state_tracker,
    const EndBattleDecider& decider
);



}
}
}
}
#endif
