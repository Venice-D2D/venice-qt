#ifndef TERMINALLANGUAGEMANAGER_H
#define TERMINALLANGUAGEMANAGER_H

#include <QLocale>
#include <QProcessEnvironment>
#include <QDebug>

static const QString DEFAULT_OS_LANGUAGE = "en_US";

class TerminalLanguageManager
{
public:
    /**
     * @brief TerminalLanguageManager Default constructor
     */
    TerminalLanguageManager();

    /**
     * @brief getYesValueAccordingToOSLanguage Returns the translation of "yes" word
     * according to the language
     * @return The traduction of yes
     */
    QString getYesValueAccordingToOSLanguage();


private:
    // The OS Language
    QString language = "US";

    /**
     * @brief getOSLanguage Retrives the OS language by using environment variables
     * @return The used OS Language
     */
    QString getOSLanguage();




};

#endif // TERMINALLANGUAGEMANAGER_H
