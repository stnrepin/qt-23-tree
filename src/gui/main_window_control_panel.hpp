#ifndef GUI_MAIN_WINDOW_CONTROL_PANEL_HPP
#define GUI_MAIN_WINDOW_CONTROL_PANEL_HPP

#include <memory>
#include <utility>

#include <QWidget>
#include <QFrame>

// Include Qt generated files
//
#include "gui/ui_main_window_control_panel.h"

namespace Ui {
    class MainWindowControlPanel;
}

class MainWindowControlPanel : public QFrame {
    Q_OBJECT

public:
    explicit MainWindowControlPanel(QWidget* parent = nullptr);
    ~MainWindowControlPanel() noexcept;

signals:
    void AddNode(int key);
    void RemoveNode(int key);

private:
    enum class Message {
        kInvalidKeyEntered,
        kNodeAdded,
        kNodeRemoved,
    };

    void AddButtonClicked();
    void RemoveButtonClicked();
    std::pair<unsigned int, bool> ReadNodeKey();
    void PrintMessage(Message mes);

    std::unique_ptr<Ui::MainWindowControlPanel> ui_;
};

#endif // !GUI_MAIN_WINDOW_CONTROL_PANEL_HPP

