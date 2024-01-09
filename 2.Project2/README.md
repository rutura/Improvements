* This application is meant to display jokes about Chuck Norris in a QML ListView.
* It is writen in Qt5 and uses a HTTP API endpoint that has changed
    * We used to get all the jokes in one go: 
        * http://api.icndb.com/jokes/random/<number>
    * Now the changes to the api are documented here: 
        https://api.chucknorris.io/

* The task: 
    * Update the application to use the latest Qt release and CMake
    * Update the application to use the new API endpoint
        * You will add a button in the UI to fetch the jokes and add them to the model for display
    * You will read on different ways to integrate C++ and qml (https://doc.qt.io/qt-6/qtqml-cppintegration-overview.html) and choose the one you think is best for this application
    * Change the application not to use the Qt resource system, and use qt_add_resources (https://doc.qt.io/qt-6/resources.html#build-system-integration)

* Deliverables: 
    * Clone the repository, create a branch, make the changes, push the branch, and create a pull request.
    * Document the changes you made in the pull request.
        * IMPORTANT:
            * Explain why you chose the integration method you used.