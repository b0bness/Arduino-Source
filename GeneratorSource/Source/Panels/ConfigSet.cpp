/*  Parent Class for all Configuration Files
 *
 *  From: https://github.com/Mysticial/Pokemon-Automation-SwSh-Arduino-Scripts
 *
 */

#include <QFile>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include "Common/Qt/StringException.h"
#include "Common/Qt/QtJsonTools.h"
#include "Tools/Tools.h"
#include "Tools/PersistentSettings.h"
#include "UI/MainWindow.h"
#include "ConfigSet.h"

namespace PokemonAutomation{


const QString ConfigSet::JSON_CONFIG_NAME   = "0-ConfigName";
const QString ConfigSet::JSON_CONFIG_PATH   = "1-ConfigPath";
const QString ConfigSet::JSON_DESCRIPTION   = "2-Description";
const QString ConfigSet::JSON_OPTIONS       = "3-Options";

ConfigSet::ConfigSet(const QJsonObject& obj)
    : RightPanel(json_get_string(obj, JSON_CONFIG_NAME))
    , m_path(json_get_string(obj, JSON_CONFIG_PATH))
    , m_description(json_get_string(obj, JSON_DESCRIPTION))
{}

QJsonDocument ConfigSet::to_json() const{
    QJsonObject root;
    root.insert(JSON_CONFIG_NAME, m_name);
    root.insert(JSON_CONFIG_PATH, m_path);
    root.insert(JSON_DESCRIPTION, m_description);
    root.insert(JSON_OPTIONS, options_json());
    return QJsonDocument(root);
}
std::string ConfigSet::to_cfile() const{
    std::string body;
    body += "//  This file is generated by the UI. There's no point in editing.\r\n";
    body += "#include \"";
    body += m_name.toUtf8().data();
    body += ".h\"\r\n";
    body += options_cpp();
    return body;
}
QString ConfigSet::save_json() const{
    QString name = settings.path + CONFIG_FOLDER_NAME + "/" + m_name + ".json";
    write_json_file(name, to_json());
    return name;
}
QString ConfigSet::save_cfile() const{
    QString name = settings.path + SOURCE_FOLDER_NAME + "/" + m_path + "/"+ m_name + ".c";
    std::string cpp = to_cfile();
    QFile file(name);
    if (!file.open(QFile::WriteOnly)){
        throw StringException("Unable to create source file: " + name);
    }
    if (file.write(cpp.c_str(), cpp.size()) != cpp.size()){
        throw StringException("Unable to write source file: " + name);
    }
    file.close();
    return name;
}


QWidget* ConfigSet::make_ui(MainWindow& parent){
    QWidget* box = new QWidget(&parent);
    QVBoxLayout* layout = new QVBoxLayout(box);
    layout->setMargin(0);
//    layout->setAlignment(Qt::AlignTop);
    layout->addWidget(new QLabel("<b><font size=4>Name:</font></b> " + m_name));
    QLabel* text = new QLabel("<b><font size=4>Description:</font></b> " + m_description);
    layout->addWidget(text);
    text->setWordWrap(true);
    layout->addWidget(make_options_body(*box), Qt::AlignBottom);

    QHBoxLayout* row = new QHBoxLayout();
    layout->addLayout(row);
    {
        QPushButton* button = new QPushButton("Save Settings", box);
        connect(
            button, &QPushButton::clicked,
            this, [&](bool){
                try{
                    QString json = save_json();
                    QString cfile = save_cfile();
                    QMessageBox box;
                    box.information(nullptr, "Success!", "Settings saved to:\n" + json + "\n" + cfile);
                }catch (const StringException& str){
                    QMessageBox box;
                    box.critical(nullptr, "Error", str.message());
                    return;
                }
            }
        );
        row->addWidget(button);
    }
    {
        QPushButton* button = new QPushButton("Restore Defaults", box);
        connect(
            button, &QPushButton::clicked,
            this, [&](bool){
                restore_defaults();
                parent.change_panel(*this);
            }
        );
        row->addWidget(button);
    }
#if 0
    {
        QPushButton* button = new QPushButton("Save and generate .hex file!", box);
        QFont font = button->font();
        font.setPointSize(16);
        button->setFont(font);
        connect(
            button, &QPushButton::clicked,
            this, [&](bool){
//                save_and_build(mcu_list.get_MCU());
            }
        );
        layout->addWidget(button);
    }
#endif
    return box;
}



}


