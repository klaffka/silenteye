//  This file is part of SilentEye.
//
//  SilentEye is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  SilentEye is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with SilentEye.  If not, see <http://www.gnu.org/licenses/>.

#ifndef ABOUTDIALOG_H_
#define ABOUTDIALOG_H_

#include <QtWidgets>
#include "ui_aboutdialog.h"
#include "config.h"

namespace SilentEye {

    //! Display information about SilentEye (author, copyright, ...)
    class AboutDialog : public QDialog, public Ui::AboutDialog
    {
        Q_OBJECT;

    public:
    AboutDialog(QWidget* parent=0) : QDialog(parent)
    {
        setupUi(this);

        // version comes from version.xml shipped next to the executable;
        // fall back to the compiled-in version
        QString version = QString::fromLatin1(SILENTEYE_VERSION);
        SilentEyeFramework::Config conf(QCoreApplication::applicationDirPath() + "/",
                                        "version.xml", true);
        if (!conf.get("name").isEmpty())
            version = conf.get("name");
        versionLabel_2->setText(version);
    }

    };

}

#endif
