#include "editor.h"
#include "ui_editor.h"

#include "LevelScene.h"
#include "PropertyRequests.h"
#include "PropertyDelegate.h"
#include "PropertyPopulate.h"
#include "PropertyApply.h"
#include "PropertyRoles.h"

#include <fstream>
#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>


Editor::Editor(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Editor)
    , scene(nullptr)
{
    ui->setupUi(this);

    loadEnviroment("enviroment.env");

    setupMenu();
    setupStatusbar();
    setupView();
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
    level.read_object(id, [this](const game::object & obj)
    {
        propertyPopulate visitor(ui->propertiesView, obj, level.get_size());
        obj.accept(visitor);
    });
}

void Editor::sceneObjectRemoved(int id)
{
    emit propertiesClear();
}

void Editor::sceneError(int error)
{
    if (error == LevelScene::errorPrototypeEmpty)
    {
        QMessageBox::information(this, "Editor", "Please, choose a prototype", QMessageBox::Ok);
    }
    else if (error == LevelScene::errorObjectUniqueness)
    {
        QString protoname;

        QListWidgetItem * prototypeItem = ui->prototypesView->currentItem();
        if (prototypeItem)
            protoname = prototypeItem->data(Qt::UserRole).toString();

        QMessageBox::information(this, "Editor",
            "The object '" + protoname + "' is uniqueuness", QMessageBox::Ok);
    }

}

void Editor::propertyChanged(QTreeWidgetItem * item, int column)
{
    if (scene)
    {
        std::string name = item->data(1, PropertyRole::roleName).toString().toStdString();
        
        level.write_object(scene->selectedObject(), [&item, &name](game::object & obj)
        {
            propertyApply visitor(item->data(1, PropertyRole::roleData));
            obj.accept(name, visitor);
        });
       
        scene->update();
    }
}

void Editor::prototypeChanged(QListWidgetItem * item, QListWidgetItem * prev)
{
    if (scene)
        scene->setPrototype(item->data(Qt::UserRole).toString().toStdString());
}

void Editor::createLevel()
{
    if (!enviroment.empty())
    {
        level = game::level("Levelname", game::size(30, 15));
        setupLevel();
    }
}

void Editor::openLevel()
{
    if (!enviroment.empty())
    {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open Level"),
            "", tr("Files (*.lvl)"));
        std::ifstream in(fileName.toStdString().c_str());
        if (!fileName.isEmpty() && level.load(in, enviroment))
            setupLevel();
    }
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
        "<DEL> - remove selected item ");
    ui->statusbar->addWidget(label);
}

void Editor::setupView()
{
    ui->levelView->scale(1, -1);
}

void Editor::setupLevel()
{
    scene = new LevelScene(&level, &enviroment, this);
    ui->levelView->setScene(scene);

    QObject::connect(scene, SIGNAL(sceneObjectSelected(int)),
        this, SLOT(sceneObjectSelected(int)));

    QObject::connect(scene, SIGNAL(sceneObjectChanged(int)),
        this, SLOT(sceneObjectSelected(int)));

    QObject::connect(scene, SIGNAL(sceneObjectRemoved(int)),
        this, SLOT(sceneObjectRemoved(int)));

    QObject::connect(scene, SIGNAL(sceneObjectMoved(int, const game::point &)),
        this, SLOT(sceneObjectMoved(int, const game::point &)));

    QObject::connect(scene, SIGNAL(sceneError(int)),
        this, SLOT(sceneError(int)));

    ui->levelView->setEnabled(true);
    ui->dockWidget_properties->setEnabled(true);
    ui->dockWidget_prototypes->setEnabled(true);
    ui->actionSave->setEnabled(true);

    emit levelCreated();
}

void Editor::setupPrototypes()
{
    QPixmap tileset;
    tileset.load(enviroment.get_tileset().c_str());

    enviroment.foreach_prototype([&tileset, this](const game::prototype & proto)
    {
        supportPosition position;
        proto.accept(position);
        if (position.supported())
        {
            readTile visitor;
            proto.accept(visitor);

            game::point posTile = visitor.get_tile();
            game::size  sizeTile = enviroment.get_tilesize();

            QPixmap pixmap = tileset.copy(sizeTile.cx * posTile.x, sizeTile.cy * posTile.y,
                sizeTile.cx, sizeTile.cy);

            QString prototypeName(proto.get_name().c_str());
            QListWidgetItem * item = new QListWidgetItem(QIcon(pixmap), prototypeName);

            item->setData(Qt::UserRole, QVariant(prototypeName));
            ui->prototypesView->addItem(item);
        }
        return false;
    });

    QObject::connect(ui->prototypesView, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
        this, SLOT(prototypeChanged(QListWidgetItem*, QListWidgetItem*)));
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
    QObject::connect(this, SIGNAL(propertiesClear()),
        ui->propertiesView, SLOT(clear()));
}
