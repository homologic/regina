
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  This program is distributed in the hope that it will be useful, but   *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *  General Public License for more details.                              *
 *                                                                        *
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/*! \file reginamain.h
 *  \brief Provides a main window for Regina.
 */

#ifndef __REGINAMAIN_H_
#define __REGINAMAIN_H_

#include "reginaiface.h"

#include <kapplication.h>
#include <kparts/mainwindow.h>

class KRecentFilesAction;
class KToggleAction;
class KURL;

/**
 * A top-level window for Regina.
 *
 * Each main window is used for a single data file.  Data files are
 * actually displayed and edited by embedded KParts.
 *
 * This class also stores global preferences for Regina.
 */
class ReginaMain : public KParts::MainWindow,
        virtual public ReginaMainInterface {
    Q_OBJECT

    private:
        /**
         * Components
         */
        KParts::ReadWritePart* currentPart;
            /**< The part containing the currently opened document, or 0 if
                 no document has yet been opened. */
        KURL lastURL;
            /**< The URL that was last contained in this window.
                 This data member is only set when the URL is finally
                 closed in the underlying part. */
        static unsigned objectNumber;
            /**< The unique positive integer to be assigned to the next
                 object of this class that is created.  Used with DCOP
                 to distinguish between different instances. */

        /**
         * Actions
         */
        KRecentFilesAction* fileOpenRecent;
            /**< The menu of recently opened files. */
        KToggleAction* showToolbar;
            /**< Action to show/hide the toolbar. */
        // KToggleAction* showStatusbar;
            /**< Action to show/hide the status bar. */

        /**
         * Preferences
         */
        bool autoDock;
            /**< Do we automatically dock new packet viewers into the
                 parent window? */
        bool autoFileExtension;
            /**< Should filenames be given an automatic extension? */
        bool displayIcon;
            /**< Should we display the pretty Regina icon? */

    public:
        /**
         * Constructors and destructors.
         */
        ReginaMain();
        virtual ~ReginaMain();

        /**
         * Retrieve global preferences for Regina.
         */
        bool getAutoDock() const;
        bool getAutoFileExtension() const;
        bool getDisplayIcon() const;

        /**
         * Set global preferences for Regina.  These routines update the
         * user interface accordingly, though they do not write the
         * preferences to the user's configuration file.
         */
        void setAutoDock(bool);
        void setAutoFileExtension(bool);
        void setDisplayIcon(bool);

        /**
         * Force this main window to reread the user's configuration
         * file and update itself (and its child windows) accordingly.
         */
        void readOptions();

        /**
         * Save the current preferences to the user's configuration file.
         * All other main windows will then be forced to reread this
         * configuration and update themselves accordingly.
         */
        void saveOptions();

    protected:
        /**
         * Overridden for drag-and-drop implementation.
         */
        virtual void dragEnterEvent(QDragEnterEvent *event);
        virtual void dropEvent(QDropEvent *event);

        /**
         * Overridden for session management.
         */
        virtual void saveProperties(KConfig *);
        virtual void readProperties(KConfig *);

        /**
         * Overridden to handle window closing.
         */
        virtual bool queryClose();
        virtual bool queryExit();

    signals:
        /**
         * Emitted when the 'auto dock' property for this main window is
         * changed.
         */
        void changedAutoDock(bool);

        /**
         * Emitted when the 'display icon' property for this main
         * window is changed.
         */
        void changedDisplayIcon(bool);

    public slots:
        /**
         * Opens a new topology data file in this window, or in a new
         * top-level window if this window already contains an open
         * document.
         */
        void newTopology();

        /**
         * Opens a new python library in this window, or in a new
         * top-level window if this window already contains an open
         * document.
         */
        void newPython();

        /**
         * Open the given URL in this window, or in a new top-level
         * window if this window already contains an open document.
         */
        bool openURL(const KURL& url);

        /**
         * Open the given URL in this window, or in a new top-level
         * window if this window already contains an open document.
         */
        bool openURL(const QString& url);

        /**
         * Open a new Python console.  The console will be linked
         * to the document in this window (if one exists).
         */
        void pythonConsole();

        /**
         * Closes this window.
         */
        void close();

        /**
         * Quits the entire application, closing all windows.
         */
        void quit();

    private slots:
        /**
         * Implementation of actions.
         */
        void fileOpen();
        void optionsShowToolbar();
        // void optionsShowStatusbar();
        void optionsConfigureKeys();
        void optionsConfigureToolbars();
        void optionsPreferences();

        // void changeStatusbar(const QString& text);
        void changeCaption(const QString& text);
        void newToolbarConfig();

        /**
         * Add the current working URL to the recent file list for every
         * top-level window (including this one) and save the file list to
         * the global configuration.
         */
        void addRecentFile();

    private:
        /**
         * Initial setup.
         */
        void setupActions();

        /**
         * Force this main window to read the given configuration
         * and update itself (and its child windows) accordingly.
         */
        void readOptions(KConfig* config);

        /**
         * Creates a new topology data part.  If no appropriate part can
         * be created, an error is displayed and 0 is returned.
         */
        KParts::ReadWritePart* newTopologyPart();

        /**
         * Creates a new text editor part.  If no appropriate part can
         * be created, an error is displayed and 0 is returned.
         */
        KParts::ReadWritePart* newTextEditorPart();

        /**
         * Inserts \a currentPart into the main window and performs any
         * additional setup that is required.  This routine can cope if
         * \a currentPart is 0.
         */
        void embedPart();
};

inline ReginaMain::~ReginaMain() {
}

inline bool ReginaMain::getAutoDock() const {
    return autoDock;
}

inline bool ReginaMain::getAutoFileExtension() const {
    return autoFileExtension;
}

inline bool ReginaMain::getDisplayIcon() const {
    return displayIcon;
}

inline void ReginaMain::setAutoFileExtension(bool value) {
    autoFileExtension = value;
}

inline void ReginaMain::readOptions() {
    readOptions(KGlobal::config());
}

#endif
