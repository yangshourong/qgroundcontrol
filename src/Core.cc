/*=====================================================================

PIXHAWK Micro Air Vehicle Flying Robotics Toolkit

(c) 2009, 2010 PIXHAWK PROJECT  <http://pixhawk.ethz.ch>

This file is part of the PIXHAWK project

    PIXHAWK is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    PIXHAWK is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with PIXHAWK. If not, see <http://www.gnu.org/licenses/>.

======================================================================*/

/**
 * @file
 *   @brief Implementation of main class
 *
 *   @author Lorenz Meier <mavteam@student.ethz.ch>
 *
 */


#include <QFile>
#include <QFlags>
#include <QThread>
#include <QSplashScreen>
#include <QPixmap>
#include <QDesktopWidget>
#include <QPainter>
#include <QStyleFactory>
#include <QAction>

#include <Core.h>
#include <MG.h>
#include <MainWindow.h>
#include "GAudioOutput.h"


/**
 * @brief Constructor for the main application.
 *
 * This constructor initializes and starts the whole application. It takes standard
 * command-line parameters
 *
 * @param argc The number of command-line parameters
 * @param argv The string array of parameters
 **/

Core::Core(int &argc, char* argv[]) : QApplication(argc, argv)
{
    this->setApplicationName("QGroundControl");
    this->setApplicationVersion("v. 0.0.5");
    this->setOrganizationName(QLatin1String("PIXHAWK Project"));
    this->setOrganizationDomain("http://pixhawk.ethz.ch");
    QSettings::setDefaultFormat(QSettings::IniFormat);
    // Exit main application when last window is closed
    connect(this, SIGNAL(lastWindowClosed()), this, SLOT(quit()));

    // Set application font
    QFontDatabase fontDatabase = QFontDatabase();
    const QString fontFileName = ":/general/vera.ttf"; ///< Font file is part of the QRC file and compiled into the app
    const QString fontFamilyName = "Bitstream Vera Sans";
    //if(!QFile::exists(fontFileName)) //printf("ERROR! font file: %s DOES NOT EXIST!", fontFileName);
    fontDatabase.addApplicationFont(fontFileName);
    setFont(fontDatabase.font(fontFamilyName, "Roman", 12));

    // Show splash screen
    QPixmap splashImage(":images/splash.png");
    QSplashScreen* splashScreen = new QSplashScreen(splashImage, Qt::WindowStaysOnTopHint);
    splashScreen->show();

    // Start the comm link manager
    splashScreen->showMessage(tr("QGroundControl (c) 2009-") + QString(QDate::currentDate().year()) + "PIXHAWK PROJECT", Qt::AlignLeft | Qt::AlignBottom);
    startLinkManager();

    // Start the UAS Manager
    startUASManager();

    //tarsus = new ViconTarsusProtocol();
    //tarsus->start();

    // Start the user interface
    startUI();

    // Remove splash screen
    splashScreen->finish(mainWindow);


}

/**
 * @brief Destructor for the groundstation. It destroys all loaded instances.
 *
 **/
Core::~Core()
{
    // Delete singletons
    delete LinkManager::instance();
    delete UASManager::instance();
}

/**
 * @brief Start the link managing component.
 *
 * The link manager keeps track of all communication links and provides the global
 * packet queue. It is the main communication hub
 **/
void Core::startLinkManager()
{
    LinkManager::instance();
}

/**
 * @brief Start the Unmanned Air System Manager
 *
 **/
void Core::startUASManager()
{
    UASManager::instance();
}

/**
 * @brief Start and show the user interface.
 **/
void Core::startUI()
{
    // Start UI
    mainWindow = new MainWindow();
    // Make UI visible
    mainWindow->show();
}

