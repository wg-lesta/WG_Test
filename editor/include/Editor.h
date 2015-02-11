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

class Editor : public QMainWindow
{
    Q_OBJECT
public:

    explicit Editor(QWidget *parent = 0);
    ~Editor();

signals:

    void levelCreated();

private slots:

    void createLevel();
    void openLevel();
    void saveLevel();

    void sceneObjectSelected(int id);
    void sceneObjectRemoved(int id);
    void sceneObjectMoved(int id, const game::point & pos);

    void sceneCellSelected(const game::point&);

    void propertyChanged(QTreeWidgetItem*, int);

private:

    void loadEnviroment(const std::string & file);
    
    void setupMenu();
    void setupStatusbar();
    void setupView();
    void setupTilset();
    void setupPrototypes();
    void setupProperties();
    void setupLevel();

    void addObjectToScene(const game::object & obj, bool isSelected = false);
    bool canAddObjectToScene(const std::string & protoname);

    bool isPositionOccupied(const game::point & position) const;

private:

    Ui::Editor *ui;

    QPixmap tileset;
    LevelScene * scene;

    game::enviroment enviroment;
    game::level level;

    int selectedId;
};

#endif //__EDITOR_INCLUDED__
