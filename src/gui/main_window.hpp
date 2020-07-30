#ifndef GUI_MAIN_WINDOW_HPP
#define GUI_MAIN_WINDOW_HPP

#include <memory>

#include <QMainWindow>
#include <QWidget>

#include "2-3_tree_impl/2-3_tree.hpp"

// Include Qt generated files
//
#include "gui/main_window_control_panel.hpp"
#include "gui/two_three_tree_canvas.hpp"
#include "gui/ui_main_window.h"

namespace Ui {
    class MainWindow;
} // namespace Ui

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

    void InitControlPanel();
    void InitCanvas();

private slots:
    void AddNode(unsigned int key);
    void RemoveNode(unsigned int key);

private: // NOLINT
    void ReprintTree();

    const int kSplitterCanvasStretchFactor = 5;
    const int kSplitterControlPanelStretchFactor = 1;

    std::unique_ptr<Ui::MainWindow> ui_;
    TwoThreeTreeCanvas* canvas_;
    MainWindowControlPanel* control_panel_;
    two_three_tree::TwoThreeTree tree_;
};

#endif // !GUI_MAIN_WINDOW_HPP

