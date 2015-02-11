#include "editor.h"
#include "ui_editor.h"

#include "LevelScene.h"
#include "LevelSceneObject.h"
#include "PropertyRequests.h"
#include "PropertyDelegate.h"
#include "PropertyPopulate.h"
#include "PropertyDeliver.h"
#include "PropertyRoles.h"

#include <fstream>
#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>


Editor::Editor(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Editor)
    , scene(nullptr)
    , selectedId(0)
{
    ui->setupUi(this);

    loadEnviroment("enviroment.env");

    setupMenu();
    setupStatusbar();
    setupView();
    setupTilset();
    setupPrototypes();
    setupProperties();
}

Editor::~Editor()
{
    delete ui;
}

void Editor::sceneObjectSelected(int id)
{
    ui->propertiesView->clear();

    selectedId = id;
    level.read_object(id, [this](const game::object & obj)
    {
        propertyPopulate visitor(ui->propertiesView, obj, level.get_size());
        obj.accept(visitor);
    });
}

void Editor::sceneObjectRemoved(int id)
{
    level.remove(id);
}

void Editor::sceneObjectMoved(int id, const game::point & pos)
{
    level.write_object(id, [&pos](game::object & obj)
    {
        writePosition visitor(pos);
        obj.accept(visitor);
    });
}

void Editor::sceneCellSelected(const game::point & position)
{
    if (isPositionOccupied(position))
        return;

    QListWidgetItem * prototypeItem = ui->prototypesView->currentItem();
    if (prototypeItem)
    {
        QString protoname = prototypeItem->data(Qt::UserRole).toString();

        if (canAddObjectToScene(protoname.toStdString()))
        {
            int idObject = level.insert(protoname.toStdString(), enviroment);
            sceneObjectMoved(idObject, position);

            level.read_object(idObject, [this](const game::object & obj)
            {
                addObjectToScene(obj, true);
            });
        }
        else
        {
            QMessageBox::information(this, "Editor", 
                "The object '" + protoname + "' is uniqueuness", QMessageBox::Ok);
        }
    }
    else
    {
        QMessageBox::information(this, "Editor", "Please, choose a prototype", QMessageBox::Ok);
    }
}

void Editor::propertyChanged(QTreeWidgetItem * item, int column)
{
    QString name = item->data(1, PropertyRole::roleName).toString();

    level.write_object(selectedId, [&item, &name](game::object & obj)
    {
        propertyDeliver visitor(item);
        obj.accept(name.toStdString(), visitor);
        
    });
}

void Editor::createLevel()
{
    level = game::level("Levelname", game::size(30, 15));
    setupLevel();
}

void Editor::openLevel()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Level"),
        "", tr("Files (*.lvl)"));
    std::ifstream in(fileName.toStdString().c_str());
    if (!fileName.isEmpty() && level.load(in, enviroment))
        setupLevel();
}

void Editor::saveLevel()
{
    if (scene)
    {
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save Level"), "./", tr("Files (*.lvl)"));
        std::ofstream out(fileName.toStdString().c_str());
        level.save(out);
    }
}


void Editor::loadEnviroment(const std::string & file)
{
    enviroment.load(std::ifstream(file));
}

void Editor::setupMenu()
{
    QObject::connect(ui->actionCreate, SIGNAL(triggered()), this, SLOT(createLevel()));
    QObject::connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openLevel()));
    QObject::connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(saveLevel()));
}

void Editor::setupStatusbar()
{
    QLabel * label = new QLabel();
    label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    label->setText("<LBUTTON> - selected/added an item |  "
        "<LBUTTON + CTRL> - selected items  |  "
        "<DEL> - remove selected item(s)  ");
    ui->statusbar->addWidget(label);
}

void Editor::setupView()
{
    ui->levelView->scale(1, -1);
}

void Editor::setupTilset()
{
    tileset.load(enviroment.get_tileset().c_str());
}

void Editor::setupLevel()
{
    QSize sizeTile(enviroment.get_tailsize().cx, enviroment.get_tailsize().cy);
    QSize sizeLevel(level.get_size().cx, level.get_size().cy);

    scene = new LevelScene(sizeLevel, sizeTile);
    
    ui->levelView->setScene(scene);

    level.foreach_objects([this](const game::object & obj)
    {
        addObjectToScene(obj);
        return false;
    });

    QObject::connect(scene, SIGNAL(cellSelected(const game::point&)),
        this, SLOT(sceneCellSelected(const game::point&)));

    ui->levelView->setEnabled(true);
    ui->dockWidget_properties->setEnabled(true);
    ui->dockWidget_prototypes->setEnabled(true);
    ui->actionSave->setEnabled(true);

    emit levelCreated();
}

void Editor::setupPrototypes()
{
    enviroment.foreach_prototype([this](const game::prototype & proto)
    {
        supportPosition position;
        proto.accept("Position", position);
        if (position.supported())
        {
            readTile visitor;
            proto.accept("Tile", visitor);

            game::point posTile = visitor.get_tile();
            game::size  sizeTile = enviroment.get_tailsize();

            QPixmap pixmap = tileset.copy(sizeTile.cx * posTile.x, sizeTile.cy * posTile.y,
                sizeTile.cx, sizeTile.cy);

            QString prototypeName(proto.get_name().c_str());
            QListWidgetItem * item = new QListWidgetItem(QIcon(pixmap), prototypeName);

            item->setData(Qt::UserRole, QVariant(prototypeName));
            ui->prototypesView->addItem(item);
        }
        return false;
    });
}

void Editor::setupProperties()
{
    ui->propertiesView->headerItem()->setText(0, "Property");
    ui->propertiesView->headerItem()->setText(1, "Value");

    ui->propertiesView->setItemDelegate(new PropertyDelegate);

    QObject::connect(ui->propertiesView, SIGNAL(itemChanged(QTreeWidgetItem*, int)),
        this, SLOT(propertyChanged(QTreeWidgetItem*, int)));
    
    QObject::connect(this, SIGNAL(levelCreated()),
        ui->propertiesView, SLOT(clear()));
}

void Editor::addObjectToScene(const game::object & obj, bool isSelected)
{
    LevelSceneObject *newObject = scene->addObject(obj, tileset);

    QObject::connect(newObject, SIGNAL(selected(int)),
        this, SLOT(sceneObjectSelected(int)));

    QObject::connect(newObject, SIGNAL(moved(int, const game::point&)),
        this, SLOT(sceneObjectMoved(int, const game::point&)));

    QObject::connect(newObject, SIGNAL(remove(int)),
        this, SLOT(sceneObjectRemoved(int)));

    if (isSelected)
    {
        newObject->setSelected(true);
        sceneObjectSelected(newObject->getId());
    }
}

bool Editor::canAddObjectToScene(const std::string & protoname)
{
    readUniqueness visitor;
    enviroment.request_property("Uniqueness", protoname, visitor);
    if (visitor.is_uniqueness())
    {
        bool result = level.foreach_objects([&protoname](const game::object & obj)
        {
            return obj.get_name() == protoname;
        });
        return !result;
    }
    return true;
}

bool Editor::isPositionOccupied(const game::point & position) const
{
    bool result = level.foreach_objects([&position](const game::object & obj)
    {
        readPosition visitor;
        obj.accept("Position", visitor);
        return visitor.get_position() == position;
    });

    return result;
}
