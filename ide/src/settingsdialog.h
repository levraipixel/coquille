/* Coquille
 * Copyright (C) 2009,2010
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

/////////////////
//   Headers   //
/////////////////

//C++ Libs

//Qt Libs
#include <QDialog>
class QCheckBox;
class QComboBox;
class QFontComboBox;
class QGridLayout;
class QLabel;
class QLineEdit;

//Project Libs

//Project Files
class Settings;
class SettingsDialog;

//File Classes
class ShortcutForm;
class FontForm;
class SettingsDialog;

//File NameSpace
namespace Ui {
    class SettingsDialog;
}

//////////////////////
//   ShortcutForm   //
//////////////////////

/*!
\brief A representation of a form to modify a shortcut for an action

It provides an easy way to display the form and read its values.
*/
class ShortcutForm : public QObject {
    Q_OBJECT

public:
    ShortcutForm( QString actionName, QString actionText, QKeySequence defaultValue, QGridLayout *shortcutsForm, int line );
    ~ShortcutForm();

    /*! returns the content of the form */
    QKeySequence readShortcut();
    /*! returns the name of the action represented by this form */
    QString actionName();

protected slots:
    void defaultValue();

protected:
    SettingsDialog *m_settingsDialog;
    QString m_actionName;
    QKeySequence m_defaultValue;
    QGridLayout *m_shortcutsForm;
    int m_line;
    QLabel *m_label;
    QLineEdit *m_lineEdit;
    QPushButton *m_pushButton;
};

//////////////////
//   FontForm   //
//////////////////

/*!
\brief A representation of a form to modify the font of a widget

It provides an easy way to display the form and apply its values.
*/
class FontForm : public QObject {
    Q_OBJECT

public:
    FontForm( QWidget *widget, QString label, QFont defaultValue, QGridLayout *fontsForm, int line );
    ~FontForm();

protected slots:
    void defaultValue();
    void changeFont(QFont font);
    void changeSize( QString size );
    void changeBold( bool b );
    void changeItalic( bool b );

protected:
    void refreshSize();
    void refreshBold();
    void refreshItalic();

    QLabel *m_label;
    QWidget *m_widget;
    QFont m_defaultValue;
    QGridLayout *m_fontsForm;
    QFontComboBox *m_family;
    QCheckBox *m_bold;
    QCheckBox *m_italic;
    QComboBox *m_size;
    int m_line;
    QPushButton *m_pushButton;
};

////////////////////////
//   SettingsDialog   //
////////////////////////

/*! \brief A dialog box displaying all the settings provided to the user */
class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    explicit SettingsDialog( Settings *parent );
    virtual ~SettingsDialog();

protected slots:
    virtual void accept();
    void apply();

protected:
    void addShortcutForm( QString actionName, QKeySequence defaultValue );
    void addFontForm( QWidget *widget, QString label );
    void writeShortcuts();
    void readShortcuts();

    virtual void changeEvent(QEvent *e);

    QList<ShortcutForm*> m_shortcutForm;
    QList<FontForm*> m_fontsForm;

    Ui::SettingsDialog *m_ui;
    Settings *m_settings;
};

#endif // SETTINGSDIALOG_H
