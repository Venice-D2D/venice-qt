#include "include/tool/terminallanguagemanager.h"

TerminalLanguageManager::TerminalLanguageManager()
{
    this->language = this->getOSLanguage();
}


QString TerminalLanguageManager::getOSLanguage()
{
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    QString lang = env.value("LANG");

    if (lang.isEmpty())
        lang = env.value("LC_MESSAGES");

    if (lang.isEmpty())
        lang = env.value("LC_ALL");

    if (lang.isEmpty())
    {
        qWarning() << "Using Default OS Language";
        lang = DEFAULT_OS_LANGUAGE;
    }

    if (lang.contains("."))
        lang = lang.split(".").first();

     return lang;
}

QString TerminalLanguageManager::getYesValueAccordingToOSLanguage()
{
    QString yesTranslation = "yes";

    if (language.indexOf("fr_FR") != -1) {
        yesTranslation = "oui";

    } else if (language.indexOf("es_ES") != -1) {
        yesTranslation = "si";
    }

    return yesTranslation;

}
