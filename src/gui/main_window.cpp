#include "gui/main_window.hpp"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui_(std::make_unique<Ui::MainWindow>())
{
    ui_->setupUi(this);

    InitCanvas();
    InitControlPanel();
    ui_->splitter->setStretchFactor(0, 5);
    ui_->splitter->setStretchFactor(1, 1);
}

void MainWindow::InitControlPanel() {
    control_panel_ = new MainWindowControlPanel(ui_->splitter);
    ui_->splitter->addWidget(control_panel_);

    connect(
        control_panel_, &MainWindowControlPanel::AddNode,
        this, [=](unsigned int key) { tree_.Add(key); ReprintTree(); }
    );
    connect(
        control_panel_, &MainWindowControlPanel::RemoveNode,
        this, [=](unsigned int key) { tree_.Remove(key); ReprintTree(); }
    );
}

void MainWindow::InitCanvas() {
    canvas_ = new TwoThreeTreeCanvas(ui_->splitter);
    ui_->splitter->addWidget(canvas_);
}

void MainWindow::ReprintTree() {
    canvas_->PrintTree(tree_.begin(), tree_.end());
}

MainWindow::~MainWindow() {
}

