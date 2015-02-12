#ifndef __EDITOR_INCLUDED__
#define __EDITOR_INCLUDED__

#include <QMainWindow>
#include <qmainwindow.h>
#include <game/enviroment.h>
#include <game/level.h>

namespace Ui 
{
    class Editor;
}

class LevelScene;
class QTreeWidgetItem;
class QListWidgetItem;

class Editor : public QMainWindow
{
    Q_OBJECT
public:

    explicit Editor(QWidget *parent = 0);
    ~Editor();

signals:

    void levelCreated();
    void propertiesClear();

private slots:

    void createLevel();
    void openLevel();
    void saveLevel();

    void sceneObjectSelected(int id);
    void sceneObjectRemoved(int id);
    void sceneError(int error);

    void propertyChanged(QTreeWidgetItem*, int);
    void prototypeChanged(QListWidgetItem*, QListWidgetItem*);

private:

    void loadEnviroment(const std::string & file);
    
    void setupMenu();
    void setupStatusbar();
    void setupView();
    void setupPrototypes();
    void setupProperties();
    void setupLevel();

private:

    Ui::Editor *ui;

    LevelScene * scene;

    game::enviroment enviroment;
    game::level level;
};

#endif //__EDITOR_INCLUDED__
