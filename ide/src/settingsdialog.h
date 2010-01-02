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

class ShortcutForm : public QObject {
    Q_OBJECT
    public:
        ShortcutForm( QString actionName, QString actionText, QKeySequence defaultValue, QGridLayout *shortcutsForm, int line );
        ~ShortcutForm();
        QKeySequence readShortcut();
    public slots:
        void defaultValue();

    public:
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

class FontForm : public QObject {
    Q_OBJECT
    public:
        FontForm( QWidget *widget, QString label, QFont defaultValue, QGridLayout *fontsForm, int line );
        ~FontForm();
    public slots:
        void defaultValue();
        void changeFont(QFont font);
        void refreshSize();
        void changeSize( QString size );
        void refreshBold();
        void changeBold( bool b );
        void refreshItalic();
        void changeItalic( bool b );

    public:
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

class SettingsDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(SettingsDialog)
public:
    explicit SettingsDialog( Settings *parent );
    virtual ~SettingsDialog();

    void addShortcutForm( QString actionName, QKeySequence defaultValue );
    void addFontForm( QWidget *widget, QString label );
    void writeShortcuts();
    void readShortcuts();

public slots:
    virtual void accept();
    void apply();

protected:
    virtual void changeEvent(QEvent *e);
    QList<ShortcutForm*> m_shortcutForm;
    QList<FontForm*> m_fontsForm;

private:
    Ui::SettingsDialog *m_ui;
    Settings *m_settings;
};

#endif // SETTINGSDIALOG_H
