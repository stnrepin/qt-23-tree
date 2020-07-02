#include "gui/main_window.hpp"

#include <exception>

#include "2-3_tree_impl/exceptions.hpp"

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
        this, &MainWindow::AddNode
    );
    connect(
        control_panel_, &MainWindowControlPanel::RemoveNode,
        this, &MainWindow::RemoveNode
    );
}

void MainWindow::InitCanvas() {
    canvas_ = new TwoThreeTreeCanvas(ui_->splitter);
    ui_->splitter->addWidget(canvas_);
}

void MainWindow::AddNode(unsigned int key) {
    try {
        tree_.Add(key);
        control_panel_->UpdateMessage(MainWindowControlPanel::Message::kNodeAdded);
    }
    catch(const two_three_tree::KeyExistsException& e) {
        control_panel_->UpdateMessage( MainWindowControlPanel::Message::kKeyExists);
    }
    catch(const std::exception& e) {
        control_panel_->UpdateMessage(
            MainWindowControlPanel::Message::kUnknownErrorOccurred
        );
    }

    ReprintTree();
}

void MainWindow::RemoveNode(unsigned int key) {
    try {
        tree_.Remove(key);
        control_panel_->UpdateMessage(MainWindowControlPanel::Message::kNodeRemoved);
    }
    catch(const two_three_tree::KeyNotExistsException& e) {
        control_panel_->UpdateMessage(
            MainWindowControlPanel::Message::kKeyNotExists
        );
    }
    catch(const std::exception& e) {
        control_panel_->UpdateMessage(
            MainWindowControlPanel::Message::kUnknownErrorOccurred
        );
    }

    ReprintTree();
}

void MainWindow::ReprintTree() {
    canvas_->PrintTree(tree_.begin(), tree_.end());
}

