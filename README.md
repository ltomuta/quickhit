QuickHit
========

This Nokia Developer example application demonstrates the use of Qt Quick as
a game platform. QuickHit is essentially a basic shooting game, but
differentiated from other such games by its levels. The levels are Qt plug-ins
which are loaded during QuickHit startup. Different levels are shown in the
application's main menu. Levels can have different sounds, graphics, and level
QML files. A common characteristic for all levels is that your ship is at the
bottom of the screen and shoots upwards. 

This application has been tested on Symbian, MeeGo 1.2 Harmattan, 
Qt Simulator, and Windows desktop.

This example application is hosted in Nokia Developer Projects:
- http://projects.developer.nokia.com/quickhit

For more information on implementation, visit the wiki page:
- http://projects.forum.nokia.com/quickhit/wiki


What's new
----------

IAP (In-Application-Purchase) features were added into the 2.0 version of the
game. New levels can now be purchased from the Nokia Store. This example
demonstrates the use of non-DRM purchasing. The levels to purchase are not 
bundled in the original installation. Instead, the levels are downloaded
from a back-end server once payment has been made.

Learn more about the IAP implementation and IAP in general at:
- http://projects.developer.nokia.com/quickhit/wiki/iap 
- http://projects.developer.nokia.com/iap

Note that IAP binaries must be installed on the Symbian device to run the
IAP-enabled version of the game.


1. Usage
-------------------------------------------------------------------------------

1.1 Level plug-ins
------------------

All levels have to implement the QuickHit level plug-in interface defined in 
levelplugininterface.h, and they get the same 
'Forum.Nokia.com.QuickHit.LevelPluginInterface/1.0' plug-in identifier. The 
interface header file exists in the folder /QuickHit/plugins.

Installed level plug-in binaries (libraries) are automatically loaded when the
application is started. Note that in Windows desktop, the DLL files must be 
copied into the shadow build folder created by the Qt SDK.


1.2 How to play
---------------

QuickHit finds level plug-ins during application startup and shows them in the 
main menu. Select 'LevelOne' or 'LevelTwo' to start a level which is loaded 
dynamically.

On keyboard arrays, pressing right or left moves your ship and pressing the 
space bar fires missiles.

On touch-enabled devices, you can move the ship with a finger and touch the 
ship to fire missiles.


2. Prerequisites
-------------------------------------------------------------------------------

 - Qt basics
 - Qt Quick basics


3. Project structure and implementation
-------------------------------------------------------------------------------

3.1 Folders
-----------

 |                  The root folder contains the project file, resource files,
 |                  the license information, and this file (release notes).
 |
 |- bin             Contains the compiled binaries.
 |
 |- doc             Contains the documentation, including class diagram etc.
 |
 |- ge_src          Qt GameEnabler audio framework source files.
 |
 |- gfx             Contains application graphics.
 |
 |- iap_src         In-Application Purchase specific implementation.
 |
 |- icons           Contains application icons.
 |
 |- plugins         Contains the level plug-ins.
 |
 |- qml             Contains the QML and JavaScript files.
 |
 |- qtc_packaging   Contains the Harmattan (Debian) packaging files.
 |
 |- sound           QuickHit's internal sounds, levels have their own sounds.
 |
 |- src             Contains the Qt/C++ source code files.


3.2 Important files and classes
-------------------------------

- mainwindow.h: Qt Quick QDeclarativeView
- Game.qml: The main QML file

Important classes: QDeclarativeView, IAPClient


4. Compatibility
-------------------------------------------------------------------------------

- Qt 4.7.4 or higher
- QtMobility 1.2.1 or higher
- IAP API

Tested on:
- Nokia C7-00
- Nokia E7-00
- Nokia N8-00
- Nokia N9
- Nokia N950
- Windows desktop
- Qt Simulator

Developed with Qt SDK 1.2.


4.1 Required capabilities on Symbian
------------------------------------

LocalServices, NetworkServices, ReadUserData, WriteUserData and 
UserEnvironment.

The application can be self signed.


4.2 Known issues
----------------

None.


5. Building, installing, and running the application
-------------------------------------------------------------------------------

5.1 Preparations
----------------

Check that you have the latest Qt SDK installed in the development environment
and the latest Qt version on the device.


5.2 Using Qt SDK
----------------

You can install and run the application on the device by using the Qt SDK.
Open the project in the SDK, set up the correct target (depending on the 
device platform), and click the Run button. For more details about this 
approach, please visit Qt Getting Started at Nokia Developer:
http://www.developer.nokia.com/Develop/Qt/Getting_started/


5.3 Symbian device
------------------

Make sure your device is connected to your computer. Locate the SIS 
installation file and open it with Ovi Suite. Accept all requests from Ovi
Suite and the device. Note that you can also install the application by 
copying the installation file onto your device and opening it with the Symbian 
File Manager application.

After the application is installed, locate the application icon in the
application menu and launch the application by tapping the icon.


5.4 Nokia N9 and Nokia N950
---------------------------

Copy the application Debian package onto the device. Locate the file with the
device and run it to install the application. Note that you can also use the 
terminal application and install the application by typing the command
'dpkg -i <package name>.deb' on the command line. To install the application
using the terminal application, make sure you have the right privileges 
to do so (root access).

Once the application is installed, locate the application icon in the
application menu and launch the application by tapping the icon.


6. License
-------------------------------------------------------------------------------

See the license text file delivered with this project. The license file is 
also available online at:
http://projects.developer.nokia.com/quickhit/browser/trunk/Licence.txt


Music and sound effects
-----------------------

QuickHit uses sounds from freesound.org.

The sounds were loaded from www.freesound.org under the Creative Commons 
Sampling Plus 1.0 license.

The sounds were created by these nicknames: HardPCM, Matt_G, klankbeeld, 
steveygos93, joe93barlow, ljudman, Jovica, patchen, nthompson.


7. Related documentation
-------------------------------------------------------------------------------

- Qt Quick: http://qt.nokia.com/qtquick/
- Monetizing your app: http://www.developer.nokia.com/Distribute/Monetizing_your_app.xhtml
- Qt GameEnabler project: http://projects.developer.nokia.com/qtgameenabler


8. Version history
-------------------------------------------------------------------------------

2.0.3   Purchased items restoration removed because it is not anymore supported in IAP non-DRM applications.
2.0.2   Symbian Belle support checked.
2.0.1   IAP updates.
2.0.0   New UI look and added IAP functionality for Symbian.
1.2.0   Added MeeGo 1.2 Harmattan support.
1.1.1   Qt SDK 1.1 beta, Qt 4.7.2, and Qt Mobility 1.1 fixes.
1.1.0   Final graphics and fixes. The game will be paused on an incoming call. 
        Level Two has a scrolling background.
1.0.0   First version. Published in FN Projects only.