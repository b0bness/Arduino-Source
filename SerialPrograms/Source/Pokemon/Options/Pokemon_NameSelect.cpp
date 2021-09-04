/*  Pokemon Name Select
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#include <QtGlobal>
#include "Common/Cpp/Exception.h"
#include "Common/Qt/QtJsonTools.h"
#include "CommonFramework/PersistentSettings.h"
#include "CommonFramework/Tools/Logger.h"
#include "Pokemon/Resources/Pokemon_PokemonNames.h"
#include "PokemonSwSh/Resources/PokemonSwSh_PokemonSprites.h"
#include "Pokemon_NameSelect.h"

#include <iostream>
using std::cout;
using std::endl;

namespace PokemonAutomation{
namespace Pokemon{


PokemonNameSelectData::PokemonNameSelectData(const QString& json_file_slugs){
    QJsonArray array = read_json_file(
        PERSISTENT_SETTINGS().resource_path + json_file_slugs
    ).array();
    for (const auto& item : array){
        QString slug = item.toString();
        std::string slug_str = slug.toUtf8().data();
        if (slug.size() <= 0){
            PA_THROW_StringException("Expected non-empty string for Pokemon slug.");
        }

        using namespace NintendoSwitch::PokemonSwSh;
        const PokemonNames& data = get_pokemon_name(slug_str);
        const PokemonSprite* sprites = get_pokemon_sprite_nothrow(slug_str);
        if (sprites == nullptr){
            m_list.emplace_back(data.display_name(), QIcon());
            global_logger().log("Missing sprite for: " + slug, "red");
        }else{
            m_list.emplace_back(
                data.display_name(),
                sprites->icon()
           );
        }
    }
}



PokemonNameSelect::PokemonNameSelect(
    QString label,
    const QString& json_file_slugs,
    const std::string& default_slug
)
    : PokemonNameSelectData(json_file_slugs)
    , StringSelect(
        std::move(label),
        cases(),
        default_slug.empty() ? "" :get_pokemon_name(default_slug).display_name()
    )
{}

const std::string& PokemonNameSelect::slug() const{
    const QString& display = (const QString&)*this;
    return parse_pokemon_name(display);
}




}
}