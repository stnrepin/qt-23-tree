#ifndef GUI_MAIN_WINDOW_HPP
#define GUI_MAIN_WINDOW_HPP

#include <memory>

#include <QWidget>
#include <QMainWindow>

#include "2-3_tree_impl/2-3_tree.hpp"

// Include Qt generated files
//
#include "gui/ui_main_window.h"
#include "gui/two_three_tree_canvas.hpp"
#include "gui/main_window_control_panel.hpp"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() noexcept;

    void InitControlPanel();
    void InitCanvas();
    void ReprintTree();

private:
    std::unique_ptr<Ui::MainWindow> ui_;
    TwoThreeTreeCanvas* canvas_;
    MainWindowControlPanel* control_panel_;
    TwoThreeTree tree_;
};

#endif // !GUI_MAIN_WINDOW_HPP

